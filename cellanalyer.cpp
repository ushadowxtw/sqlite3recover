
#include "cellanalyer.h"

cellanalyer::cellanalyer()
{
    m_ischeckfirst=false;
    m_firstfeildtype=-1;//不存在
    m_firstlen=-1;//不存在
}


cellanalyer::~cellanalyer()
{
}
//拆解自由块到单元
/**

单元信息
    var(1-9) 可变长整数 payload长度
    var(1-9) 可变长整数  rowid 
    payload | (假设有N个字段)N+1个可变长整数 |
            | head－size 可变长整数，记录头的长度
            | (datatype-datalen)*N (N个可变长整数)| ....|...
            |
            |
    unsigned int 溢出页ID(不一定存在)     

    1.查找起始点 ＊＊＊＊＊
        由自由块增样确定真实的payload起始位置是恢复数据中最困难的部分
        A.自由块前4个字节是覆盖的原单元4个字节   由上可知可能覆盖第一个字段的datatype-datalen
        B.除A,会保留完整的字段信息(N＊datalen)
            A.1 A种情况会丢失第一个字段信息 导致不能确定第一字段的 导致不能找到字段数据开始位置
            A.2 A种情况出现概率与字段数量以及值的总长度有关，字段少而且字段值全比较短，则A情况出现的概率很大
            A.3 A种情况时开始字节并非一定为第二个字段的 (datatype-datalen),在第一个字段为TEX｜BLOB时，可能为第一个字段的(datatype-datalen)一部分
            A.4 因此在假设为A种情况时，需要提供第一个字段的信息：类型 是否为主键
            A.5 A种情况此程序只考虑：1.第一个字段为 主键 并 类型为 INTEGER 2.确定第一字段(datatype-datalen)占一字节并且明确第一字段占的位数
            A.6 A种情况可以通过为删除的单元进行智能判断，在此不做考虑
        



    2.获取字段值类型与长度

**/
//拆解自由块单元
void cellanalyer::prasingcell(int &line,fieldstruct* onefieldstruct,vector<CHECKFILED> filedarr,CREATTABLEINFO oneCREATTABLEINFO,char* celldata,unsigned int celllen)
{

    
    //1. 判断参数合法性
    if(filedarr.size()==0)
    {
        return ;
    }
    if(!celldata||celllen==0)
    {
        return ;
    }
    if(oneCREATTABLEINFO.fieldnum>celllen)
    {
        return ;
    }

    getfirstfield( filedarr);

    int startpos=0;
    //检测  -1---6 
    if(m_firstfeildtype==1&&m_firstlen==1)//-1----6
    {
        
        startpos=-1;
    }
    int usehollen=0;
    while(startpos<7)
    {
        vector<CHECKBYTE> feildarr;
       
        if(operationbyte(startpos,celldata,celllen,oneCREATTABLEINFO.fieldnum, feildarr,usehollen))
        {
            
            if(parefeild( filedarr, feildarr))
            {
                //插入字段
    //          insert(int line,string key,char* value,unsigned int vlen);
                line++;
                for(int i=0;i<feildarr.size();i++)
                {
                    string key=oneCREATTABLEINFO.fieldarr.at(i);
                    
        
                   onefieldstruct->insert( line, key,feildarr.at(i).value,feildarr.at(i).valuelen);
                    
                }

                break;
            }
            

        }
        //释放资源
        for(int m=0;m<feildarr.size();m++)
        {
            if(feildarr.at(m).value)
            {
                delete [] feildarr.at(m).value;
            }
        }

        startpos++;
    }

   if(celllen>(usehollen+4))
   {
        char *nextdata=celldata+(usehollen+4);
        unsigned int nextlen=celllen-(usehollen+4);
        prasingcell(line,onefieldstruct,filedarr, oneCREATTABLEINFO,nextdata,nextlen);
   }
    
    
}
//检测
bool cellanalyer::parefeild(vector<CHECKFILED> filedarr,vector<CHECKBYTE> feildarr)
{
    bool retflg=true;
    for(int m=0;m<filedarr.size();m++)
    {
        int findex=filedarr.at(m).index;
        if(findex>=feildarr.size())
        {
             retflg=false;
             break;
        }


        if((feildarr.at(findex).type!=-1)&&(filedarr.at(m).type!=feildarr.at(findex).type))
        {
            if(filedarr.at(m).type==2&&(feildarr.at(findex).type!=1))//特  如果验证的字段为 FLOAT 但实际数值小数点后为0,则有时标识为 INT ＝＝1
            {
                 retflg=false;
                break;
            }
        }
        if(filedarr.at(m).checktype==-1)
        {
            continue;
        }
        if(filedarr.at(m).checktype==1||filedarr.at(m).checktype==3)//比较长度
        {
            if((feildarr.at(findex).bytelen<filedarr.at(m).minlen)||(feildarr.at(findex).bytelen>filedarr.at(m).maxlen))
            {
                retflg=false;
                break;
            }
        }
        if(filedarr.at(m).checktype==2||filedarr.at(m).checktype==3)
        {
            if(feildarr.at(findex).type!=1)
            {
                retflg=false;
                break;
            }
            if((feildarr.at(findex).intvalue<filedarr.at(m).min)||(feildarr.at(findex).intvalue>filedarr.at(m).max))
            {
                retflg=false;
                break;
            }
        }
     //  if(filedarr.at(m).checktype==4)//只检验数据类型
       // {

        //}
        
    }
    
    return retflg;
    
}

//
bool cellanalyer::operationbyte(int startpos,char* data,unsigned int datalen,int feildsize,vector<CHECKBYTE>& feildarr,int &usehollen)
{

    bool ret=true;
    int feildpos=0;
    usehollen=0;//使用的全部长度
    unsigned int datapos=0;
    if(startpos==-1)
    {
        feildpos+=1;
        CHECKBYTE oneCHECKBYTE;
        oneCHECKBYTE.type=1;
        oneCHECKBYTE.bytelen=0;
        feildarr.push_back(oneCHECKBYTE);
    }
    else if(startpos>=1)
    {
            datapos+=startpos;
            usehollen+=startpos;
    }
   
    
    while(feildpos<feildsize)
    {
        if(datapos>=datalen)
        {
            ret=false;  
            break; 
        }
        if(usehollen>datalen)
        {
            ret=false;  
            break; 
        }

        char* p=data+datapos;
        unsigned int  retlen=0;
        int count=0;
        bool retflg=converVariableA( p,(datalen-datapos),retlen,count);
        if(!retflg)
        {
            ret=false;  
            break; 
        }
        int feildtype;
        unsigned int vlen=gettypelen(retlen,feildtype);
        
        CHECKBYTE oneCHECKBYTE;
        oneCHECKBYTE.type=feildtype;
        oneCHECKBYTE.bytelen=vlen;//数据长度
        oneCHECKBYTE.typevalue=retlen;
        feildarr.push_back(oneCHECKBYTE);
        usehollen+=vlen;
        usehollen+=count;

        datapos+=count;
        feildpos++;

    }
    if(!ret)
    {
        return false;
    }
    
    
    for(int n=0;n<feildarr.size();n++)
    {
        if(startpos==-1&&n==0)
        {
            continue;
        }
        char* p=data+datapos;
        //类型
        int type=feildarr.at(n).type;
        if(type==1)//INT
        {
            if(feildarr.at(n).typevalue>=1&&feildarr.at(n).typevalue<=6)
            {
                feildarr.at(n).intvalue=converttoint((unsigned char*) p, feildarr.at(n).bytelen);
             //   printf("feildarr.at(n).intvalue=%d\r\n",feildarr.at(n).intvalue);
            }
           
            else if(feildarr.at(n).typevalue==8)
            {
                 feildarr.at(n).intvalue=0;
            }
            else if(feildarr.at(n).typevalue==9)
            {
                 feildarr.at(n).intvalue=1;
            }
            
        }
        if(type==2)
        {
            if(feildarr.at(n).typevalue==7)//double
            {
                 feildarr.at(n).doublevalue=*(double*)p;
                 feildarr.at(n).doublevalue=dbtohl( feildarr.at(n).doublevalue);
                // printf("feildarr.at(n).doublevalue=%f\r\n",feildarr.at(n).doublevalue);
            }
    
        }
        
        if(type==1)
        {
            char tmpbuf[64]={0};
            int ivlen=sprintf(tmpbuf,"%lld", feildarr.at(n).intvalue);
            //feildarr.at(n).bytelen=ivlen;
             feildarr.at(n).valuelen=ivlen;
            feildarr.at(n).value=new char[ivlen+1];
            memcpy(feildarr.at(n).value,tmpbuf,ivlen);
            feildarr.at(n).value[ivlen]=0;
             
        }
        if(type==2)//double
        {
             char tmpbuf[64]={0};
            int ivlen=sprintf(tmpbuf,"%f", feildarr.at(n).doublevalue);
            //feildarr.at(n).bytelen=ivlen;
             feildarr.at(n).valuelen=ivlen;
            feildarr.at(n).value=new char[ivlen+1];
            memcpy(feildarr.at(n).value,tmpbuf,ivlen);
            feildarr.at(n).value[ivlen]=0;
             
        }
        if(type==3||type==4)
        {
            if(feildarr.at(n).bytelen>0)
            {
                int valuelen=feildarr.at(n).bytelen;
                feildarr.at(n).value=new char[valuelen+1];
                memcpy(feildarr.at(n).value,p,valuelen);
                feildarr.at(n).value[valuelen]=0;
                feildarr.at(n).valuelen=valuelen;
            }
        }
        datapos+= feildarr.at(n).bytelen;
    }
    
    return ret;
    
}


//获取第一个字段的信息
bool cellanalyer::getfirstfield(vector<CHECKFILED> filedarr)
{
    if(m_ischeckfirst)
    {
        return true;
    }

    m_ischeckfirst=true;
    bool retflg=false;
    for(int n=0;n<filedarr.size();n++)
    {
        
        if(filedarr.at(n).index==0)
        {
            retflg=true;
            m_firstfeildtype=filedarr.at(n).type;
            m_firstlen=filedarr.at(n).bytelen;
            break;
        }
        
    }
    return retflg;
    
}
