
#ifndef DBRECOVER_H
#define DBRECOVER_H
#include "basetype.h"
#include "tableanalyer.h"
/**
数据恢复
*/
class dbrecover:public baseType{
public:
    dbrecover();

    ~dbrecover();
public:
    int insertDB(const char*filename);
    int getdeltabledata(string tablename,vector<CHECKFILED> filedarr,fieldstruct* onefieldstruct);
private:
    int  checkdbhead();
    int readtableinfo();
    int analytableinfo();

public:
    DATABASEHEADER m_dbhead;


private:
    string m_dbfile;
    FILE* m_file;
    unsigned int m_dbfilelen;
    char* m_dbbuf;

    map<string,CREATTABLEINFO> m_tablemap;
    
    map<string,tableanalyer*> m_tabledetailmap;
};

#endif
