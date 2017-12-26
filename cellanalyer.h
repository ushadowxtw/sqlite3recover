
#ifndef CELLANALYER_H
#define CELLANALYER_H
#include "basetype.h"
/**
单元解析
*/
class cellanalyer:public baseType{
public:
    cellanalyer();

    ~cellanalyer();
public:
    void prasingcell(int &line,fieldstruct* onefieldstruct,vector<CHECKFILED> filedarr,CREATTABLEINFO oneCREATTABLEINFO,char* celldata,unsigned int celllen);
    bool getfirstfield(vector<CHECKFILED> filedarr);
    bool parefeild(vector<CHECKFILED> filedarr,vector<CHECKBYTE> feildarr);
    bool operationbyte(int startpos,char* data,unsigned int datalen,int feildsize,vector<CHECKBYTE>& feildarr,int &usehollen);
private:
    bool m_ischeckfirst;
    int m_firstfeildtype;
    int m_firstlen;
};

#endif
