
#ifndef SQLITEAPI_H
#define SQLITEAPI_H
#include <sqlite3.h>
#include <string>
#include <vector>
using namespace std;
 
/**
数据库接口
*/
class sqliteapi{
public:
    sqliteapi();

    ~sqliteapi();

public:
        bool LoadDBFile(const char* dbfilename);
        bool SqlExe(const char*sqlcmd);
        int  SqlQuery(const char* sqlcmd,string & str,const char * splite1="\2", char splite2='\1');
        bool  GetColName(vector<string>& vecColName, string strTableName);
        bool  GetColType(vector<int>& vecType, string strTableName)  ;
private:
    sqlite3* m_sqlpd; 
    bool m_load;

};

#endif
