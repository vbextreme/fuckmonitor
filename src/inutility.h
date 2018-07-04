#ifndef __INUTILITY_H_INCLUDE__
#define __INUTILITY_H_INCLUDE__

#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

#ifndef __N__
	#define __N__ 0
#endif
#ifndef __Y__
	#define __Y__ 1
#endif

#define __private static
#define __unused __attribute__((unused))

typedef enum {FALSE, TRUE} bool_t;
typedef int err_t;

#ifndef str_cmp 
	#define str_cmp(A,OP,B) (strcmp(A,B) OP 0)
	#define str_ncmp(A,OP,B) (strncmp(A,B,strlen(B)) OP 0)
#endif

#ifndef NEW
	#define NEW(T) (T*)malloc(sizeof(T))
#endif
#ifndef MANY
	#define MANY(T,S) (T*)malloc(sizeof(T) * (S))
#endif

#ifndef EMPTY_MACRO
	#define EMPTY_MACRO do{}while(0)
#endif

#ifndef ROUND_UP
	#define ROUND_UP(N,S) ((((N)+(S)-1)/(S))*(S))
#endif



#endif
