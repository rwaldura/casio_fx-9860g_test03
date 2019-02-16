/*
 * From http://media.taricorp.net/prizm/simon_chm/fx_useful_functions.htm
 */

/*
 * mov.l   #h'80010070, r2
 * mov.l   #SyscallNo, r0
 * jmp     @r2
 * nop
 */
#define SYSCALL_A 0xD201D002
#define SYSCALL_B 0x422B0009

// 0x80010070 is the fx-9860 syscall entrypoint
// 0x80020070 is the fx-CG syscall entrypoint (but remember: the syscall numbers are different!)
#define SYSCALL_ENTRY 0x80010070
// #define SYSCALL_ENTRY 0x80020070

// define some function pointer types
// (for every syscall's interface a different function pointer type is required)
// the following type is for syscalls, which return an int and require no input.
typedef int (*sc_iv)(void);
// the following type is for syscalls, which return an int and require an int-pointer
// as input (like GetKey).
typedef int (*sc_ipi)(int*);

// return a string, take a string as input
typedef char* (*sc_cpcp)(char*);

/* 
 * 0x462: char *GetAppName(char *dest);
 * Copies the registered name for the running application into the character
 * array dest. dest must be able to hold 9 bytes. dest is returned.
 */
const unsigned int sc0462[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x0462 };
#define GetAppName (*(sc_cpcp) sc0462)

/*
 * 
 */
const unsigned int sc003B[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x003B };
#define RTC_GetTicks (*(sc_iv) sc003B)

/******************** 

// example 1
const unsigned int sc0998[] = { SCA, SCB, SCE, 0x0998 };
#define App_DYNA (*(sc_iv)sc0998)

// now you could use
  App_DYNA();
// to call the built-in DYNA.

// or

// example 2
const unsigned int sc090F[] = { SCA, SCB, SCE, 0x090F };
#define GetKey (*(sc_ipi)sc090F)

// to call
  GetKey( &key );

 ************************/