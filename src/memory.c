#include "fuckmonitor.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

void* mem_shared_create(char* name, int priv, size_t size){
	int fm = shm_open( name, O_CREAT | O_EXCL | O_RDWR, priv);
	if( fm == -1 ){
		dbg_error("shm_open return error");
		dbg_errno();
		return NULL;
	}
	if( ftruncate(fm, size) == -1 ){
		dbg_error("ftruncate return error");
		dbg_errno();
		close(fm);
		return NULL;
	}
	
	void* mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fm, 0);
	close(fm);
	
	if( mem == (void*)-1 ){
		dbg_error("mmap return error");
		dbg_errno();
		return NULL;
	}

	return mem;
}

void* mem_shared_map(char* name){
	int fm = shm_open( name, O_RDWR, 0);
	if( fm == -1 ){
		dbg_error("shm_open return error");
		dbg_errno();
		return NULL;
	}
	struct stat ss;
	if( fstat(fm, &ss) == -1 ){
		dbg_error("stat return error");
		dbg_errno();
		close(fm);
		return NULL;
	}
	void* mem = mmap(NULL, ss.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fm, 0);
	close(fm);
	
	if( mem == (void*)-1 ){
		dbg_error("mmap return error");
		dbg_errno();
		return NULL;
	}

	return mem;
}

void* mem_shared_create_or_map(char* name, int priv, size_t size){
	void* mem = mem_shared_create(name, priv, size);
	if( mem ) return mem;
	return mem_shared_map(name);
}
	
size_t mem_shared_size(char* name){
	int fm = shm_open( name, O_RDWR, 0);
	if( fm == -1 ){
		dbg_error("shm_open return error");
		dbg_errno();
		return 0;
	}
		
	struct stat ss;
	if( fstat(fm, &ss) == -1 ){
		dbg_error("stat return error");
		dbg_errno();
		close(fm);
		return 0;
	}
	close(fm);
	return ss.st_size;
}

void mem_shared_unmap(void* mem, size_t size){
	if( munmap(mem, size) == -1 ){
		dbg_error("stat return error");
		dbg_errno();
	}
}

int mem_shared_close(char* name){
	if( shm_unlink(name) == -1 ){
		dbg_error("stat return error");
		dbg_errno();
		return -1;
	}
	return 0;
}

void* mem_alloc(void** mem, size_t size){
	void* ret = *mem;
	size = ROUND_UP(size, sizeof(void*));
	*mem += size;
	return ret;
}
