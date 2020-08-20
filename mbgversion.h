
/**************************************************************************
 *
 *  $Id: mbgversion.h 1.16.1.1 2020/05/13 11:07:52 martin TEST $
 *
 *  Copyright (c) Meinberg Funkuhren, Bad Pyrmont, Germany
 *
 *  Description:
 *    Program version definitions for package mbgtools-lx.
 *
 * -----------------------------------------------------------------------
 *  $Log: mbgversion.h $
 *  Revision 1.16.1.1  2020/05/13 11:07:52  martin
 *  Changed version code to 4.2.11 for the develoment branch.
 *  Revision 1.16  2020/03/31 12:47:43  martin
 *  New version code 4.2.10.
 *  Revision 1.15  2020/02/24 11:17:02  martin
 *  Changed version code to 4.2.8 and copyright year to 2020.
 *  Revision 1.14  2019/03/18 08:54:36  martin
 *  New version code 4.2.6.
 *  Revision 1.13  2019/03/11 16:05:00  martin
 *  New version code 4.2.4.
 *  Revision 1.12  2019/02/08 10:18:07  martin
 *  New version code 4.2.2.
 *  Changed current copyright year to 2019.
 *  Revision 1.11  2018/12/13 15:39:00  martin
 *  New version code 4.2.1.
 *  Revision 1.10  2018/08/07 13:15:33  martin
 *  Cleaned up version definitions.
 *  Revision 1.9  2018/06/25 15:53:14  martin
 *  New version code 4.2.0.
 *  Changed copyright year to 2018.
 *  Revision 1.8  2017/07/03 09:05:11  martin
 *  New version code 4.0.0.
 *  Revision 1.7  2017/01/31 13:04:21  martin
 *  Include words.h instead of mbg_tgt.h
 *  Revision 1.6  2017/01/27 12:05:34  martin
 *  Changed copyright year to 2017.
 *  New file format.
 *  Revision 1.5  2015/12/17 14:40:04  martin
 *  Changed copyright year to 2015.
 *  Revision 1.4  2014/01/06 11:08:10  martin
 *  Changed copyright year to 2014.
 *  Revision 1.3  2013/01/03 12:09:30  martin
 *  Changed copyright year to 2013.
 *  Revision 1.2  2012/01/17 10:10:54  martin
 *  Changed current copyright year to 2012.
 *  Revision 1.1  2011/07/08 11:38:32  martin
 *  Initial revision for pre-release.
 *
 **************************************************************************/

#include <words.h>   // also includes mbg_tgt.h

// --------------------------------------------------------------------
// The following definitions have to be updated to reflect the
// copyright year, and the software version numbers.
// --------------------------------------------------------------------

#define MBG_CURRENT_COPYRIGHT_YEAR      2020

#define MBG_MAJOR_VERSION_CODE          4
#define MBG_MINOR_VERSION_CODE          2
#define MBG_MICRO_VERSION_CODE          11

#if defined( MBG_TGT_WIN32 )
  #define MBG_BUILD_VERSION_CODE        1
#endif


// --------------------------------------------------------------------
// The following definitions are derived from the definitions above.
// --------------------------------------------------------------------

#define MBG_CURRENT_COPYRIGHT_YEAR_STR  STRINGIFY( MBG_CURRENT_COPYRIGHT_YEAR )

#define MBG_MAJOR_VERSION_STR    STRINGIFY( MBG_MAJOR_VERSION_CODE )
#define MBG_MINOR_VERSION_STR    STRINGIFY( MBG_MINOR_VERSION_CODE )
#define MBG_MICRO_VERSION_STR    STRINGIFY( MBG_MICRO_VERSION_CODE )

#if defined( MBG_BUILD_VERSION_CODE )
  #define MBG_BUILD_VERSION_STR  STRINGIFY( MBG_BUILD_VERSION_CODE )
#endif


#define MBG_MAIN_VERSION_CODE    ( ( MBG_MAJOR_VERSION_CODE << 8 ) | MBG_MINOR_VERSION_CODE )
#define MBG_MAIN_VERSION_STR     MBG_MAJOR_VERSION_STR "." MBG_MINOR_VERSION_STR

#if defined( MBG_BUILD_VERSION_STR )
  #define MBG_FULL_VERSION_STR    MBG_MAIN_VERSION_STR "." MBG_MICRO_VERSION_STR "." MBG_BUILD_VERSION_STR
#else
  #define MBG_FULL_VERSION_STR    MBG_MAIN_VERSION_STR "." MBG_MICRO_VERSION_STR
#endif

