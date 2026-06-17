/*-----------------------------------------------------------------------/
/ task_main.h                                                           /
/-----------------------------------------------------------------------*/

#ifndef _TASK_MAIN_H_DEFINED
#define _TASK_MAIN_H_DEFINED

#ifdef __cplusplus
extern "C"
{
#endif
#include <stddef.h>
#include <stdint.h>

#define NO_SSP (0) /* SSPカーネルあり*/

#ifndef TOPPERS_MACRO_ONLY
    extern void task2(intptr_t arg);
    extern void task3(intptr_t arg);


#ifndef INTHDR
    typedef void (*INTHDR)(void);
#endif
    extern void _kernel_handler(INTHDR userhandler);
    extern void isig_tim(void);
    extern void main_task(intptr_t arg);
    extern void user_handler(void);

#if NO_SSP 
    extern void sta_ker(void);
#endif


#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif

#endif /* _TASK_MAIN_H_DEFINED */
