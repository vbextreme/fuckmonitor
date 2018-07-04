#ifndef __DEBUG_H_INCLUDE__
#define __DEBUG_H_INCLUDE__

#include <errno.h>
#include "inutility.h"

#ifndef DBG_OUTPUT
	#define DBG_OUTPUT stderr
#endif

#ifndef PRINTE 
	#define PRINTE(FORMAT, arg...) fprintf(DBG_OUTPUT, FORMAT, ## arg)
#endif

#ifdef DEBUG_COLOR
	#define DBG_COLOR_INFO    "\033[36m"
	#define DBG_COLOR_WARNING "\033[93m"
	#define DBG_COLOR_ERROR   "\033[31m"
	#define DBG_COLOR_FAIL    "\033[91m"
	#define DBG_COLOR_RESET   "\033[m"
#else
	#define DBG_COLOR_INFO    ""
	#define DBG_COLOR_WARNING ""
	#define DBG_COLOR_ERROR   ""
	#define DBG_COLOR_FAIL    ""
	#define DBG_COLOR_RESET   ""
#endif

#ifndef DBG_INFO
	#define DBG_INFO    "info"
#endif
#ifndef DBG_WARNING
	#define DBG_WARNING "warning"
#endif
#ifndef DBG_ERROR
	#define DBG_ERROR   "error"
#endif
#ifndef DBG_FAIL
	#define DBG_FAIL    "fail"
#endif
#ifndef DBG_ERRNO
	#define DBG_ERRNO   "errno"
#endif

#ifndef DBG_LVL_FAIL
	#define DBG_LVL_FAIL    1
#endif
#ifndef DBG_LVL_ERROR
	#define DBG_LVL_ERROR   2
#endif
#ifndef DBF_LVL_WARNING
	#define DBG_LVL_WARNING 3
#endif
#ifndef DBGBG_LVL_INFO
	#define DBG_LVL_INFO    4
#endif

#if DEBUG_ENABLE >= 1
	#define dbg(TYPE, COLOR, FORMAT, arg...) do{\
										fprintf(DBG_OUTPUT, "%s[%u]:{%d} %s(): %s%s" DBG_COLOR_RESET "::" FORMAT "\n",\
										__FILE__,\
									   	__LINE__,\
									   	0,\
										__FUNCTION__,\
										COLOR, TYPE,\
										## arg); \
										fflush(DBG_OUTPUT);\
									  }while(0)

	#define dbg_fail(FORMAT, arg...) do{ \
										dbg(DBG_FAIL, DBG_COLOR_FAIL, FORMAT, ## arg);\
										exit(1);\
									 }while(0)

	#define dbg_errno() dbg(DBG_ERRNO, DBG_COLOR_ERROR, " %d descript: %s", errno, strerror(errno)) 
#else
	#define dbg(TYPE, FORMAT, arg...) EMPTY_MACRO
	#define dbg_fail(FORMAT, arg...) do{exit(1);}while(0)
	#define dbg_errno() EMPTY_MACRO
#endif

#if DEBUG_ENABLE > DBG_LVL_FAIL
	#define dbg_error(FORMAT, arg...) dbg(DBG_ERROR, DBG_COLOR_ERROR, FORMAT, ## arg)
#else
	#define dbg_error(FORMAT, arg...) EMPTY_MACRO
#endif

#if DEBUG_ENABLE > DBG_LVL_ERROR
	#define dbg_warning(FORMAT, arg...) dbg(DBG_WARNING, DBG_COLOR_WARNING, FORMAT, ## arg)
#else
	#define dbg_warning(FORMAT, arg...) EMPTY_MACRO
#endif

#if DEBUG_ENABLE > DBG_LVL_WARNING
	#define dbg_info(FORMAT, arg...) dbg(DBG_INFO, DBG_COLOR_INFO, FORMAT, ## arg)
#else
	#define dbg_info(FORMAT, arg...) EMPTY_MACRO
#endif
	
#if IASSERT_ENABLE == __Y__
	#define iassert(C) do{ if ( !(C) ){fprintf(stderr,"assertion fail %s[%u]: %s\n", __FILE__, __LINE__, #C); exit(0);}}while(0)
#else
	#define iassert(C) EMPTY_MACRO
#endif

#if RASSERT_ENABLE == __Y__
	#define rassert(C) do{ if ( !(C) ){fprintf(stderr,"assertion fail %s[%u]: %s\n", __FILE__, __LINE__, #C); exit(0);}}while(0)
#else
	#define rassert(C) EMPTY_MACRO
#endif

#endif
