// 
#ifndef BASETYPE_H
#define BASETYPE_H
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/statfs.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string>
#include <vector>
#include <map>
using namespace std;
#define u8 uint8_t

#define SQLITE_UTF8           1    /* IMP: R-37514-35566 */
#define SQLITE_UTF16LE        2    /* IMP: R-03371-37637 */
#define SQLITE_UTF16BE        3    /* IMP: R-51971-34154 */
#define SQLITE_UTF16          4    /* Use native byte order */
#define SQLITE_ANY            5    /* Deprecated */
#define SQLITE_UTF16_ALIGNED  8    /* sqlite3_create_collation only */
#pragma pack(push)
#pragma pack(1)
struct DD_AQPI{
   double p;
};
//库 头
/*
** The first page is always a btree page.  The first 100 bytes of the first
** page contain a special header (the "file header") that describes the file.
** The format of the file header is as follows:
**
**   OFFSET   SIZE    DESCRIPTION
**      0      16     Header string: "SQLite format 3\000"
**     16       2     Page size in bytes.  (1 means 65536)
**     18       1     File format write version
**     19       1     File format read version
**     20       1     Bytes of unused space at the end of each page
**     21       1     Max embedded payload fraction (must be 64)
**     22       1     Min embedded payload fraction (must be 32)
**     23       1     Min leaf payload fraction (must be 32)
**     24       4     File change counter
**     28       4     Reserved for future use
**     32       4     First freelist page
**     36       4     Number of freelist pages in the file
**     40      60     15 4-byte meta values passed to higher layers
**
**     40       4     Schema cookie
**     44       4     File format of schema layer
**     48       4     Size of page cache
**     52       4     Largest root-page (auto/incr_vacuum)
**     56       4     1=UTF-8 2=UTF16le 3=UTF16be
**     60       4     User version
**     64       4     Incremental vacuum mode
**     68       4     Application-ID
**     72      20     unused
**     92       4     The version-valid-for number
**     96       4     SQLITE_VERSION_NUMBER
*/



struct DATABASEHEADER
{
        unsigned char		magic[0x10];
	unsigned short		pagesize;		//byte
	unsigned char		writeversion;	//must be 1
	unsigned char		readeversion;	//must be 1
	unsigned char		bytesofunused;	//may be 0
	unsigned char		maxpercentpercell;	//must be 64
	unsigned char		minpercentpercell;	//must be 32
	unsigned char		minpercentperleafcell;	//must be 32
	unsigned int		changecount;
	unsigned int		totalpages;
	unsigned int		firstfreepageid;
	unsigned int		freepagecount;
	unsigned char		schemacookie[4];
	unsigned int		schemaformatnumber;
	unsigned int		defaultpagecachesize;
	unsigned int		largestrootpageid;
	unsigned int		textencode;	//1 UTF8 ; 2 UTF16 LE ; 3 UTF16 BE (在此只考虑 UTF8)
	unsigned int		userversion;
	unsigned int		ncrementalvacuummod;//Incremental vacuum mode ==1 
	unsigned int		applicationid;
	unsigned char		unknown[20];
	unsigned int		versionvalidfornumber;
	unsigned int		sqliteversion;
};

//页头
struct PAGEHEADER
{
    unsigned char		type;	//1: intkey, 2: zerodata, 4: leafdata, 8: leaf
    unsigned short		freecelladdr;
    unsigned short		cellcount;
    unsigned short		celladdr;
    unsigned char		piececount;
//  unsigned int		rightchildpageid;	//if leaf==false
};

//内部页单元
struct INSIDECELL
{
    unsigned int childpageid;
    int index;
};
//自由块单元头
struct FREEBLOCKCELL
{

    unsigned short	nextcelladdr;
    unsigned short	celllen;//(包含数据与此2字节)
};

/**
基本类
*/

/*
** The following table describes the various storage classes for data:
**
**   serial type        bytes of data      type
**   --------------     ---------------    ---------------
**      0                     0            NULL
**      1                     1            signed integer
**      2                     2            signed integer
**      3                     3            signed integer
**      4                     4            signed integer
**      5                     6            signed integer
**      6                     8            signed integer
**      7                     8            IEEE float TIMESTAMP 
**      8                     0            Integer constant 0
**      9                     0            Integer constant 1
**     10,11                               reserved for expansion
**    N>=12 and even       (N-12)/2        BLOB
**    N>=13 and odd        (N-13)/2        text


 列亲和性测定

列的亲和性是由它的声明类型决定的，按照以下顺序所示的规则：

1. 如果声明类型包含字符串“INT”，那它被指定为整型亲和性；

2. 如果列的声明类型包含任何“CHAR”、“CLOB”或“TEXT”字符串，那么该列具有文本亲和性。注意：VARCHAR类型包含“CHAR”并且被指定为文本亲和性；

3. 如果列的声明类型包含“BLOB”或者没有指定类型，那这列具有NONE亲和性；

4. 如果列的声明类型包含任何“REAL”、“FLOA”或“DOUB”字符串，则该列具有实数亲和性；

5. 否则，它将具有数值亲和性。

注意：判定列亲和性规则的顺序是很重要的。一个具有“CHARINT”声明类型的列将匹配规则1和2，但是规则1优先所有该列具有整型亲和性。
**/


//表字段项
struct CREATTABLEINFO
{
    string tablename;//表名字
    int tabletype;//表类型- 0-table
    int fieldnum;//字段数
    
    string  primarykey;//主键
    int  primarykeytype;//主键类型
    
    string creatsql;
    int rootpage; 
    
    vector<string> fieldarr;//字段
    vector<int> fieldtype;//类型 
    
    
    
};
/**
**/
//字段检测
struct CHECKFILED{
    string key;//字段名

    int checktype;// 检测状态 -1－不检测此字段值   1－检测此字段长度 2－检测此字段大小  3－大小和长度同时检测  
    int type;//类型 SQLITE_INTEGER ＝ 1   SQLITE_FLOAT ＝2  SQLITE_TEXT （VARCHAR）＝3  SQLITE_BLOB＝4 SQLITE_NULL ＝5 6-时间（TIMESTAMP）
    int bytelen;//所占位数 只在第一个字段中使用(INT索引)

    // checktype＝＝1 ｜3
    int minlen;//最少长度
    int maxlen;//最大长度

    //checktype＝＝2 ｜3
    int64_t min;
    int64_t max;

    /**留空字段**/
    int index;//位置 留空 从0开始
    CHECKFILED()
    {
        checktype=-1;
        minlen=maxlen=0;
        min=max=0;
        bytelen=-1;
    };
    
};
//
struct CHECKBYTE{
    int type;//类型 -1 未知 
    int bytelen;//长度（占字节）
    unsigned int typevalue;//标识类型的原值－－－－》长度 类型 
    char* value;
    unsigned int valuelen;
    
    int64_t intvalue;
    double doublevalue;//
    CHECKBYTE()
    {
        doublevalue=0.0;
        intvalue=0;
        type=-1;
        value=NULL;
        valuelen=0;
    };
};

struct FIELDBLOCK{
    char* value;
//    string key;
    unsigned int valuelen;
    FIELDBLOCK()
    {
        value=NULL;
        valuelen=0;
    }
};
#define FIELDMAP map<string,FIELDBLOCK>

#pragma pack(pop)
class fieldstruct{
public:
    fieldstruct()
    {};
    ~fieldstruct()
    {
       map<int,FIELDMAP>::iterator itt;
       for(itt=m_fieldmap.begin();itt!=m_fieldmap.end();)
       {
            FIELDMAP::iterator itr;
            for(itr=itt->second.begin();itr!=itt->second.end();)
            {
                if(itr->second.value)
                {
                    delete [] itr->second.value;
                }
                itt->second.erase(itr++);
            }
            m_fieldmap.erase(itt++);
       }
         
    };
    
public:
    //插入
    void insert(int line,string key,char* value,unsigned int vlen)
    {
        map<int,FIELDMAP>::iterator itt;
        itt=m_fieldmap.find(line);
        if(itt==m_fieldmap.end())
        {
            FIELDMAP oneFIELDMAP;
            FIELDBLOCK oneFIELDBLOCK;
            if(vlen>0)
            {
                oneFIELDBLOCK.value=new char[vlen+1];
                memcpy(oneFIELDBLOCK.value,value,vlen);
                oneFIELDBLOCK.value[vlen]=0;
                oneFIELDBLOCK.valuelen=vlen;
            }
            oneFIELDMAP.insert(make_pair(key,oneFIELDBLOCK));
            
            m_fieldmap.insert(make_pair(line,oneFIELDMAP));
        }
        else
        {

            FIELDMAP::iterator itr;
            itr=itt->second.find(key);
            if(itr ==itt->second.end())
            {
                FIELDBLOCK oneFIELDBLOCK;
                if(vlen>0)
                {
                    oneFIELDBLOCK.value=new char[vlen+1];
                    memcpy(oneFIELDBLOCK.value,value,vlen);
                    oneFIELDBLOCK.value[vlen]=0;
                    oneFIELDBLOCK.valuelen=vlen;
                }
                itt->second.insert(make_pair(key,oneFIELDBLOCK));
            }
            else
            {
                if(itr->second.value)
                {
                    delete [] itr->second.value;
                }
                itr->second.value=new char[vlen+1];
                memcpy(itr->second.value,value,vlen);
                itr->second.valuelen=vlen;
            }
        }
    };
    //获取数量
    int getsize()
    {
        return m_fieldmap.size();
    };
    //获取
    bool get(int line,string key,char* buf,unsigned int buflen)
    {
        bool retflg=false;
        map<int,FIELDMAP>::iterator itt;
        itt=m_fieldmap.find(line);
        if(itt!=m_fieldmap.end())
        {
            FIELDMAP::iterator itr;
            itr=itt->second.find(key);
            if(itr !=itt->second.end())
            {
                retflg=true;
               
                if(itr->second.value)
                {
                    buflen=itr->second.valuelen;
                    buf=new char[buflen+1];
                    memcpy(buf,itr->second.value,buflen);
                    buf[buflen]=0;
                }
                else
                {
                    buf=NULL;
                    buflen=0;
                }
                
            }
            
        }
        return retflg;
    };
    bool get(int line,string key,string & bufstr)
    {
        bool retflg=false;
        map<int,FIELDMAP>::iterator itt;
        itt=m_fieldmap.find(line);
        if(itt!=m_fieldmap.end())
        {
            FIELDMAP::iterator itr;
            itr=itt->second.find(key);
            if(itr !=itt->second.end())
            {
                retflg=true;
               
                if(itr->second.value)
                {
                    itr->second.value[itr->second.valuelen]=0;
                    bufstr=string(itr->second.value);
                }
                else
                {
                    bufstr="";
                }
                
            }
            
        }
        return retflg;

    }
private:
    map<int,FIELDMAP> m_fieldmap;
};

class baseType{
public:
    baseType();

    ~baseType();
public:
    //可变整数
    bool converVariable(char* p,int & retlen,int &count);
    bool converVariableA(char* p,int plen,unsigned int & retlen,int &count);
    int64_t converttoint(unsigned char* p, unsigned int len);
    double dbtohl(double x);
    int sqlite3AtoF(const char *z, double *pResult, int length, u8 enc=SQLITE_UTF8);
    void SpliteStoV(char* buf,int len,char* e,std::vector<string>& sVector,bool isdeleempty=false);
    unsigned int gettypelen(unsigned int type,int& feildtype);
    //获取字段位置 0.1.2.3....
    int getfeildindex(vector<string> feildsVector,string key);
    
    unsigned short getShort(char* buf, bool asc=false, int len=2) ;
};

#endif
