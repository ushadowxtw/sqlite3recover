
#ifndef TABLEANALYER_H
#define TABLEANALYER_H
#include "basetype.h"
#include "pageanalyer.h"

/**
表解析
*/
class tableanalyer{
public:
    tableanalyer();

    ~tableanalyer();

    bool inserttable(char* dbdata,unsigned int dbdatalen,DATABASEHEADER &dbhead,CREATTABLEINFO &oneCREATTABLEINFO);

    //
    bool prasingpage(fieldstruct* onefieldstruct,vector<CHECKFILED> filedarr,CREATTABLEINFO oneCREATTABLEINFO);
private:
        bool analyonepageapi(DATABASEHEADER dbhead,unsigned int dbdatalen,unsigned int pageid,unsigned int pagesize,char* pagedata,char* dbdata);
public:
    map<unsigned int,pageanalyer*> m_pagemap;
    int m_lineindex;

};

#endif
