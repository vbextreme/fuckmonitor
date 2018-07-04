#include "fuckmonitor.h"

#define FK_DATA "/fuck-monitor"

__private void* memory;
__private uint64_t* fkcur;
__private uint64_t fkold;
__private fkmonitor_s* fkpage[2];

__private void memory_alloc(void) {
	void* mfree = memory;
	fkcur = mem_alloc(&mfree, sizeof(uint64_t));
	fkpage[0] = mem_alloc(&mfree, sizeof(fkmonitor_s));
	fkpage[1] = mem_alloc(&mfree, sizeof(fkmonitor_s));
}

__private void memory_load(void){
	size_t memsize = sizeof(fkmonitor_s) * 2 + sizeof(uint64_t);
	memsize = ROUND_UP(memsize, 4096);
	dbg_info("struct size: %lu", sizeof(fkmonitor_s));
	dbg_info("mem used: %lu", memsize);

	memory = mem_shared_create_or_map(FK_DATA, 0660, memsize);
	if( NULL == memory ){
		dbg_fail("memory allocation");
	}
	
	memory_alloc();
	fkold = *fkcur;
	*fkcur = (*fkcur + 1) & 1;
	dbg_info("current(%ld) old(%ld)", *fkcur, fkold);
}

__private void memory_destroy(void) {
	mem_shared_close(FK_DATA);
}

__private inline fkmonitor_s* fkmonitor_current(void) {
	return fkpage[*fkcur];
}

__private inline fkmonitor_s* fkmonitor_old(void) {
	return fkpage[fkold];
}

__private void fkmonitor_init(fkmonitor_s* fk){
	dbg_info("init cpu");
	fk->cores = cpu_count() + 1;
	cpu_time(fk);

	dbg_info("init mem");
	if( sysinfo(&fk->mem) < 0 ){
		dbg_fail("many errors");
	}
	
	dbg_info("init net");
	net_device(fk);
	
	dbg_info("get time");
	fk->ms = time_ms();

	dbg_info("init power");
	power_stat(fk);
}

__private void monitor_title_memory(fkmonitor_s* fk){
	double mem = fk->mem.totalram - fk->mem.freeram;
	mem = mem * 100.0 / (double)fk->mem.totalram;
	argos_text(" 🌀 %.1lf%%",mem);
}

__private void monitor_title_end(){
	argos_display("s",8);
	argos_be_title();
}

__private void monitor_menu_memory(fkmonitor_s* fk){
	double mem = fk->mem.totalram - fk->mem.freeram;
	mem = mem * 100.0 / (double)fk->mem.totalram;
	double pfree = (double)fk->mem.freeram * 100.0 / (double)fk->mem.totalram;
	double	unit = (1024.0 * 1024.0) / fk->mem.mem_unit;
	argos_text(" 🌀 used %.1lf MiB %.1lf%%", (fk->mem.totalram - fk->mem.freeram) / unit, mem);
	argos_display("s",8);
	argos_next();
	argos_text(" 🌀 free %.1lf MiB %.1lf%%", fk->mem.freeram / unit, pfree);
	argos_display("s",8);
	argos_next();
	argos_text(" 🌀 total %.1lf MiB", fk->mem.totalram / unit);
	argos_display("s",8);
	argos_next();
}

int main(int argc, char** argv)
{
	if( argc > 1 && str_cmp(argv[1], == ,"destroy") ){
		memory_destroy();
		return 0;
	}

	memory_load();
	fkmonitor_s* fkc = fkmonitor_current();
	fkmonitor_s* fko = fkmonitor_old();
	fkmonitor_init(fkc);
	monitor_title_cpu(fkc, fko);
	monitor_title_memory(fkc);
	monitor_title_net(fkc,fko);
	monitor_title_power(fkc);
	monitor_title_end();

	monitor_menu_cpu(fkc, fko);
	monitor_menu_memory(fkc);
	monitor_menu_net(fkc, fko);
	monitor_menu_power(fkc);
	
	return 0;
}
