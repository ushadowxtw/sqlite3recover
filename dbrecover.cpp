
#include "dbrecover.h"
#include "basetype.h"
#include "sqliteapi.h"
#include "pageanalyer.h"
dbrecover::dbrecover()
{
    m_dbbuf=NULL;
    m_dbfilelen=0;
    m_file=NULL;
}


dbrecover::~dbrecover()
{

    if(m_file)
        fclose(m_file);
    if(m_dbbuf)
        delete [] m_dbbuf;
    //
    map<string,tableanalyer*> ::iterator itt;
    for(itt=m_tabledetailmap.begin();itt!=m_tabledetailmap.end();itt++)
    {
        delete itt->second;
    }
    
    
}
//插入数据库
/**
RET :

1-不能恢复的数据库(auto_vacuum)
0-成功
-1 不可知错误
-2 数据库文件异常
-3 读取库头失败
-4 读取表失败
*/
int dbrecover::insertDB(const char*filename)
{
    //判断是否存在
    if(access(filename,0)!=0)
    {
        printf("error:db file %s not exist\r\n",filename);
        return -2;
    }
    m_dbfile=filename;
    int retflg=readtableinfo();
    if(retflg!=0)
        return retflg;

    if(m_file)
        return -1;

    m_file = fopen(filename, "rb");
    if(!m_file)
    {
        printf("error:db file %s open error\r\n",filename);
        return -2;
    }
    fseek(m_file, 0, SEEK_END);
    m_dbfilelen = ftell(m_file);
    fseek(m_file, 0, SEEK_SET);

    
    if(m_dbfilelen<=sizeof(DATABASEHEADER))
    {
        printf("error:db file %s size error\r\n",filename);
        return -2;
    }

    //
    //读取库头
    int readlen = fread(&m_dbhead, 1, sizeof(DATABASEHEADER), m_file);
    
    if(readlen!= sizeof(DATABASEHEADER))
    {

        return -3;
    }
    fseek(m_file, 0, SEEK_SET);
    m_dbbuf=new char[m_dbfilelen+1];
    fread(m_dbbuf, 1, m_dbfilelen, m_file);

    retflg=checkdbhead();
    if(retflg!=0)
        return retflg;
    



    //读取数据库BUF
    if(m_file)
    {
     
        fclose(m_file);
        m_file=NULL;
    }
    
    analytableinfo();
    
    
    return 0;
    


}
//验证头
int  dbrecover::checkdbhead()
{
    
    m_dbhead.pagesize = ntohs(m_dbhead.pagesize);
    m_dbhead.changecount = ntohl(m_dbhead.changecount);
    m_dbhead.totalpages = ntohl(m_dbhead.totalpages);
    m_dbhead.firstfreepageid = ntohl(m_dbhead.firstfreepageid);
    m_dbhead.freepagecount = ntohl(m_dbhead.freepagecount);
    m_dbhead.schemaformatnumber = ntohl(m_dbhead.schemaformatnumber);
    m_dbhead.defaultpagecachesize = ntohl(m_dbhead.defaultpagecachesize);
    m_dbhead.largestrootpageid = ntohl(m_dbhead.largestrootpageid);
    m_dbhead.textencode = ntohl(m_dbhead.textencode);
    m_dbhead.userversion = ntohl(m_dbhead.userversion);
    m_dbhead.ncrementalvacuummod = ntohl(m_dbhead.ncrementalvacuummod);
    m_dbhead.applicationid = ntohl(m_dbhead.applicationid);
    m_dbhead.versionvalidfornumber = ntohl(m_dbhead.versionvalidfornumber);
    m_dbhead.sqliteversion = ntohl(m_dbhead.sqliteversion);

    if(memcmp(m_dbhead.magic, "SQLite format 3\0", 0x10)!=0)
    {
            printf("error:db head error\r\n");
            return -3;
    }

    if(m_dbhead.totalpages*m_dbhead.pagesize!=m_dbfilelen)
    {
            printf("error:db size error\r\n");
            return -3;
    }

    return 0;

}

//数据库的表结构
//返回表的数量
int dbrecover::readtableinfo()
{

    sqliteapi onesqliteapi;
    bool reflg=onesqliteapi.LoadDBFile(m_dbfile.c_str() );
    if(!reflg)
        return -4;
   

   //查询表
    string sqlcmd ="select type,name,sql,rootpage from sqlite_master where type == \"table\"";
    
    string retstr="";
    onesqliteapi.SqlQuery( sqlcmd.c_str(),retstr);
    
    vector<string> fieldvect;
    SpliteStoV((char*)retstr.c_str(),retstr.length(),(char*)"\1",fieldvect,true);
    for(int m=0;m<fieldvect.size();m++)
    {
        vector<string> infor;
        SpliteStoV((char*)fieldvect[m].c_str(),fieldvect[m].length(),(char*)"\2",infor,true);
        if(infor.size()==4)
        {
            CREATTABLEINFO oneCREATTABLEINFO;
            string tablename=infor.at(1);
            string rootpage=infor.at(3);
            string sqlstr=infor.at(2);
            
            oneCREATTABLEINFO.tabletype=0;
            oneCREATTABLEINFO.rootpage=(atoi(rootpage.c_str())-1);
            oneCREATTABLEINFO.creatsql=sqlstr;
            oneCREATTABLEINFO.tablename=tablename;

            if(oneCREATTABLEINFO.rootpage<0)
            {//空表 暂不考虑
                 continue;
            }
            bool isgetcolname=onesqliteapi.GetColName(oneCREATTABLEINFO.fieldarr, tablename);
            if(!isgetcolname)
            {
                printf("error:table %s get feild error\r\n",tablename.c_str());
                continue;
            }
            bool  isgetcoltype=onesqliteapi.GetColType(oneCREATTABLEINFO.fieldtype,tablename);
            if(!isgetcoltype)
            {
                 printf("error:table %s get type error\r\n",tablename.c_str());
                continue;
            }
            if(oneCREATTABLEINFO.fieldtype.size()!=oneCREATTABLEINFO.fieldarr.size())
            {
                 printf("error:table %s chechk getinfo error\r\n",tablename.c_str());
                 continue;
            }
          
            oneCREATTABLEINFO.fieldnum=oneCREATTABLEINFO.fieldtype.size();
            if(oneCREATTABLEINFO.fieldnum==0)
            {//空表 暂不考虑
                 continue;
            }
            map<string,CREATTABLEINFO>::iterator itt;
            itt=m_tablemap.find(tablename);
            if(itt==m_tablemap.end())
            {
//                 printf("success:table %s getinfo success\r\n",tablename.c_str());
//                 printf("success:table %s getinfo success\r\n",oneCREATTABLEINFO.tablename.c_str());
//                 printf("success:fieldnum %d \r\n",oneCREATTABLEINFO.fieldnum);
//                 printf("success:rootpage %d\r\n",oneCREATTABLEINFO.rootpage);
                m_tablemap.insert(make_pair(tablename,oneCREATTABLEINFO));
            }
        }
    }
    return 0;
}
//拆分页
int dbrecover::analytableinfo()
{
    map<string,CREATTABLEINFO>::iterator itt;
    for(itt=m_tablemap.begin();itt!=m_tablemap.end();itt++)
    {

        tableanalyer *onetableanalyer=new tableanalyer;
        if(onetableanalyer->inserttable(m_dbbuf,m_dbfilelen,m_dbhead,itt->second))
        {
            m_tabledetailmap.insert(make_pair(itt->first,onetableanalyer));
        }
        else
        {
            delete onetableanalyer;
        }
    }




}


//获取某表已经删除数据
//ret 数量
int dbrecover::getdeltabledata(string tablename,vector<CHECKFILED> filedarr,fieldstruct* onefieldstruct)
{
    int ret=0;


    map<string,CREATTABLEINFO> ::iterator it;
    it=m_tablemap.find(tablename);
    if(it==m_tablemap.end())
    {
        return ret;   
    }

    //验证
    for(int n=0;n<filedarr.size();n++)
    {
        int index=getfeildindex(it->second.fieldarr, filedarr.at(n).key);
        if(index<0)
        {   
            ret=-1;
            break;
        }
        filedarr.at(n).index=index;
        

    }
    if(ret<0)
    {
        return ret;
    }
    map<string,tableanalyer*> ::iterator itt;
    itt=m_tabledetailmap.find(tablename);
    if(itt!=m_tabledetailmap.end())
    {
        itt->second->prasingpage( onefieldstruct, filedarr,it->second);
    }
    
    return onefieldstruct->getsize();
    
}




