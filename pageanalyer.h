
#ifndef PAGEANALYER_H
#define PAGEANALYER_H
#include "basetype.h"
/**
页解析
*/
class pageanalyer:public baseType{
public:
    pageanalyer();

    ~pageanalyer();

//分析页
    int analyonepage(char* page,int pagesize);


//public
    unsigned int m_cellcount;
    unsigned int m_freecellcount; 
//B+TREE内部页 0x05 =2 
    vector<INSIDECELL> m_sidepagearr;
//B+tree 叶子页0x0D =1
    struct freebtreecell{
        int type;//0-正常  1－含有溢出页
        unsigned int cellbuflen;
        char* cellbuf;
        freebtreecell()
        {
            type=0;
            cellbuf=NULL;     
        }
    };
    vector<freebtreecell> m_freeblocakcellarr;

public:
   //B+tree 叶子页0x0D =1内部页 0x05 =2
    //B-tree 叶子页0x0A =3内部页 0x02 =4
    int m_pagetype;//

public:
    void getpagetype();

    //void dealbtree_leaf();
    void dealbtree();
    void dealbtree_leaf(unsigned short freeaddr);
    void dealb_tree_leaf();
    void dealb_tree();    

    //
    void prasingpagecell(int &line,fieldstruct* onefieldstruct,vector<CHECKFILED> filedarr,CREATTABLEINFO oneCREATTABLEINFO);
private:
    PAGEHEADER *m_paghead;
    unsigned int m_pos;//当前指针位置
 

    unsigned int m_lastrightchild;
    
    char* m_pagecontent;//页内容
    unsigned int m_pagesize;//页大小
    
     

};

#endif
