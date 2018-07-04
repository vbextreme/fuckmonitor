#ifndef __FUCKMONITOR_H_INCLUDE__
#define __FUCKMONITOR_H_INCLUDE__

#include "debug.h"
#include <sys/sysinfo.h>

#ifndef FK_NCORES
	#define FK_NCORES 64
#endif
typedef enum { CPU_USER, CPU_NICE, CPU_SYSTEM, CPU_IDLE, CPU_IOWAIT, CPU_IRQ, CPU_SOFTIRQ, CPU_STEAL, CPU_GUEST, CPU_GUEST_NICE, CPU_TIME_COUNT } cputime_e;

#ifndef FK_MAX_NET_DEVICES
	#define FK_MAX_NET_DEVICES 12
#endif
typedef enum { ND_BYTES, ND_PACKETS, ND_ERRS, ND_DROP, ND_FIFO, ND_FRAME, ND_COMPRESSED, ND_MULTICAST, ND_COUNT } netdev_e;

#define POWER_STAT_ERROR "ERR"
typedef struct powerstat{
	char status[32];
	size_t voltageMin; /* uV */
	size_t voltageNow; /* uV */
	size_t energyFull; /* uW/h */
	size_t energyNow; /* uW/h */
	size_t powerNow; /* uW */
	size_t capacity; /* % */
}powerstat_s;

typedef struct netdev {
	char name[128];
	size_t receive[ND_COUNT];
	size_t transmit[ND_COUNT];
}netdev_s;

typedef struct fkmonitor {
	int id;
	int cores;
	uint64_t cputick[FK_NCORES][CPU_TIME_COUNT];
	uint64_t ms;
	struct sysinfo mem;
	netdev_s devs[FK_MAX_NET_DEVICES];
	size_t devscount;
	powerstat_s power;
}fkmonitor_s;

/*** memory.c ***/
void* mem_shared_create_or_map(char* name, int priv, size_t size);
void* mem_shared_create(char* name, int priv, size_t size);
void* mem_shared_map(char* name);
void* mem_shared(char* name, int priv, size_t size);
size_t mem_shared_size(char* name);
void mem_shared_unmap(void* mem, size_t size);
int mem_shared_close(char* name);
void* mem_alloc(void** mem, size_t size);

/*** string.h ***/
char* str_skip_h(char* str);
char* str_copy_to_ch(char* dst, char* src, char ch);

/*** argos.c ***/
#define argos_text printf
void argos_separator(void);
void argos_submenu(void);
void argos_be_title(void);
void argos_next(void);
void argos_display(char* frm, ...);
void argos_action(char* frm, ...);

/*** delay.c ***/
uint64_t time_ms(void);
uint64_t time_us(void);
double time_dbls(void);
void delay_ms(uint64_t ms);
void delay_us(uint64_t us);
void delay_dbls(double s);
void delay_hard(uint64_t us);

/*** cpu.c ***/
int cpu_count(void);
void cpu_time(fkmonitor_s* fk);
uint64_t cpu_time_tick(uint64_t* st);
uint64_t cpu_tick_elapse(uint64_t* st, uint64_t* en, cputime_e ct);
double cpu_load_average(uint64_t tick, uint64_t idle);
void monitor_title_cpu(fkmonitor_s* fk, fkmonitor_s* old);
void monitor_menu_cpu(fkmonitor_s* fk, fkmonitor_s* old);

/*** net.c ***/
void net_device(fkmonitor_s* fk);
void monitor_display_unit(double met, unsigned un);
void monitor_title_net(fkmonitor_s* fk, fkmonitor_s* old);
void monitor_menu_net(fkmonitor_s* fk, fkmonitor_s* old);

/*** power.c ***/
void power_stat(fkmonitor_s* fk);
void monitor_title_power(fkmonitor_s* fk);
void monitor_menu_power(fkmonitor_s* fk);

#endif
