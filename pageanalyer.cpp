
#include "pageanalyer.h"
#include "cellanalyer.h"
pageanalyer::pageanalyer()
{
    m_pagecontent=NULL;
    m_paghead=NULL;
    m_pagetype=0;
    m_pos=0;    
    m_lastrightchild=0;
    m_cellcount=0;
}


pageanalyer::~pageanalyer()
{
 //   if(m_paghead!=NULL)
   //     delete m_paghead;
    //
    if(m_pagetype==1)
    {
        for(int n=0;n<m_freeblocakcellarr.size();n++)
        {
            if(m_freeblocakcellarr.at(n).cellbuf)
            {
                delete [] m_freeblocakcellarr.at(n).cellbuf;
            }
        }
    }
}

//分析一个页
int pageanalyer::analyonepage(char* page,int pagesize)
{
//    char pagetype=
    if(pagesize<=sizeof(PAGEHEADER))
    {
        return -1;
    }

    m_paghead = (PAGEHEADER*)page;
    
   
    
    m_pos+=sizeof(PAGEHEADER);
    /**获取页类型*/
    getpagetype();
    /***/
    if(m_pagetype==0)
    {
        /**不标准的页类型*/
        return -1;
    }

    /**第一个自由快地址*/
    m_paghead->freecelladdr= ntohs(m_paghead->freecelladdr);
    /**本页的单元数量（不包含最右儿子页）*/
    m_paghead->cellcount= ntohs(m_paghead->cellcount);
    /**单元内容区的起始位置*/
    m_paghead->celladdr= ntohs(m_paghead->celladdr);
    /**碎片的字节数*/
    //m_paghead->piececount
    if(m_pagetype==2||m_pagetype==4)//内部页
    {
        //含有最右儿子页（ID）
        memcpy(&m_lastrightchild, page+m_pos, 4);
        m_lastrightchild = ntohl(m_lastrightchild);
        m_pos+=4;
    }

    m_pagecontent=page;//页内容
    m_pagesize=pagesize;//页大小
    
    m_cellcount=m_paghead->cellcount;

    dealbtree();
    if(m_pagetype==1)
    {
        dealbtree_leaf(m_paghead->freecelladdr);
    }
    return 0;
}
//获取页类型
// ret -10- 失败 
//B+tree 叶子页0x0D =1内部页 0x05 =2
//B-tree 叶子页0x0A =3内部页 0x02 =4
void pageanalyer::getpagetype()
{
// printf("deal:[pageanalyer::analyonepage]0x%02x\r\n",m_paghead->type);
    switch (m_paghead->type)
    {
        case 0x0D:
        //B+tree 叶子页0x0D =1
            m_pagetype=1;
            break;
        case 0x05:
         //B+tree 内部页 0x05 =2   
            m_pagetype=2;
            break;
        case 0x0A:
          //B-tree 叶子页0x0A =3
            m_pagetype=3;
            break;
        case 0x02:
            //B-tree 内部页 0x02 =4
            m_pagetype=4;
            break;
        default:
            break;
    }

}
//B+TREE内部页 0x05 =2
void pageanalyer::dealbtree()
{
    if(m_pagetype!=2)
    {
        return;
    }
    //获取单元的地址列表
    
    if(m_paghead->cellcount==0)
    {
  
        return;
    }
    
    vector< unsigned int> cellarr;
    //读取单元地址列表
    /**自高地址到底地址增长**/
    unsigned int startaddr=m_pos+(m_paghead->cellcount-1)*2;//起始地址
    if(m_pagesize<startaddr)
    {
        return;
    }
    
    for(int n=0;n<m_paghead->cellcount;n++)
    {
        //unsigned short	celladdr=0;
       // char tmpcell[8]={0};
       // memcpy(tmpcell,m_pagecontent+(startaddr-n*2),2);
       // celladdr=( unsigned short)tmpcell;
        
       unsigned short	celladdr= getShort(m_pagecontent+(startaddr-n*2)) ;


     //   celladdr= ntohs(celladdr);
        
        cellarr.push_back(celladdr);
        
    }
    //处理各个单元
    for(int m=0;m<cellarr.size();m++)
    {
        //unsigned int childpageid;
        //memcpy((void*)childpageid,m_pagecontent+cellarr.at(m),4);
      //  printf("%u\r\n",cellarr.at(m));
        int64_t ii=converttoint((unsigned char*)(m_pagecontent+cellarr.at(m)), 4);
        unsigned int childpageid=ii;
     //   childpageid = ntohl(childpageid);
       //  printf("childpageid=%u\r\n",childpageid);
        int retlen=0;
        int count=0;
        int cellindexpos=cellarr.at(m)+4;
        char *p=m_pagecontent+cellindexpos;
        bool retflg=converVariable(p,retlen,count);
        if(!retflg)
        {
            continue;
        }
        INSIDECELL oneINSIDECELL;
        oneINSIDECELL.childpageid=(childpageid-1);
        oneINSIDECELL.index=retlen;
        m_sidepagearr.push_back(oneINSIDECELL);
    }
}
//处理
//B+tree 叶子页0x0D =1
void pageanalyer::dealbtree_leaf(unsigned short freeaddr)
{
    //
    if(m_pagetype!=1)
    {
        return;
    }
    //对自由块处理
    if( freeaddr<=0)
    {
        return;
    }

    FREEBLOCKCELL *freeblockcellhead = (FREEBLOCKCELL*)(m_pagecontent+freeaddr);
    
    
    freeblockcellhead->nextcelladdr= ntohs(freeblockcellhead->nextcelladdr);
    freeblockcellhead->celllen= ntohs(freeblockcellhead->celllen);

    freebtreecell onefreebtreecell;
    onefreebtreecell.cellbuflen=freeblockcellhead->celllen-2;
    onefreebtreecell.cellbuf=new char[onefreebtreecell.cellbuflen+4];
    int copylen=onefreebtreecell.cellbuflen;//
    /**含有溢出页**/
    if(((freeaddr+4)+onefreebtreecell.cellbuflen)>m_pagesize)//含有溢出页
    {
        //读取溢出页 暂时不做处理
        onefreebtreecell.type=1;
        
        copylen=m_pagesize-(freeaddr+4);
    }
    memcpy(onefreebtreecell.cellbuf,m_pagecontent+(freeaddr+4),onefreebtreecell.cellbuflen);
    
    m_freeblocakcellarr.push_back(onefreebtreecell);
    
    if(freeblockcellhead->nextcelladdr>0)
        dealbtree_leaf(freeblockcellhead->nextcelladdr);
    
}
//B-tree 内部页 0x02 =4 (索引)
void pageanalyer::dealb_tree()
{
    //根页
    return;

}
//B-tree 叶子页0x0A =3（索引）
void pageanalyer::dealb_tree_leaf()
{
    //实际情况在索引列删除 自由块为 \x00 00 00 00……
    return;
    
}

//解析自由块
void pageanalyer::prasingpagecell(int &line,fieldstruct* onefieldstruct,vector<CHECKFILED> filedarr,CREATTABLEINFO oneCREATTABLEINFO)
{
    for(int n=0;n<m_freeblocakcellarr.size();n++)
    {
//        m_freeblocakcellarr.at(n);
        cellanalyer onecellanalyer;
        onecellanalyer.prasingcell(line, onefieldstruct,filedarr,oneCREATTABLEINFO,m_freeblocakcellarr.at(n).cellbuf,m_freeblocakcellarr.at(n).cellbuflen);
    }


}







