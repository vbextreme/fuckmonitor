#include "fuckmonitor.h"

char* str_skip_h(char* str) {
	while( *str && (*str == ' ' || *str == '\t') ) ++str;
	return str;
}

char* str_copy_to_ch(char* dst, char* src, char ch){
	while( *src != ch && (*dst++=*src++));
	*dst = 0;
	return src;
}
