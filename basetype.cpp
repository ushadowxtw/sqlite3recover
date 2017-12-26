
#include "basetype.h"

baseType::baseType()
{
}


baseType::~baseType()
{
}

void baseType::SpliteStoV(char* buf,int len,char* e,std::vector<string>& sVector,bool isdeleempty)
{
	sVector.clear();
	int count=0;
	char* tp=buf;
	int spliteword=strlen(e);
    char* p=(char*)strstr(buf,e);
	while(p!=NULL)
	{
		*p=0;
		int off=p-tp;
		if(!(isdeleempty&&off==0))
		{
			sVector.push_back(tp);
			count++;
		}
		tp=p+spliteword;
		len-=(off+spliteword);
        p=(char*)strstr(tp,e);
	}
	if(len>0)
	{
		sVector.push_back(tp);
		count++;
		return;
	}
	if(! isdeleempty && len ==0 )
	{
		sVector.push_back(tp);
		count++;
	}

	return;
}
//不定长整数
bool baseType::converVariableA(char* p,int plen,unsigned int & retlen,int &count)
{

    bool retflg=true;
    retlen = 0;
    count=0;
    unsigned char c=0;
    do
    {
        if(count>plen)
        {
            retflg=false;
            break;
        }
        c = *p;
        retlen *= 0x80;
        retlen += (c&0x7F);

        p++;

        count++;
  
        if(count>=9)
            break;
    }
    while((c&0x80)!=0);//符号位是否为0

    if(count>9)
    {
        retflg=false;
        retlen = 0;
        count = 0;
    }
    p-=count;//复位指针

    return retflg;


}

bool baseType::converVariable(char* p,int & retlen,int &count)
{
    bool retflg=true;
    retlen = 0;
    count=0;
    unsigned char c=0;
    do
    {
        c = *p;
        retlen *= 0x80;
        retlen += (c&0x7F);

        p++;

        count++;
        if(count>=9)
                break;
    }
    while((c&0x80)!=0);//符号位是否为0
    p-=count;//复位指针
    if(count>9)
    {
        retflg=false;
        retlen = 0;
        count = 0;
    }


    return retflg;
}


unsigned int baseType::gettypelen(unsigned int type,int& feildtype)
{
        feildtype=-1;
	if(type==0)
	{
            return 0;
	}
	else if(type>=1 && type<7)
	{
            feildtype=1;
            if(type==5)
                    return 6;
            if(type==6 )
                    return 8;

            return type;
	}
        else if(type==7)
        {
            feildtype=2;
            return 8;
        }
	else if(type==8)
	{
            feildtype=1;
            return 0;
	}
	else if(type==9)
	{
            feildtype=1;
            return 0;
	}
	else if(type>12 && (type%2)==0)
	{
            feildtype=4;
            return (type-12)/2;
	}
	else if(type>13 && (type%2)!=0)
	{
            feildtype=3;
            return (type-13)/2;
	}
	else
	{
            return 0;
	}
}

unsigned short baseType::getShort(char*buf, bool asc, int len) 
{  
    short r = 0;  
    if (asc)  
    for (int i = len - 1; i >= 0; i--) {  
        r <<= 8;  
        r |= (buf[i] & 0x00ff);  
    }  
    else  
    for (int i = 0; i < len; i++) {  
    r <<= 8;  
        r |= (buf[i] & 0x00ff);  
    }  
    
    return r;  
}  
int64_t baseType::converttoint(unsigned char* p, unsigned int len)
{
        if(len<=0)
		return 0;

	int64_t ret = 0;
       int count=0;
	unsigned char c=0;
	do
	{
		c = *p;
		ret *= 0x100;
		ret += (c);
		
		p++;
		len--;
	    count++;
	}
	while(len>0);
           p-=count;//复位指针
	return ret;
}
#if 'A' == '\301'
# define SQLITE_EBCDIC 1
#else
# define SQLITE_ASCII 1
#endif

#ifdef SQLITE_ASCII
const unsigned char sqlite3CtypeMap[256] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 00..07    ........ */
  0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,  /* 08..0f    ........ */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 10..17    ........ */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 18..1f    ........ */
  0x01, 0x00, 0x80, 0x00, 0x40, 0x00, 0x00, 0x80,  /* 20..27     !"#$%&' */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 28..2f    ()*+,-./ */
  0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,  /* 30..37    01234567 */
  0x0c, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 38..3f    89:;<=>? */

  0x00, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x02,  /* 40..47    @ABCDEFG */
  0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,  /* 48..4f    HIJKLMNO */
  0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,  /* 50..57    PQRSTUVW */
  0x02, 0x02, 0x02, 0x80, 0x00, 0x00, 0x00, 0x40,  /* 58..5f    XYZ[\]^_ */
  0x80, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x22,  /* 60..67    `abcdefg */
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,  /* 68..6f    hijklmno */
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,  /* 70..77    pqrstuvw */
  0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 78..7f    xyz{|}~. */

  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* 80..87    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* 88..8f    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* 90..97    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* 98..9f    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* a0..a7    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* a8..af    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* b0..b7    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* b8..bf    ........ */

  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* c0..c7    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* c8..cf    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* d0..d7    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* d8..df    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* e0..e7    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* e8..ef    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* f0..f7    ........ */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40   /* f8..ff    ........ */
};
#endif




#define i64 int64_t

#ifdef SQLITE_ASCII
# define sqlite3Toupper(x)  ((x)&~(sqlite3CtypeMap[(unsigned char)(x)]&0x20))
# define sqlite3Isspace(x)   (sqlite3CtypeMap[(unsigned char)(x)]&0x01)
# define sqlite3Isalnum(x)   (sqlite3CtypeMap[(unsigned char)(x)]&0x06)
# define sqlite3Isalpha(x)   (sqlite3CtypeMap[(unsigned char)(x)]&0x02)
# define sqlite3Isdigit(x)   (sqlite3CtypeMap[(unsigned char)(x)]&0x04)
# define sqlite3Isxdigit(x)  (sqlite3CtypeMap[(unsigned char)(x)]&0x08)
# define sqlite3Tolower(x)   (sqlite3UpperToLower[(unsigned char)(x)])
# define sqlite3Isquote(x)   (sqlite3CtypeMap[(unsigned char)(x)]&0x80)
#else
# define sqlite3Toupper(x)   toupper((unsigned char)(x))
# define sqlite3Isspace(x)   isspace((unsigned char)(x))
# define sqlite3Isalnum(x)   isalnum((unsigned char)(x))
# define sqlite3Isalpha(x)   isalpha((unsigned char)(x))
# define sqlite3Isdigit(x)   isdigit((unsigned char)(x))
# define sqlite3Isxdigit(x)  isxdigit((unsigned char)(x))
# define sqlite3Tolower(x)   tolower((unsigned char)(x))
# define sqlite3Isquote(x)   ((x)=='"'||(x)=='\''||(x)=='['||(x)=='`')
#endif

#define LARGEST_INT64  (0xffffffff|(((i64)0x7fffffff)<<32))

#define LONGDOUBLE_TYPE long double
int baseType::sqlite3AtoF(const char *z, double *pResult, int length, u8 enc){

  int incr;
  const char *zEnd = z + length;
  /* sign * significand * (10 ^ (esign * exponent)) */
  int sign = 1;    /* sign of significand */
  i64 s = 0;       /* significand */
  int d = 0;       /* adjust exponent for shifting decimal point */
  int esign = 1;   /* sign of exponent */
  int e = 0;       /* exponent */
  int eValid = 1;  /* True exponent is either not used or is well-formed */
  double result;
  int nDigits = 0;
  int nonNum = 0;  /* True if input contains UTF16 with high byte non-zero */

//  assert( enc==SQLITE_UTF8 || enc==SQLITE_UTF16LE || enc==SQLITE_UTF16BE );
  *pResult = 0.0;   /* Default return value, in case of an error */

  if( enc==SQLITE_UTF8 ){
    incr = 1;
  }else{
    int i;
    incr = 2;
//    assert( SQLITE_UTF16LE==2 && SQLITE_UTF16BE==3 );
    for(i=3-enc; i<length && z[i]==0; i+=2){}
    nonNum = i<length;
    zEnd = &z[i^1];
    z += (enc&1);
  }

  /* skip leading spaces */
  while( z<zEnd && sqlite3Isspace(*z) ) z+=incr;
  if( z>=zEnd ) return 0;

  /* get sign of significand */
  if( *z=='-' ){
    sign = -1;
    z+=incr;
  }else if( *z=='+' ){
    z+=incr;
  }

  /* copy max significant digits to significand */
  while( z<zEnd && sqlite3Isdigit(*z) && s<((LARGEST_INT64-9)/10) ){
    s = s*10 + (*z - '0');
    z+=incr; nDigits++;
  }

  /* skip non-significant significand digits
  ** (increase exponent by d to shift decimal left) */
  while( z<zEnd && sqlite3Isdigit(*z) ){ z+=incr; nDigits++; d++; }
  if( z>=zEnd ) goto do_atof_calc;

  /* if decimal point is present */
  if( *z=='.' ){
    z+=incr;
    /* copy digits from after decimal to significand
    ** (decrease exponent by d to shift decimal right) */
    while( z<zEnd && sqlite3Isdigit(*z) ){
      if( s<((LARGEST_INT64-9)/10) ){
        s = s*10 + (*z - '0');
        d--;
      }
      z+=incr; nDigits++;
    }
  }
  if( z>=zEnd ) goto do_atof_calc;

  /* if exponent is present */
  if( *z=='e' || *z=='E' ){
    z+=incr;
    eValid = 0;

    /* This branch is needed to avoid a (harmless) buffer overread.  The 
    ** special comment alerts the mutation tester that the correct answer
    ** is obtained even if the branch is omitted */
    if( z>=zEnd ) goto do_atof_calc;              /*PREVENTS-HARMLESS-OVERREAD*/

    /* get sign of exponent */
    if( *z=='-' ){
      esign = -1;
      z+=incr;
    }else if( *z=='+' ){
      z+=incr;
    }
    /* copy digits to exponent */
    while( z<zEnd && sqlite3Isdigit(*z) ){
      e = e<10000 ? (e*10 + (*z - '0')) : 10000;
      z+=incr;
      eValid = 1;
    }
  }

  /* skip trailing spaces */
  while( z<zEnd && sqlite3Isspace(*z) ) z+=incr;

do_atof_calc:
  /* adjust exponent by d, and update sign */
  e = (e*esign) + d;
  if( e<0 ) {
    esign = -1;
    e *= -1;
  } else {
    esign = 1;
  }

  if( s==0 ) {
    /* In the IEEE 754 standard, zero is signed. */
    result = sign<0 ? -(double)0 : (double)0;
  } else {
    /* Attempt to reduce exponent.
    **
    ** Branches that are not required for the correct answer but which only
    ** help to obtain the correct answer faster are marked with special
    ** comments, as a hint to the mutation tester.
    */
    while( e>0 ){                                       /*OPTIMIZATION-IF-TRUE*/
      if( esign>0 ){
        if( s>=(LARGEST_INT64/10) ) break;             /*OPTIMIZATION-IF-FALSE*/
        s *= 10;
      }else{
        if( s%10!=0 ) break;                           /*OPTIMIZATION-IF-FALSE*/
        s /= 10;
      }
      e--;
    }

    /* adjust the sign of significand */
    s = sign<0 ? -s : s;

    if( e==0 ){                                         /*OPTIMIZATION-IF-TRUE*/
      result = (double)s;
    }else{
      LONGDOUBLE_TYPE scale = 1.0;
      /* attempt to handle extremely small/large numbers better */
      if( e>307 ){                                      /*OPTIMIZATION-IF-TRUE*/
        if( e<342 ){                                    /*OPTIMIZATION-IF-TRUE*/
          while( e%308 ) { scale *= 1.0e+1; e -= 1; }
          if( esign<0 ){
            result = s / scale;
            result /= 1.0e+308;
          }else{
            result = s * scale;
            result *= 1.0e+308;
          }
        }else{ //assert( e>=342 );
          if( esign<0 ){
            result = 0.0*s;
          }else{
#ifdef INFINITY
            result = INFINITY*s;
#else
            result = 1e308*1e308*s;  /* Infinity */
#endif
          }
        }
      }else{
        /* 1.0e+22 is the largest power of 10 than can be 
        ** represented exactly. */
        while( e%22 ) { scale *= 1.0e+1; e -= 1; }
        while( e>0 ) { scale *= 1.0e+22; e -= 22; }
        if( esign<0 ){
          result = s / scale;
        }else{
          result = s * scale;
        }
      }
    }
  }

  /* store the result */
  *pResult = result;

  /* return true if number and no extra non-whitespace chracters after */
  return z==zEnd && nDigits>0 && eValid && nonNum==0;

}
//double  字节序转换
double baseType::dbtohl(double x)
{
  char tmp;
  char* p = (char*)&x;    // 指向首字节

  for(int i = 0; i < 4; i++)
  {
    tmp = *(p+i);
    *(p+i) = *(p+(7-i));
    *(p+(7-i)) = tmp;
  }

  return x;
}

int baseType::getfeildindex(vector<string> feildsVector,string key)
{
    int index=-1;
    for(int n=0;n<feildsVector.size();n++)
    {
        if(feildsVector.at(n)==key)
        {
            index=n;
            break;
        }

    }
    return index;

}