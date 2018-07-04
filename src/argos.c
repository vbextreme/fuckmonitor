#include "fuckmonitor.h"

__private int att = 0;

__private void argos_begin_attribute() {
	if( att ) return;
	fputs(" |",stdout);
}

void argos_separator(void) {
	puts("---");
	att = 0;
}

void argos_submenu(void) {
	fputs("--",stdout);
}

void argos_be_title(void) {
	putchar('\n');
	argos_separator();
}

void argos_next(void) {
	putchar('\n');
	att = 0;
}

#define MAPD_SET(CH,STR) [CH - 'A'] = STR
void argos_display(char* frm, ...) {
	static char* mapd[] = { 
		MAPD_SET('c',"color"), 
		MAPD_SET('f',"font"), 
		MAPD_SET('s',"size"), 
		MAPD_SET('N',"iconName"), 
		MAPD_SET('i',"image"), 
		MAPD_SET('w',"imageWidth"), 
		MAPD_SET('h',"imageHeight"), 
		MAPD_SET('l',"lenght"), 
		MAPD_SET('t',"trim"),
		MAPD_SET('d',"dropdown"), 
		MAPD_SET('a',"alternate"), 
		MAPD_SET('e',"emojize"), 
		MAPD_SET('n',"ansi"),
		MAPD_SET('M',"useMarkup"),
		MAPD_SET('u',"unescape")
	};

	argos_begin_attribute();
	
	va_list vl;
	va_start(vl, frm);
	char* argch;
	int argi;

	for(; *frm; ++frm ) {
		switch( *frm ){
			case 'c': case 'f': case 'N': case 'i':
				argch = va_arg(vl, char*);
				printf(" %s=%s",mapd[*frm - 'A'], argch);
			break;

			case 's': case 'w': case 'h': case 'l':
				argi = va_arg(vl, int);
				printf(" %s=%d",mapd[*frm - 'A'], argi);
			break;

			case 't': case 'd': case 'a': case 'e': case 'n': case 'M': case 'u':
				argi = va_arg(vl, int);
				printf(" %s=%s",mapd[*frm - 'A'], argi ? "true" : "false");
			break;

			default: goto ERROR;
		}
	}

ERROR:
	va_end(vl);
}

#define MAPA_SET(CH,STR) [CH - 'a'] = STR
void argos_action(char* frm, ...) {
	static char* mapa[] = { 
		MAPA_SET('b',"bash"), 
		MAPA_SET('t',"terminal"), 
		MAPA_SET('h',"href"), 
		MAPA_SET('e',"eval"), 
		MAPA_SET('r',"refresh") 
	};

	argos_begin_attribute();
	
	va_list vl;
	va_start(vl, frm);
	char* argch;
	int argi;

	for(; *frm; ++frm ) {
		switch( *frm ){
			case 'b': case 'h': case 'e':
				argch = va_arg(vl, char*);
				printf(" %s=%s",mapa[*frm - 'a'], argch);
			break;

			case 't': case 'r':
				argi = va_arg(vl, int);
				printf(" %s=%s",mapa[*frm - 'a'], argi ? "true" : "false");
			break;

			default: goto ERROR;
		}
	}

ERROR:
	va_end(vl);
}







