


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include "basetype.h"
#include "dbrecover.h"
/**
sqlite3 数据库恢复
*/
using namespace std;
int iphonesms();
 int iphoneaddreeebook();
int iphonehistory();



int main(int argc, char *argv[])
{
iphoneaddreeebook();
iphonesms();
iphonehistory();
  while(1)
    {
        sleep(60);
    }
}
int iphonehistory()
{
    string dbfilename="./CallHistory.db";
    
    /**test**/
    dbrecover onedbrecover;
    onedbrecover.insertDB(dbfilename.c_str());

    vector<CHECKFILED> filedarr;
    CHECKFILED oneROWID;
    oneROWID.key="Z_PK";
    oneROWID.type=1;
    oneROWID.checktype=-1;
    oneROWID.bytelen=1;
    filedarr.push_back(oneROWID );

    
    CHECKFILED oneZDATE;
    oneZDATE.key="ZDATE";
    oneZDATE.type=2;
    oneZDATE.checktype=-1;
    
    filedarr.push_back(oneZDATE );

    

    CHECKFILED oneZDURATION;
    oneZDURATION.key="ZDURATION";
    oneZDURATION.type=2;
    oneZDURATION.checktype=-1;
    
    filedarr.push_back(oneZDURATION );


    CHECKFILED oneZISO_COUNTRY_CODE;
    oneZISO_COUNTRY_CODE.key="ZISO_COUNTRY_CODE";
    oneZISO_COUNTRY_CODE.type=3;
    oneZISO_COUNTRY_CODE.checktype=1;
    oneZISO_COUNTRY_CODE.maxlen=5;
    oneZISO_COUNTRY_CODE.minlen=2;

    filedarr.push_back(oneZISO_COUNTRY_CODE );


    

     CHECKFILED oneZADDRESS;
    oneZADDRESS.key="ZADDRESS";
    oneZADDRESS.type=4;
    oneZADDRESS.checktype=1;
    oneZADDRESS.maxlen=20;
    oneZADDRESS.minlen=2;

    filedarr.push_back(oneZADDRESS );


     CHECKFILED oneZUNIQUE_ID;
    oneZUNIQUE_ID.key="ZUNIQUE_ID";
    oneZUNIQUE_ID.type=3;
    oneZUNIQUE_ID.checktype=1;
    oneZUNIQUE_ID.maxlen=40;
    oneZUNIQUE_ID.minlen=30;

    filedarr.push_back(oneZUNIQUE_ID );

    
    
    CHECKFILED oneZANSWERED;
    oneZANSWERED.key="ZANSWERED";
    oneZANSWERED.type=1;
    oneZANSWERED.checktype=-1;

    filedarr.push_back(oneZANSWERED );
    

    fieldstruct* onefieldstruct=new fieldstruct;
    int linenum=onedbrecover.getdeltabledata("ZCALLRECORD", filedarr,onefieldstruct);
    printf("linenum=%d\r\n",linenum);
    int line=1;
    string  bufstr;
    while(onefieldstruct->get( line,"ZADDRESS", bufstr))
    {
            printf("ZADDRESS=%s\r\n",bufstr.c_str());
    
            onefieldstruct->get( line,"ZANSWERED", bufstr);
            printf("ZANSWERED=%s\r\n",bufstr.c_str());

            onefieldstruct->get( line,"ZDURATION", bufstr);
            printf("ZDURATION=%s\r\n",bufstr.c_str());

            onefieldstruct->get( line,"ZDATE", bufstr);
            printf("ZDATE=%s\r\n",bufstr.c_str());
            line++;
    }    
    delete  onefieldstruct;

}
int iphonesms()
{
 string dbfilename="./sms.db";
    
    /**test**/
    dbrecover onedbrecover;
    onedbrecover.insertDB(dbfilename.c_str());
    
    vector<CHECKFILED> filedarr;
    CHECKFILED oneROWID;
    oneROWID.key="ROWID";
    oneROWID.type=1;
    oneROWID.checktype=-1;
    oneROWID.bytelen=1;
    filedarr.push_back(oneROWID );

      CHECKFILED onevalueguid;
    onevalueguid.key="guid";
    onevalueguid.type=3;
    onevalueguid.minlen=36;
    onevalueguid.maxlen=40;
    onevalueguid.checktype=1;

    filedarr.push_back(onevalueguid );

    
    CHECKFILED onetext;
    onetext.key="text";
    onetext.type=3;
    onetext.minlen=1;
    onetext.maxlen=4096;
    onetext.checktype=1;

    filedarr.push_back(onetext );

    CHECKFILED onereplace;
    onereplace.key="replace";
    onereplace.type=1;
    onereplace.checktype=-1;

    filedarr.push_back(onereplace );


    CHECKFILED onedate;
    onedate.key="date";
    onedate.type=1;
    onedate.min=1000000000;
    onedate.max=900000000000000000;
    onedate.checktype=2;

    filedarr.push_back(onedate );

    CHECKFILED oneis_sent;
    oneis_sent.key="is_sent";
    oneis_sent.type=1;
    oneis_sent.min=1000000000;

    oneis_sent.checktype=-1;

    filedarr.push_back(oneis_sent );




    fieldstruct* onefieldstruct=new fieldstruct;
    int linenum=onedbrecover.getdeltabledata("message", filedarr,onefieldstruct);
    printf("linenum=%d\r\n",linenum);
    int line=1;
    string  bufstr;
    while(onefieldstruct->get( line,"guid", bufstr))
    {
            printf("guid=%s\r\n",bufstr.c_str());
    
            onefieldstruct->get( line,"date", bufstr);
            printf("date=%s\r\n",bufstr.c_str());

            onefieldstruct->get( line,"text", bufstr);
            printf("text=%s\r\n",bufstr.c_str());
            line++;
    }    



    delete  onefieldstruct;


}
int iphoneaddreeebook()
{
    cout << "Hello, world!" << endl;
    string dbfilename="./AddressBook.db";
    
    /**test**/
    dbrecover onedbrecover;
    onedbrecover.insertDB(dbfilename.c_str());
    vector<CHECKFILED> filedarr;
    CHECKFILED onerecord_id;
    onerecord_id.key="record_id";
    onerecord_id.type=1;
    onerecord_id.checktype=-1;
    
    filedarr.push_back(onerecord_id );

    CHECKFILED oneUID;
    oneUID.key="UID";
    oneUID.type=1;
    oneUID.checktype=-1;
    oneUID.bytelen=1;
    filedarr.push_back(oneUID );

    
    CHECKFILED oneproperty;
    oneproperty.key="property";
    oneproperty.type=1;
    oneproperty.checktype=-1;

    filedarr.push_back(oneproperty );
    
    CHECKFILED onevalue;
    onevalue.key="value";
    onevalue.type=3;
    onevalue.minlen=2;
    onevalue.maxlen=20;
    onevalue.checktype=1;

    filedarr.push_back(onevalue );

    CHECKFILED onevalueguid;
    onevalueguid.key="guid";
    onevalueguid.type=3;
    onevalueguid.minlen=36;
    onevalueguid.maxlen=40;
    onevalueguid.checktype=1;

    filedarr.push_back(onevalueguid );

    fieldstruct* onefieldstruct=new fieldstruct;
    int linenum=onedbrecover.getdeltabledata("ABMultiValue", filedarr,onefieldstruct);
    printf("linenum=%d\r\n",linenum);
    int line=1;
    string  bufstr;
    while(onefieldstruct->get( line,"record_id", bufstr))
    {
            printf("record_id=%s\r\n",bufstr.c_str());
    
            onefieldstruct->get( line,"value", bufstr);
            printf("value=%s\r\n",bufstr.c_str());
            line++;
    }    



    delete  onefieldstruct;
  
  return EXIT_SUCCESS;
}
