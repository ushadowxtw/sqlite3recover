
#include "tableanalyer.h"

tableanalyer::tableanalyer()
{
    m_lineindex=0;
}


tableanalyer::~tableanalyer()
{

   map<unsigned int,pageanalyer*>::iterator itt;
   for(itt= m_pagemap.begin();itt!=m_pagemap.end();itt++)
   {
        delete   itt->second;
   }
}
//插入表信息
bool tableanalyer::inserttable(char* dbdata,unsigned int dbdatalen,DATABASEHEADER &dbhead,CREATTABLEINFO &oneCREATTABLEINFO)
{
   //// printf("deal:table %s getinfo success\r\n",oneCREATTABLEINFO.tablename.c_str());
   // printf("deal:fieldnum %d \r\n",oneCREATTABLEINFO.fieldnum);
   // printf("deal:rootpage %d\r\n",oneCREATTABLEINFO.rootpage);

    unsigned int pos=0;
    if(oneCREATTABLEINFO.rootpage==0)
    {
        pos+=100;
    }
    else
    {
        pos=oneCREATTABLEINFO.rootpage* dbhead.pagesize ;
    }
    if(pos+dbhead.pagesize>dbdatalen)//检查根页是否完整
    {
        return false;
    }
  //  printf("pagedata pos=%d\r\n",pos);
    char* pagedata=dbdata+pos;
    
    analyonepageapi(dbhead,dbdatalen,oneCREATTABLEINFO.rootpage,dbhead.pagesize,pagedata, dbdata);

    return true;
}
//处理一个页
bool tableanalyer::analyonepageapi(DATABASEHEADER dbhead,unsigned int dbdatalen,unsigned int pageid,unsigned int pagesize,char* pagedata,char* dbdata)
{
    pageanalyer* onepageanalyer=new pageanalyer;
    onepageanalyer->analyonepage( pagedata,pagesize );
    if(onepageanalyer->m_pagetype==1&&onepageanalyer->m_freeblocakcellarr.size()>0)
    {
        m_pagemap.insert(make_pair(pageid,onepageanalyer));
    }
    else if(onepageanalyer->m_pagetype==2)
    {
        for(int n=0;n<onepageanalyer->m_sidepagearr.size();n++)
        {
            INSIDECELL oneINSIDECELL=onepageanalyer->m_sidepagearr.at(n);
            unsigned int childpageid=oneINSIDECELL.childpageid;
            int pos=0;
            if(childpageid==0)
            {
                pos=100;
            }
            else
            {
                pos+=childpageid*dbhead.pagesize;
            }
            
            if(pos+dbhead.pagesize>dbdatalen)
            {
                continue;
            }
            char* childpagedata=dbdata+pos;

            analyonepageapi(dbhead,dbdatalen,childpageid,dbhead.pagesize,childpagedata,dbdata);

        }
        delete onepageanalyer;
    }
    else{
        //B－tree 索引
        delete onepageanalyer;
    }
    return true;
    
}
//解析
bool tableanalyer::prasingpage(fieldstruct* onefieldstruct,vector<CHECKFILED> filedarr,CREATTABLEINFO oneCREATTABLEINFO)
{
    map<unsigned int,pageanalyer*>::iterator itt;
    for(itt=m_pagemap.begin();itt!=m_pagemap.end();itt++)
    {
        itt->second->prasingpagecell(m_lineindex, onefieldstruct,filedarr,oneCREATTABLEINFO);
    }
    return true;
}


