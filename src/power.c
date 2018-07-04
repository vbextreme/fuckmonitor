#include "fuckmonitor.h"

#define POWER_SUPPLY "/sys/class/power_supply/BAT0/uevent"

void power_stat(fkmonitor_s* fk){
	struct mapname{
		char* name;
		size_t* ptr;
	}map[] = { 
		{ "VOLTAGE_MIN_DESIGN=" , &fk->power.voltageMin },
		{ "VOLTAGE_NOW=" , &fk->power.voltageNow },
		{ "ENERGY_FULL=" , &fk->power.energyFull },
		{ "ENERGY_NOW=" , &fk->power.energyNow },
		{ "POWER_NOW=" , &fk->power.powerNow },
		{ "CAPACITY=" , &fk->power.capacity },
		{ NULL, NULL }
	};

	FILE* fn = fopen(POWER_SUPPLY, "r");
	if( NULL == fn ) {
		strcpy(fk->power.status, POWER_STAT_ERROR);
		dbg_warning("no %s", POWER_SUPPLY);
		return;
	}

	char in[1024];
	while( fgets(in, 1024, fn) ){
		char* parse = in + strlen("POWER_SUPPLY_");
		if( str_ncmp(parse, ==, "STATUS=") ){
			parse += 7;
			str_copy_to_ch(fk->power.status, parse, '\n');
			continue;
		}
		unsigned i;
		for(i = 0; map[i].name; ++i){
			if( str_ncmp(parse, ==, map[i].name) ){
				parse = strchr(parse, '=');
				++parse;
				*map[i].ptr = strtol(parse, NULL, 10);
				break;
			}
		}
	}

	fclose(fn);
}

void monitor_title_power(fkmonitor_s* fk){
	__private char* ico[] = { "▁", "▂", "▃", "▄", "▅", "▆", "▇", "█", "🔋"};
	
	if( str_cmp(fk->power.status, ==, POWER_STAT_ERROR) ){
		dbg_warning("No power devices");
		return;
	}
	
	unsigned iconid = fk->power.capacity / 14.3;

	double timeleft = (double)fk->power.energyNow / (double)fk->power.powerNow;
	argos_text(" %s %s%lu%%", ico[8], ico[iconid], fk->power.capacity);
	argos_text(" %u:%u", (unsigned)timeleft, (unsigned)(timeleft * 60)%60);
}

void monitor_menu_power(fkmonitor_s* fk){
	__private char* ico = "🔋";
	
	if( str_cmp(fk->power.status, ==, POWER_STAT_ERROR) ){
		dbg_warning("No power devices");
		return;
	}
	
	argos_text(" %s %lu%% %.1lfV %.1lfW %.1lfW/h", ico, fk->power.capacity, (double)fk->power.voltageNow / 1000000.0, (double)fk->power.powerNow / 1000000.0, (double)fk->power.energyNow / 1000000.0);
	argos_next();
}

