#ifndef __BasicTypes_H
#define __BasicTypes_H


typedef signed          char  H8;
typedef signed short    int   H16;
typedef signed          int   H32;
//typedef signed        __int64 H64;

typedef unsigned        char  N8;
typedef unsigned short  int   N16;
typedef unsigned        int   N32;
//typedef unsigned      __int64 N64;


typedef const signed          char  HC8;
typedef const signed short    int   HC16;
typedef const signed          int   HC32;
//typedef const signed        __int64 HC64;

typedef const unsigned        char  NC8;
typedef const unsigned short  int   NC16;
typedef const unsigned        int   NC32;
//typedef const unsigned      __int64 NC64;


typedef volatile signed          char  HV8;
typedef volatile signed short    int   HV16;
typedef volatile signed          int   HV32;
//typedef volatile signed        __int64 HV64;

typedef volatile unsigned        char  NV8;
typedef volatile unsigned short  int   NV16;
typedef volatile unsigned        int   NV32;
//typedef volatile unsigned      __int64 NV64;


typedef volatile const signed          char  HVC8;
typedef volatile const signed short    int   HVC16;
typedef volatile const signed          int   HVC32;
//typedef volatile const signed        __int64 HVC64;

typedef volatile const unsigned        char  NVC8;
typedef volatile const unsigned short  int   NVC16;
typedef volatile const unsigned        int   NVC32;
//typedef volatile const unsigned      __int64 NVC64;



typedef float           F32;
typedef double          F64;

#define TRUE            1       
#define FALSE           0  
//#define ENABLE          1       
//#define DISBLE          0

#define FORWARD 1    
#define REVERSE 2    
#define STOP    0   
#define PAUSE   1
#define ALLOW   0  

/*
·ûºÅ      Ö¸Êý      Î²Êý
1       8       23+1
1       11      52+1
*/
#endif
