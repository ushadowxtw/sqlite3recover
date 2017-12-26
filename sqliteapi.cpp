
#include "sqliteapi.h"

sqliteapi::sqliteapi()
{
    m_sqlpd=NULL;
    m_load=false;
}


sqliteapi::~sqliteapi()
{
    if(m_sqlpd)
        sqlite3_close( m_sqlpd );
}
//搜索
bool sqliteapi::LoadDBFile(const char* dbfilename)
{
    if(access(dbfilename,0)!=0)
    {
        return false;
    }

    int res = sqlite3_open(dbfilename,&m_sqlpd);
    if( res != SQLITE_OK )
    { 
  
         return false;
    }
    else
    {
        m_load=true;
        return true;

    }
}
//执行语句
 //执行指令 除却查询
bool sqliteapi::SqlExe(const char*sqlcmd)
{
    if(!m_load)
    {
        return false;
    }
    char* errMsg; 
    
    int res = sqlite3_exec(m_sqlpd,sqlcmd,0,0,&errMsg);

    if(res != SQLITE_OK)
    {
        return false;
    }
    return true;
}

int sqliteapi::SqlQuery(const char* sqlcmd,string & str, const char *  splite1, char splite2)
{
    //查询数据
	if(!m_load)
	{
            return -1;
	}
	char ** dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;//行   列
	int i , j;        //遍历基数
	int index;        //列
        char* errMsg; 
	int res = sqlite3_get_table( m_sqlpd, sqlcmd, &dbResult, &nRow, &nColumn, &errMsg );
	if( SQLITE_OK == res )
	{
		//查询成功
		str.clear();
		index = nColumn; //前面说过 dbResult 前面第一行数据是字段名称，从 nColumn 索引开始才是真正的数据
		for(  i = 0; i < nRow ; i++ )
		{

			for( j = 0 ; j < nColumn; j++ )
			{
 				//printf( "字段名:%s  ?> 字段值:%s\n",  dbResult[j], dbResult [index] );

				if(dbResult [index]!=NULL)
				{
					str+=dbResult [index];
				}

				str+=splite1;
				++index;
			}
			str[str.size()-1]=splite2;

		}
	}
	else
	{
		sqlite3_free_table( dbResult );
		return -1;
	}
	//到这里，不论数据库查询是否成功，都释放 char** 查询结果，使用 sqlite 提供的功能来释放
	sqlite3_free_table( dbResult );

	int len=str.size();
	if(len>=1)
	{
		str[len-1]=0;
		str.resize(len-1,' ');
		
	}
	return nRow;
}
//获取列名  
bool  sqliteapi::GetColName(vector<string>& vecColName, string strTableName)  
{  
    sqlite3_stmt *stmt = NULL;  
    char sql[200];  
    sprintf(sql, "SELECT * FROM %s limit 0,1", strTableName.c_str());  
    char **pRes=NULL;  
    int nRow=0, nCol=0;  
    char *pErr=NULL;  
  
    //第一行是列名称  
    sqlite3_get_table((sqlite3*)m_sqlpd, sql, &pRes, &nRow, &nCol, &pErr);      
    for (int i=0; i<nRow; i++)  
    {  
        for (int j=0; j<nCol; j++)  
        {  
            char *pv = *(pRes+nCol*i+j);  
            vecColName.push_back(pv);  
        }  
        break;  
    }  
  
    if (pErr!=NULL)  
    {  
        sqlite3_free(pErr);  
    }  
    sqlite3_free_table(pRes);  
    return true;  
}  
//获取列类型  
bool  sqliteapi::GetColType(vector<int>& vecType, string strTableName)  
{  
    //if(strTableName=="ABGroup")
   // {
    //    printf("EEE");
   // }
    sqlite3_stmt *stmt = NULL;  
    char sql[200];  
    sprintf(sql, "SELECT * FROM %s limit 0,1", strTableName.c_str());  
    sqlite3_prepare((sqlite3*)m_sqlpd, sql,-1,&stmt,0);     
    if(stmt)   
    {  
        while(sqlite3_step(stmt) == SQLITE_ROW)   
        {     
            int nCount = sqlite3_column_count(stmt);  
//            printf("nCount=%d\r\n",nCount);
            for (int i=0; i<nCount; i++)  
            {  
                int nValue = sqlite3_column_int(stmt,0);  
                int nType = sqlite3_column_type(stmt, i);  
                if(nType!=1&&nType!=2&&nType!=3&&nType!=4&&nType!=5)
                    printf("nType=%d\r\n",nType);
                vecType.push_back(nType);  
                switch (nType)  
                {  
                case 1:  
                    //SQLITE_INTEGER  
                    break;  
                case 2:  
                    //SQLITE_FLOAT  
                    break;  
                case 3:  
                    //SQLITE_TEXT  
                    break;  
                case 4:  
                    //SQLITE_BLOB  
                    break;  
                case 5:  
                    //SQLITE_NULL  
                    break;  
                }  
            }  
            break;  
        }  
        sqlite3_finalize(stmt);  
        stmt = NULL;  
    }  
    return true;  
}

