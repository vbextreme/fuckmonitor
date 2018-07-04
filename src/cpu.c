#include "fuckmonitor.h"

#define PROC_STAT "/proc/stat"

int cpu_count(void){
	return sysconf(_SC_NPROCESSORS_ONLN);
}

void cpu_time(fkmonitor_s* fk) {
	char in[1024];
	FILE* fps = fopen(PROC_STAT, "r");
	if( fps == NULL ) {
		dbg_errno();
		dbg_fail("%s not available", PROC_STAT);
	}

	int i,k;
	for( i = 0; i < fk->cores; ++i ){
		if( NULL == fgets(in, 1024, fps) )	dbg_fail("fgets");
		char* parse;
		if( (parse = strpbrk(in, " \t")) == NULL) dbg_fail("on skip cpu");

		for(k = 0; k < CPU_TIME_COUNT; ++k){
			char* en;
			parse = str_skip_h(parse);
			fk->cputick[i][k] = strtol(parse, &en, 10);
			parse = en;
		}
	}
	
	fclose(fps);
}

uint64_t cpu_time_tick(uint64_t* st){
	int i;
	uint64_t full = 0;
	for( i = 0; i < CPU_TIME_COUNT; ++i ){
		full += *st++;
	}
	return full;
}

uint64_t cpu_tick_elapse(uint64_t* st, uint64_t* en, cputime_e ct){
	return en[ct] - st[ct];
}

double cpu_load_average(uint64_t tick, uint64_t idle){
	return ((double)(tick - idle) / (double)tick) * 100.00; 
}

__private double monitor_cpu_average(fkmonitor_s* fk, fkmonitor_s* old, int core) {
	uint64_t tick = cpu_time_tick(fk->cputick[core]) - cpu_time_tick(old->cputick[core]);
	uint64_t idle = cpu_tick_elapse(old->cputick[core], fk->cputick[core], CPU_IDLE);
	return cpu_load_average(tick, idle);
}

void monitor_title_cpu(fkmonitor_s* fk, fkmonitor_s* old){
	argos_text(" 💻 %.1lf%%", monitor_cpu_average(fk, old, 0));
}

void monitor_menu_cpu(fkmonitor_s* fk, fkmonitor_s* old){
	int i;
	for( i = 1; i < fk->cores; ++i ){
		argos_text(" 💻 %.1lf%%", monitor_cpu_average(fk, old, i));
		if( !(i % 2) ){
			argos_display("s",8);
			argos_next();
		}
	}
	if( ((i-1) % 2) ){
		argos_display("s",8);
		argos_next();
	}
}






