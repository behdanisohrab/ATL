#include <stdio.h>
#include <stdlib.h>

// for getting _r_debug out of the dynamic section
// this is needed by the shim bionic linker to register stuff with gdb
#include <elf.h>
#include <link.h>

// the dynamic section
extern Elf64_Dyn _DYNAMIC[];

extern struct r_debug *_r_debug_ptr;
// this has to be called from the main executable, since that's the only one guaranteed to have the debug section filled in
void init__r_debug() {
#if defined(_r_debug)
// _r_debug is defined by glibc and is declared as extern in link.h
	_r_debug_ptr = &_r_debug;
#else
	int i = 0;
	Elf64_Dyn current;

	do {
		current = _DYNAMIC[i];
		if(current.d_tag == DT_DEBUG) {
			_r_debug_ptr = (struct r_debug *)current.d_un.d_ptr;
			break;
		}
		i++;
	} while(current.d_tag != 0);

	if(!_r_debug_ptr) {
		fprintf(stderr, "error: no DEBUG tag in the dynamic section, treating this as fatal\n");
		exit(1);
	}
#endif
}
