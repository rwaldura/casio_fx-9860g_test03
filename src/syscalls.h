/*
 * From http://media.taricorp.net/prizm/simon_chm/fx_useful_functions.htm
 * see also http://media.taricorp.net/prizm/simon_chm/fx_legacy_syscalls.htm
 */

/*
 * How to execute a syscall:
 *
 * mov.l   #h'80010070, r2
 * mov.l   #SyscallNo, r0
 * jmp     @r2
 * nop
 */
#define SYSCALL_A 0xD201D002
#define SYSCALL_B 0x422B0009

// 0x80010070 is the fx-9860 syscall entrypoint
// 0x80020070 is the fx-CG syscall entrypoint (but remember: the syscall numbers are different!)
#ifdef FX_CG
#define SYSCALL_ENTRY 0x80020070
#else
#define SYSCALL_ENTRY 0x80010070
#endif

// define some function pointer types
// (for every syscall interface, a different function pointer type is required)
// the following type is for syscalls which return an int, and require no input.
typedef int (*sc_iv)(void);
// the following type is for syscalls which return an int, and require an int pointer
// as input (e.g. GetKey).
typedef int (*sc_ipi)(int*);
// return a string, take a string as input
typedef char* (*sc_cpcp)(char*);
// takes nothing, returns nothing
typedef void (*sc_vv)(void);

/* 
 * 0x462: char *GetAppName(char *dest);
 * Copies the registered name for the running application into the character
 * array dest. dest must be able to hold 9 bytes. dest is returned.
 */
const unsigned int sc0462[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x0462 };
#define GetAppName (*(sc_cpcp) sc0462)

/*
 * Internal clock.
 */
const unsigned int sc003B[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x003B };
#define RTC_GetTicks (*(sc_iv) sc003B)

/*
 * Disable the MENU key
 * http://media.taricorp.net/prizm/simon_chm/fx_legacy_keyboard.htm
 * This call disables the special MENU-key function. After calling this function,
 * it is not longer possible to change to the main menu!
 * An application should not try to return with the special MENU-key function
 * disabled. The special MENU-key function has to be enabled, before leaving
 * the application. 
 */
const unsigned int sc0478[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x0478 };
#define DisableGetkeyToMainFunctionReturn (*(sc_vv) sc0478)

/*
 * Enable the MENU key
 */
const unsigned int sc0477[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x0477 };
#define EnableGetkeyToMainFunctionReturn (*(sc_vv) sc0477)
