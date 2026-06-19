/*
 *  関数のプロトタイプ宣言
 */
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef TOPPERS_MACRO_ONLY
extern void main_task(intptr_t arg);
extern void High_task(intptr_t arg);
extern void Low_task(intptr_t arg);
extern void setup(void);

#endif /* TOPPERS_MACRO_ONLY */
#ifdef __cplusplus
}
#endif
