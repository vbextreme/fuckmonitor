#include "fuckmonitor.h"

#define PROC_NET_DEV "/proc/net/dev"

void net_device(fkmonitor_s* fk){
	FILE* fn = fopen(PROC_NET_DEV, "r");
	if( NULL == fn ) {
		dbg_fail("no %s", PROC_NET_DEV);
	}

	char in[1024];
	if( NULL == fgets(in, 1024, fn) ){
		dbg_fail("no fgets1");
	}
	if( NULL == fgets(in, 1024, fn) ){
		dbg_fail("no fgets2");
	}
	
	size_t k = 0;
	while( fgets(in, 1024, fn) && k < FK_MAX_NET_DEVICES ){
		char* parse = str_skip_h(in);
		parse = str_copy_to_ch(fk->devs[k].name, parse, ':');
		++parse;
		parse = str_skip_h(parse);
		size_t i;
		for( i = 0; i < ND_COUNT; ++i){
			char* toked;
			fk->devs[k].receive[i] = strtol(parse, &toked, 10);
			parse = str_skip_h(toked);
		}
		for( i = 0; i < ND_COUNT; ++i){
			char* toked;
			fk->devs[k].transmit[i] = strtol(parse, &toked, 10);
			parse = str_skip_h(toked);
		}
		++k;
	}

	fclose(fn);
	fk->devscount = k;
}

void monitor_display_unit(double met, unsigned un) {
	__private char* unit[] = {"B","KB","MB","GB","TB"};
	dbg_info("bytes %.1lf unit %s", met, unit[un]);
	while(met > 1000.0 && un < 4){
		met /= 1000.0;
		++un;
		dbg_info("bytes %.1lf unit %s", met, unit[un]);
	}
	argos_text("%.1lf%s", met, unit[un]);
}

void monitor_title_net(fkmonitor_s* fk, fkmonitor_s* old){
	size_t i = 1;
	size_t transmit = fk->devs[0].transmit[ND_BYTES] - old->devs[0].transmit[ND_BYTES];
	size_t receive = fk->devs[0].receive[ND_BYTES] - old->devs[0].receive[ND_BYTES];
	size_t max = 0;

	for(; i < fk->devscount && i < old->devscount; ++i){
		size_t t = fk->devs[i].transmit[ND_BYTES] - old->devs[i].transmit[ND_BYTES];
		size_t r = fk->devs[i].receive[ND_BYTES] - old->devs[i].receive[ND_BYTES];
		if( t > transmit && r > receive){
			max = i;
			transmit = t;
			receive = r;
		}
	}
	
	double elapse = (fk->ms - old->ms) / 1000.0;
	argos_text(" 🖧 %s",fk->devs[max].name);
	argos_text(" ⇧ ");
	monitor_display_unit(transmit / elapse,0);
	argos_text("/s ⇩ ");
	monitor_display_unit(receive / elapse,0);
	argos_text("/s");
}

void monitor_menu_net(fkmonitor_s* fk, fkmonitor_s* old){
	size_t i = 0;
	double elapse = (fk->ms - old->ms / 1000.0);

	for(; i < fk->devscount && i < old->devscount; ++i){
		size_t t = fk->devs[i].transmit[ND_BYTES] - old->devs[i].transmit[ND_BYTES];
		size_t r = fk->devs[i].receive[ND_BYTES] - old->devs[i].receive[ND_BYTES];
		argos_text(" 🖧 %s",fk->devs[i].name);
		argos_text(" ⇧ ");
		monitor_display_unit(t / elapse,0);
		argos_text("/s ⇩ ");
		monitor_display_unit(r / elapse,0);
		argos_text("/s");
		argos_display("s",8);
		argos_next();
	}
}



