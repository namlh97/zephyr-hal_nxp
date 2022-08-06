/*
 * Copyright 2021-2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
*   @file
*
*   @addtogroup RTE_MODULE
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Std_Types.h"
#include "Mcal.h"
#include "OsIf.h"
#include "SchM_Uart.h"
#ifdef MCAL_TESTING_ENVIRONMENT
#include "EUnit.h" /* EUnit Test Suite */
#endif

/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define SCHM_UART_AR_RELEASE_MAJOR_VERSION_C     4
#define SCHM_UART_AR_RELEASE_MINOR_VERSION_C     4
#define SCHM_UART_AR_RELEASE_REVISION_VERSION_C  0
#define SCHM_UART_SW_MAJOR_VERSION_C             0
#define SCHM_UART_SW_MINOR_VERSION_C             8
#define SCHM_UART_SW_PATCH_VERSION_C             0

/*==================================================================================================
*                                       LOCAL CONSTANTS
==================================================================================================*/
#ifdef MCAL_PLATFORM_ARM
    #if (MCAL_PLATFORM_ARM == MCAL_ARM_AARCH64)
        #define ISR_STATE_MASK     ((uint32)0x000000C0UL)   /**< @brief DAIF bit I and F */
    #elif  (MCAL_PLATFORM_ARM == MCAL_ARM_RARCH)
        #define ISR_STATE_MASK     ((uint32)0x00000080UL)   /**< @brief CPSR bit I */
    #else
        #if ((defined MCAL_ENABLE_USER_MODE_SUPPORT)&&(!defined MCAL_PLATFORM_ARM_M0PLUS))
            #define ISR_STATE_MASK     ((uint32)0x000000FFUL)   /**< @brief BASEPRI[7:0] mask */
        #else
            #define ISR_STATE_MASK     ((uint32)0x00000001UL)   /**< @brief PRIMASK bit 0 */
        #endif
    #endif 
#else
    #ifdef MCAL_PLATFORM_S12
        #define ISR_STATE_MASK     ((uint32)0x00000010UL)   /**< @brief I bit of CCR */
    #else
        #define ISR_STATE_MASK     ((uint32)0x00008000UL)   /**< @brief EE bit of MSR */
    #endif
#endif
/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#ifdef MCAL_PLATFORM_ARM
    #if (MCAL_PLATFORM_ARM == MCAL_ARM_AARCH64)
        #define ISR_ON(msr)            (uint32)(((uint32)(msr) & (uint32)(ISR_STATE_MASK)) != (uint32)(ISR_STATE_MASK))
    #elif  (MCAL_PLATFORM_ARM == MCAL_ARM_RARCH)
        #define ISR_ON(msr)            (uint32)(((uint32)(msr) & (uint32)(ISR_STATE_MASK)) != (uint32)(ISR_STATE_MASK))
    #else
        #define ISR_ON(msr)            (uint32)(((uint32)(msr) & (uint32)(ISR_STATE_MASK)) == (uint32)0)
    #endif    
#else
    #ifdef MCAL_PLATFORM_S12
        #define ISR_ON(msr)            (uint32)(((uint32)(msr) & (uint32)(ISR_STATE_MASK)) == (uint32)0)
    #else
        #define ISR_ON(msr)            (uint32)((uint32)(msr) & (uint32)(ISR_STATE_MASK))
    #endif
#endif

/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL VARIABLES
==================================================================================================*/
#define RTE_START_SEC_VAR_CLEARED_32_NO_CACHEABLE
#include "Rte_MemMap.h"
VAR_SEC_NOCACHE(msr_UART_EXCLUSIVE_AREA_00)           static volatile uint32 msr_UART_EXCLUSIVE_AREA_00[NUMBER_OF_CORES];
VAR_SEC_NOCACHE(reentry_guard_UART_EXCLUSIVE_AREA_00) static volatile uint32 reentry_guard_UART_EXCLUSIVE_AREA_00[NUMBER_OF_CORES];
VAR_SEC_NOCACHE(msr_UART_EXCLUSIVE_AREA_01)           static volatile uint32 msr_UART_EXCLUSIVE_AREA_01[NUMBER_OF_CORES];
VAR_SEC_NOCACHE(reentry_guard_UART_EXCLUSIVE_AREA_01) static volatile uint32 reentry_guard_UART_EXCLUSIVE_AREA_01[NUMBER_OF_CORES];
VAR_SEC_NOCACHE(msr_UART_EXCLUSIVE_AREA_02)           static volatile uint32 msr_UART_EXCLUSIVE_AREA_02[NUMBER_OF_CORES];
VAR_SEC_NOCACHE(reentry_guard_UART_EXCLUSIVE_AREA_02) static volatile uint32 reentry_guard_UART_EXCLUSIVE_AREA_02[NUMBER_OF_CORES];
VAR_SEC_NOCACHE(msr_UART_EXCLUSIVE_AREA_03)           static volatile uint32 msr_UART_EXCLUSIVE_AREA_03[NUMBER_OF_CORES];
VAR_SEC_NOCACHE(reentry_guard_UART_EXCLUSIVE_AREA_03) static volatile uint32 reentry_guard_UART_EXCLUSIVE_AREA_03[NUMBER_OF_CORES];
VAR_SEC_NOCACHE(msr_UART_EXCLUSIVE_AREA_04)           static volatile uint32 msr_UART_EXCLUSIVE_AREA_04[NUMBER_OF_CORES];
VAR_SEC_NOCACHE(reentry_guard_UART_EXCLUSIVE_AREA_04) static volatile uint32 reentry_guard_UART_EXCLUSIVE_AREA_04[NUMBER_OF_CORES];
VAR_SEC_NOCACHE(msr_UART_EXCLUSIVE_AREA_05)           static volatile uint32 msr_UART_EXCLUSIVE_AREA_05[NUMBER_OF_CORES];
VAR_SEC_NOCACHE(reentry_guard_UART_EXCLUSIVE_AREA_05) static volatile uint32 reentry_guard_UART_EXCLUSIVE_AREA_05[NUMBER_OF_CORES];
VAR_SEC_NOCACHE(msr_UART_EXCLUSIVE_AREA_06)           static volatile uint32 msr_UART_EXCLUSIVE_AREA_06[NUMBER_OF_CORES];
VAR_SEC_NOCACHE(reentry_guard_UART_EXCLUSIVE_AREA_06) static volatile uint32 reentry_guard_UART_EXCLUSIVE_AREA_06[NUMBER_OF_CORES];
VAR_SEC_NOCACHE(msr_UART_EXCLUSIVE_AREA_07)           static volatile uint32 msr_UART_EXCLUSIVE_AREA_07[NUMBER_OF_CORES];
VAR_SEC_NOCACHE(reentry_guard_UART_EXCLUSIVE_AREA_07) static volatile uint32 reentry_guard_UART_EXCLUSIVE_AREA_07[NUMBER_OF_CORES];
VAR_SEC_NOCACHE(msr_UART_EXCLUSIVE_AREA_08)           static volatile uint32 msr_UART_EXCLUSIVE_AREA_08[NUMBER_OF_CORES];
VAR_SEC_NOCACHE(reentry_guard_UART_EXCLUSIVE_AREA_08) static volatile uint32 reentry_guard_UART_EXCLUSIVE_AREA_08[NUMBER_OF_CORES];

#define RTE_STOP_SEC_VAR_CLEARED_32_NO_CACHEABLE
#include "Rte_MemMap.h"
/*==================================================================================================
*                                       GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

#ifndef _COSMIC_C_S32ZSE_
/*================================================================================================*/
/** 
* @brief   This function returns the MSR register value (32 bits). 
* @details This function returns the MSR register value (32 bits). 
*     
* @param[in]     void        No input parameters
* @return        uint32 msr  This function returns the MSR register value (32 bits). 
* 
* @pre  None
* @post None
* 
*/
uint32 Uart_schm_read_msr(void); 
#endif /*ifndef _COSMIC_C_S32ZSE_*/
/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
#define RTE_START_SEC_CODE
#include "Rte_MemMap.h"

#if (defined(_GREENHILLS_C_S32ZSE_) || defined(_CODEWARRIOR_C_S32ZSE_))
/*================================================================================================*/
/** 
* @brief   This macro returns the MSR register value (32 bits). 
* @details This macro function implementation returns the MSR register value in r3 (32 bits). 
*     
* @pre  None
* @post None
* 
*/
#ifdef MCAL_PLATFORM_ARM
#if (MCAL_PLATFORM_ARM == MCAL_ARM_AARCH64)
ASM_KEYWORD uint32 Uart_schm_read_msr(void)
{
    mrs x0, S3_3_c4_c2_1
}
#elif  (MCAL_PLATFORM_ARM == MCAL_ARM_RARCH)
ASM_KEYWORD uint32 Uart_schm_read_msr(void)
{
    mrs r0, CPSR
}
#else
ASM_KEYWORD uint32 Uart_schm_read_msr(void)
{
#if ((defined MCAL_ENABLE_USER_MODE_SUPPORT)&&(!defined MCAL_PLATFORM_ARM_M0PLUS))
    mrs r0, BASEPRI
#else
    mrs r0, PRIMASK
#endif
}
#endif
#else
#ifdef MCAL_PLATFORM_S12
ASM_KEYWORD uint32 Uart_schm_read_msr(void)
{
   tfr ccr, d6
}
#else
ASM_KEYWORD uint32 Uart_schm_read_msr(void)
{
    mfmsr r3
}
#endif
#endif
#endif /*#ifdef GHS||CW*/

#ifdef _DIABDATA_C_S32ZSE_
/** 
* @brief   This function returns the MSR register value (32 bits). 
* @details This function returns the MSR register value (32 bits). 
*     
* @param[in]     void        No input parameters
* @return        uint32 msr  This function returns the MSR register value (32 bits). 
* 
* @pre  None
* @post None
* 
*/
#ifdef MCAL_PLATFORM_ARM
uint32 Uart_schm_read_msr(void)
{
    register uint32 reg_tmp;
    #if (MCAL_PLATFORM_ARM == MCAL_ARM_AARCH64)
        __asm volatile( " mrs %x0, DAIF " : "=r" (reg_tmp) );
    #elif  (MCAL_PLATFORM_ARM == MCAL_ARM_RARCH)
        __asm volatile( " mrs %0, CPSR " : "=r" (reg_tmp) );
    #else
        #if ((defined MCAL_ENABLE_USER_MODE_SUPPORT)&&(!defined MCAL_PLATFORM_ARM_M0PLUS))
        __asm volatile( " mrs %0, basepri " : "=r" (reg_tmp) );
        #else
        __asm volatile( " mrs %0, primask " : "=r" (reg_tmp) );
        #endif
    #endif
    return (uint32)reg_tmp;
}
#else
ASM_KEYWORD uint32 Uart_schm_read_msr(void)
{
    mfmsr r3
}    
#endif  /* MCAL_PLATFORM_ARM */

#endif   /* _DIABDATA_C_S32ZSE_*/

#ifdef _COSMIC_C_S32ZSE_
/*================================================================================================*/
/** 
* @brief   This function returns the MSR register value (32 bits). 
* @details This function returns the MSR register value (32 bits). 
*     
* @param[in]     void        No input parameters
* @return        uint32 msr  This function returns the MSR register value (32 bits). 
* 
* @pre  None
* @post None
* 
*/

#ifdef MCAL_PLATFORM_S12
    #define Uart_schm_read_msr()  ASM_KEYWORD("tfr ccr, d6")
#else
    #define Uart_schm_read_msr() ASM_KEYWORD("mfmsr r3")
#endif

#endif  /*Cosmic compiler only*/


#ifdef _HITECH_C_S32ZSE_
/*================================================================================================*/
/** 
* @brief   This function returns the MSR register value (32 bits). 
* @details This function returns the MSR register value (32 bits). 
*     
* @param[in]     void        No input parameters
* @return        uint32 msr  This function returns the MSR register value (32 bits). 
* 
* @pre  None
* @post None
* 
*/
uint32 Uart_schm_read_msr(void)
{
    uint32 result;
    __asm volatile("mfmsr %0" : "=r" (result) :);
    return result;
}

#endif  /*HighTec compiler only*/
 /*================================================================================================*/
#ifdef _LINARO_C_S32ZSE_
/** 
* @brief   This function returns the MSR register value (32 bits). 
* @details This function returns the MSR register value (32 bits). 
*     
* @param[in]     void        No input parameters
* @return        uint32 msr  This function returns the MSR register value (32 bits). 
* 
* @pre  None
* @post None
* 
*/
uint32 Uart_schm_read_msr(void)
{
    register uint32 reg_tmp;
    #if (MCAL_PLATFORM_ARM == MCAL_ARM_AARCH64)
        __asm volatile( " mrs %x0, DAIF " : "=r" (reg_tmp) );
    #elif  (MCAL_PLATFORM_ARM == MCAL_ARM_RARCH)
        __asm volatile( " mrs %0, CPSR " : "=r" (reg_tmp) );
    #else
        #if ((defined MCAL_ENABLE_USER_MODE_SUPPORT)&&(!defined MCAL_PLATFORM_ARM_M0PLUS))
        __asm volatile( " mrs %0, basepri " : "=r" (reg_tmp) );
        #else
        __asm volatile( " mrs %0, primask " : "=r" (reg_tmp) );
        #endif
    #endif
    return (uint32)reg_tmp;
}
#endif   /* _LINARO_C_S32ZSE_*/
/*================================================================================================*/

#ifdef _ARM_DS5_C_S32ZSE_
/** 
* @brief   This function returns the MSR register value (32 bits). 
* @details This function returns the MSR register value (32 bits). 
*     
* @param[in]     void        No input parameters
* @return        uint32 msr  This function returns the MSR register value (32 bits). 
* 
* @pre  None
* @post None
* 
*/
uint32 Uart_schm_read_msr(void)
{
    register uint32 reg_tmp;
    #if (MCAL_PLATFORM_ARM == MCAL_ARM_AARCH64)
        __asm volatile( " mrs %x0, DAIF " : "=r" (reg_tmp) );
    #elif  (MCAL_PLATFORM_ARM == MCAL_ARM_RARCH)
        __asm volatile( " mrs %0, CPSR " : "=r" (reg_tmp) );
    #else
        #if ((defined MCAL_ENABLE_USER_MODE_SUPPORT)&&(!defined MCAL_PLATFORM_ARM_M0PLUS))
        __asm volatile( " mrs %0, basepri " : "=r" (reg_tmp) );
        #else
        __asm volatile( " mrs %0, primask " : "=r" (reg_tmp) );
        #endif
    #endif
    return (uint32)reg_tmp;
}
#endif   /* _ARM_DS5_C_S32ZSE_ */

#ifdef _IAR_C_S32ZSE_
/** 
* @brief   This function returns the MSR register value (32 bits). 
* @details This function returns the MSR register value (32 bits). 
*     
* @param[in]     void        No input parameters
* @return        uint32 msr  This function returns the MSR register value (32 bits). 
* 
* @pre  None
* @post None
* 
*/
uint32 Uart_schm_read_msr(void)
{
    register uint32 reg_tmp;

#if ((defined MCAL_ENABLE_USER_MODE_SUPPORT)&&(!defined MCAL_PLATFORM_ARM_M0PLUS))
   __asm volatile( " mrs %0, basepri " : "=r" (reg_tmp) );
#else
   __asm volatile( " mrs %0, primask " : "=r" (reg_tmp) );
#endif

    return (uint32)reg_tmp;
}
#endif   /* _IAR_C_S32ZSE_ */

#define RTE_STOP_SEC_CODE
#include "Rte_MemMap.h"

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/
#define RTE_START_SEC_CODE
#include "Rte_MemMap.h"

void SchM_Enter_Uart_UART_EXCLUSIVE_AREA_00(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    if(0UL == reentry_guard_UART_EXCLUSIVE_AREA_00[u32CoreId])
    {
#if (defined MCAL_ENABLE_USER_MODE_SUPPORT)
        msr_UART_EXCLUSIVE_AREA_00[u32CoreId] = OsIf_Trusted_Call_Return(Uart_schm_read_msr);
#else
        msr_UART_EXCLUSIVE_AREA_00[u32CoreId] = Uart_schm_read_msr();  /*read MSR (to store interrupts state)*/
#endif /* MCAL_ENABLE_USER_MODE_SUPPORT */
        if (ISR_ON(msr_UART_EXCLUSIVE_AREA_00[u32CoreId])) /*if MSR[EE] = 0, skip calling Suspend/Resume AllInterrupts*/
        {
            OsIf_SuspendAllInterrupts();
#ifdef _ARM_DS5_C_S32ZSE_
            ASM_KEYWORD(" nop ");/* Compiler fix - forces the CSPID instruction to be generated with -02, -Ospace are selected*/
#endif
        }
    }
    reentry_guard_UART_EXCLUSIVE_AREA_00[u32CoreId]++;
}

void SchM_Exit_Uart_UART_EXCLUSIVE_AREA_00(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    reentry_guard_UART_EXCLUSIVE_AREA_00[u32CoreId]--;
    if ((ISR_ON(msr_UART_EXCLUSIVE_AREA_00[u32CoreId]))&&(0UL == reentry_guard_UART_EXCLUSIVE_AREA_00[u32CoreId]))         /*if interrupts were enabled*/
    {
        OsIf_ResumeAllInterrupts();
#ifdef _ARM_DS5_C_S32ZSE_
        ASM_KEYWORD(" nop ");/* Compiler fix - forces the CSPID instruction to be generated with -02, -Ospace are selected*/
#endif
    }
}

void SchM_Enter_Uart_UART_EXCLUSIVE_AREA_01(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    if(0UL == reentry_guard_UART_EXCLUSIVE_AREA_01[u32CoreId])
    {
#if (defined MCAL_ENABLE_USER_MODE_SUPPORT)
        msr_UART_EXCLUSIVE_AREA_01[u32CoreId] = OsIf_Trusted_Call_Return(Uart_schm_read_msr);
#else
        msr_UART_EXCLUSIVE_AREA_01[u32CoreId] = Uart_schm_read_msr();  /*read MSR (to store interrupts state)*/
#endif /* MCAL_ENABLE_USER_MODE_SUPPORT */
        if (ISR_ON(msr_UART_EXCLUSIVE_AREA_01[u32CoreId])) /*if MSR[EE] = 0, skip calling Suspend/Resume AllInterrupts*/
        {
            OsIf_SuspendAllInterrupts();
#ifdef _ARM_DS5_C_S32ZSE_
            ASM_KEYWORD(" nop ");/* Compiler fix - forces the CSPID instruction to be generated with -02, -Ospace are selected*/
#endif
        }
    }
    reentry_guard_UART_EXCLUSIVE_AREA_01[u32CoreId]++;
}

void SchM_Exit_Uart_UART_EXCLUSIVE_AREA_01(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    reentry_guard_UART_EXCLUSIVE_AREA_01[u32CoreId]--;
    if ((ISR_ON(msr_UART_EXCLUSIVE_AREA_01[u32CoreId]))&&(0UL == reentry_guard_UART_EXCLUSIVE_AREA_01[u32CoreId]))         /*if interrupts were enabled*/
    {
        OsIf_ResumeAllInterrupts();
#ifdef _ARM_DS5_C_S32ZSE_
        ASM_KEYWORD(" nop ");/* Compiler fix - forces the CSPID instruction to be generated with -02, -Ospace are selected*/
#endif
    }
}

void SchM_Enter_Uart_UART_EXCLUSIVE_AREA_02(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    if(0UL == reentry_guard_UART_EXCLUSIVE_AREA_02[u32CoreId])
    {
#if (defined MCAL_ENABLE_USER_MODE_SUPPORT)
        msr_UART_EXCLUSIVE_AREA_02[u32CoreId] = OsIf_Trusted_Call_Return(Uart_schm_read_msr);
#else
        msr_UART_EXCLUSIVE_AREA_02[u32CoreId] = Uart_schm_read_msr();  /*read MSR (to store interrupts state)*/
#endif /* MCAL_ENABLE_USER_MODE_SUPPORT */
        if (ISR_ON(msr_UART_EXCLUSIVE_AREA_02[u32CoreId])) /*if MSR[EE] = 0, skip calling Suspend/Resume AllInterrupts*/
        {
            OsIf_SuspendAllInterrupts();
#ifdef _ARM_DS5_C_S32ZSE_
            ASM_KEYWORD(" nop ");/* Compiler fix - forces the CSPID instruction to be generated with -02, -Ospace are selected*/
#endif
        }
    }
    reentry_guard_UART_EXCLUSIVE_AREA_02[u32CoreId]++;
}

void SchM_Exit_Uart_UART_EXCLUSIVE_AREA_02(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    reentry_guard_UART_EXCLUSIVE_AREA_02[u32CoreId]--;
    if ((ISR_ON(msr_UART_EXCLUSIVE_AREA_02[u32CoreId]))&&(0UL == reentry_guard_UART_EXCLUSIVE_AREA_02[u32CoreId]))         /*if interrupts were enabled*/
    {
        OsIf_ResumeAllInterrupts();
#ifdef _ARM_DS5_C_S32ZSE_
        ASM_KEYWORD(" nop ");/* Compiler fix - forces the CSPID instruction to be generated with -02, -Ospace are selected*/
#endif
    }
}

void SchM_Enter_Uart_UART_EXCLUSIVE_AREA_03(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    if(0UL == reentry_guard_UART_EXCLUSIVE_AREA_03[u32CoreId])
    {
#if (defined MCAL_ENABLE_USER_MODE_SUPPORT)
        msr_UART_EXCLUSIVE_AREA_03[u32CoreId] = OsIf_Trusted_Call_Return(Uart_schm_read_msr);
#else
        msr_UART_EXCLUSIVE_AREA_03[u32CoreId] = Uart_schm_read_msr();  /*read MSR (to store interrupts state)*/
#endif /* MCAL_ENABLE_USER_MODE_SUPPORT */
        if (ISR_ON(msr_UART_EXCLUSIVE_AREA_03[u32CoreId])) /*if MSR[EE] = 0, skip calling Suspend/Resume AllInterrupts*/
        {
            OsIf_SuspendAllInterrupts();
#ifdef _ARM_DS5_C_S32ZSE_
            ASM_KEYWORD(" nop ");/* Compiler fix - forces the CSPID instruction to be generated with -02, -Ospace are selected*/
#endif
        }
    }
    reentry_guard_UART_EXCLUSIVE_AREA_03[u32CoreId]++;
}

void SchM_Exit_Uart_UART_EXCLUSIVE_AREA_03(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    reentry_guard_UART_EXCLUSIVE_AREA_03[u32CoreId]--;
    if ((ISR_ON(msr_UART_EXCLUSIVE_AREA_03[u32CoreId]))&&(0UL == reentry_guard_UART_EXCLUSIVE_AREA_03[u32CoreId]))         /*if interrupts were enabled*/
    {
        OsIf_ResumeAllInterrupts();
#ifdef _ARM_DS5_C_S32ZSE_
        ASM_KEYWORD(" nop ");/* Compiler fix - forces the CSPID instruction to be generated with -02, -Ospace are selected*/
#endif
    }
}

void SchM_Enter_Uart_UART_EXCLUSIVE_AREA_04(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    if(0UL == reentry_guard_UART_EXCLUSIVE_AREA_04[u32CoreId])
    {
#if (defined MCAL_ENABLE_USER_MODE_SUPPORT)
        msr_UART_EXCLUSIVE_AREA_04[u32CoreId] = OsIf_Trusted_Call_Return(Uart_schm_read_msr);
#else
        msr_UART_EXCLUSIVE_AREA_04[u32CoreId] = Uart_schm_read_msr();  /*read MSR (to store interrupts state)*/
#endif /* MCAL_ENABLE_USER_MODE_SUPPORT */
        if (ISR_ON(msr_UART_EXCLUSIVE_AREA_04[u32CoreId])) /*if MSR[EE] = 0, skip calling Suspend/Resume AllInterrupts*/
        {
            OsIf_SuspendAllInterrupts();
#ifdef _ARM_DS5_C_S32ZSE_
            ASM_KEYWORD(" nop ");/* Compiler fix - forces the CSPID instruction to be generated with -02, -Ospace are selected*/
#endif
        }
    }
    reentry_guard_UART_EXCLUSIVE_AREA_04[u32CoreId]++;
}

void SchM_Exit_Uart_UART_EXCLUSIVE_AREA_04(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    reentry_guard_UART_EXCLUSIVE_AREA_04[u32CoreId]--;
    if ((ISR_ON(msr_UART_EXCLUSIVE_AREA_04[u32CoreId]))&&(0UL == reentry_guard_UART_EXCLUSIVE_AREA_04[u32CoreId]))         /*if interrupts were enabled*/
    {
        OsIf_ResumeAllInterrupts();
#ifdef _ARM_DS5_C_S32ZSE_
        ASM_KEYWORD(" nop ");/* Compiler fix - forces the CSPID instruction to be generated with -02, -Ospace are selected*/
#endif
    }
}

void SchM_Enter_Uart_UART_EXCLUSIVE_AREA_05(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    if(0UL == reentry_guard_UART_EXCLUSIVE_AREA_05[u32CoreId])
    {
#if (defined MCAL_ENABLE_USER_MODE_SUPPORT)
        msr_UART_EXCLUSIVE_AREA_05[u32CoreId] = OsIf_Trusted_Call_Return(Uart_schm_read_msr);
#else
        msr_UART_EXCLUSIVE_AREA_05[u32CoreId] = Uart_schm_read_msr();  /*read MSR (to store interrupts state)*/
#endif /* MCAL_ENABLE_USER_MODE_SUPPORT */
        if (ISR_ON(msr_UART_EXCLUSIVE_AREA_05[u32CoreId])) /*if MSR[EE] = 0, skip calling Suspend/Resume AllInterrupts*/
        {
            OsIf_SuspendAllInterrupts();
#ifdef _ARM_DS5_C_S32ZSE_
            ASM_KEYWORD(" nop ");/* Compiler fix - forces the CSPID instruction to be generated with -02, -Ospace are selected*/
#endif
        }
    }
    reentry_guard_UART_EXCLUSIVE_AREA_05[u32CoreId]++;
}

void SchM_Exit_Uart_UART_EXCLUSIVE_AREA_05(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    reentry_guard_UART_EXCLUSIVE_AREA_05[u32CoreId]--;
    if ((ISR_ON(msr_UART_EXCLUSIVE_AREA_05[u32CoreId]))&&(0UL == reentry_guard_UART_EXCLUSIVE_AREA_05[u32CoreId]))         /*if interrupts were enabled*/
    {
        OsIf_ResumeAllInterrupts();
#ifdef _ARM_DS5_C_S32ZSE_
        ASM_KEYWORD(" nop ");/* Compiler fix - forces the CSPID instruction to be generated with -02, -Ospace are selected*/
#endif
    }
}

void SchM_Enter_Uart_UART_EXCLUSIVE_AREA_06(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    if(0UL == reentry_guard_UART_EXCLUSIVE_AREA_06[u32CoreId])
    {
#if (defined MCAL_ENABLE_USER_MODE_SUPPORT)
        msr_UART_EXCLUSIVE_AREA_06[u32CoreId] = OsIf_Trusted_Call_Return(Uart_schm_read_msr);
#else
        msr_UART_EXCLUSIVE_AREA_06[u32CoreId] = Uart_schm_read_msr();  /*read MSR (to store interrupts state)*/
#endif /* MCAL_ENABLE_USER_MODE_SUPPORT */
        if (ISR_ON(msr_UART_EXCLUSIVE_AREA_06[u32CoreId])) /*if MSR[EE] = 0, skip calling Suspend/Resume AllInterrupts*/
        {
            OsIf_SuspendAllInterrupts();
#ifdef _ARM_DS5_C_S32ZSE_
            ASM_KEYWORD(" nop ");/* Compiler fix - forces the CSPID instruction to be generated with -02, -Ospace are selected*/
#endif
        }
    }
    reentry_guard_UART_EXCLUSIVE_AREA_06[u32CoreId]++;
}

void SchM_Exit_Uart_UART_EXCLUSIVE_AREA_06(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    reentry_guard_UART_EXCLUSIVE_AREA_06[u32CoreId]--;
    if ((ISR_ON(msr_UART_EXCLUSIVE_AREA_06[u32CoreId]))&&(0UL == reentry_guard_UART_EXCLUSIVE_AREA_06[u32CoreId]))         /*if interrupts were enabled*/
    {
        OsIf_ResumeAllInterrupts();
#ifdef _ARM_DS5_C_S32ZSE_
        ASM_KEYWORD(" nop ");/* Compiler fix - forces the CSPID instruction to be generated with -02, -Ospace are selected*/
#endif
    }
}

void SchM_Enter_Uart_UART_EXCLUSIVE_AREA_07(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    if(0UL == reentry_guard_UART_EXCLUSIVE_AREA_07[u32CoreId])
    {
#if (defined MCAL_ENABLE_USER_MODE_SUPPORT)
        msr_UART_EXCLUSIVE_AREA_07[u32CoreId] = OsIf_Trusted_Call_Return(Uart_schm_read_msr);
#else
        msr_UART_EXCLUSIVE_AREA_07[u32CoreId] = Uart_schm_read_msr();  /*read MSR (to store interrupts state)*/
#endif /* MCAL_ENABLE_USER_MODE_SUPPORT */
        if (ISR_ON(msr_UART_EXCLUSIVE_AREA_07[u32CoreId])) /*if MSR[EE] = 0, skip calling Suspend/Resume AllInterrupts*/
        {
            OsIf_SuspendAllInterrupts();
#ifdef _ARM_DS5_C_S32ZSE_
            ASM_KEYWORD(" nop ");/* Compiler fix - forces the CSPID instruction to be generated with -02, -Ospace are selected*/
#endif
        }
    }
    reentry_guard_UART_EXCLUSIVE_AREA_07[u32CoreId]++;
}

void SchM_Exit_Uart_UART_EXCLUSIVE_AREA_07(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    reentry_guard_UART_EXCLUSIVE_AREA_07[u32CoreId]--;
    if ((ISR_ON(msr_UART_EXCLUSIVE_AREA_07[u32CoreId]))&&(0UL == reentry_guard_UART_EXCLUSIVE_AREA_07[u32CoreId]))         /*if interrupts were enabled*/
    {
        OsIf_ResumeAllInterrupts();
#ifdef _ARM_DS5_C_S32ZSE_
        ASM_KEYWORD(" nop ");/* Compiler fix - forces the CSPID instruction to be generated with -02, -Ospace are selected*/
#endif
    }
}

void SchM_Enter_Uart_UART_EXCLUSIVE_AREA_08(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    if(0UL == reentry_guard_UART_EXCLUSIVE_AREA_08[u32CoreId])
    {
#if (defined MCAL_ENABLE_USER_MODE_SUPPORT)
        msr_UART_EXCLUSIVE_AREA_08[u32CoreId] = OsIf_Trusted_Call_Return(Uart_schm_read_msr);
#else
        msr_UART_EXCLUSIVE_AREA_08[u32CoreId] = Uart_schm_read_msr();  /*read MSR (to store interrupts state)*/
#endif /* MCAL_ENABLE_USER_MODE_SUPPORT */
        if (ISR_ON(msr_UART_EXCLUSIVE_AREA_08[u32CoreId])) /*if MSR[EE] = 0, skip calling Suspend/Resume AllInterrupts*/
        {
            OsIf_SuspendAllInterrupts();
#ifdef _ARM_DS5_C_S32ZSE_
            ASM_KEYWORD(" nop ");/* Compiler fix - forces the CSPID instruction to be generated with -02, -Ospace are selected*/
#endif
        }
    }
    reentry_guard_UART_EXCLUSIVE_AREA_08[u32CoreId]++;
}

void SchM_Exit_Uart_UART_EXCLUSIVE_AREA_08(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    reentry_guard_UART_EXCLUSIVE_AREA_08[u32CoreId]--;
    if ((ISR_ON(msr_UART_EXCLUSIVE_AREA_08[u32CoreId]))&&(0UL == reentry_guard_UART_EXCLUSIVE_AREA_08[u32CoreId]))         /*if interrupts were enabled*/
    {
        OsIf_ResumeAllInterrupts();
#ifdef _ARM_DS5_C_S32ZSE_
        ASM_KEYWORD(" nop ");/* Compiler fix - forces the CSPID instruction to be generated with -02, -Ospace are selected*/
#endif
    }
}


#ifdef MCAL_TESTING_ENVIRONMENT
/** 
@brief   This function checks that all entered exclusive areas were also exited. 
@details This function checks that all entered exclusive areas were also exited. The check
         is done by verifying that all reentry_guard_* static variables are back to the
         zero value.
    
@param[in]     void       No input parameters
@return        void       This function does not return a value. Test asserts are used instead. 

@pre  None
@post None

@remarks Covers 
@remarks Implements 
*/
void SchM_Check_uart(void)
{
    uint32 u32CoreId = (uint32)OsIf_GetCoreID();

    EU_ASSERT(0UL == reentry_guard_UART_EXCLUSIVE_AREA_00[u32CoreId]);
    reentry_guard_UART_EXCLUSIVE_AREA_00[u32CoreId] = 0UL; /*reset reentry_guard_UART_EXCLUSIVE_AREA_00 for the next test in the suite*/

    EU_ASSERT(0UL == reentry_guard_UART_EXCLUSIVE_AREA_01[u32CoreId]);
    reentry_guard_UART_EXCLUSIVE_AREA_01[u32CoreId] = 0UL; /*reset reentry_guard_UART_EXCLUSIVE_AREA_01 for the next test in the suite*/

    EU_ASSERT(0UL == reentry_guard_UART_EXCLUSIVE_AREA_02[u32CoreId]);
    reentry_guard_UART_EXCLUSIVE_AREA_02[u32CoreId] = 0UL; /*reset reentry_guard_UART_EXCLUSIVE_AREA_02 for the next test in the suite*/

    EU_ASSERT(0UL == reentry_guard_UART_EXCLUSIVE_AREA_03[u32CoreId]);
    reentry_guard_UART_EXCLUSIVE_AREA_03[u32CoreId] = 0UL; /*reset reentry_guard_UART_EXCLUSIVE_AREA_03 for the next test in the suite*/

    EU_ASSERT(0UL == reentry_guard_UART_EXCLUSIVE_AREA_04[u32CoreId]);
    reentry_guard_UART_EXCLUSIVE_AREA_04[u32CoreId] = 0UL; /*reset reentry_guard_UART_EXCLUSIVE_AREA_04 for the next test in the suite*/

    EU_ASSERT(0UL == reentry_guard_UART_EXCLUSIVE_AREA_05[u32CoreId]);
    reentry_guard_UART_EXCLUSIVE_AREA_05[u32CoreId] = 0UL; /*reset reentry_guard_UART_EXCLUSIVE_AREA_05 for the next test in the suite*/

    EU_ASSERT(0UL == reentry_guard_UART_EXCLUSIVE_AREA_06[u32CoreId]);
    reentry_guard_UART_EXCLUSIVE_AREA_06[u32CoreId] = 0UL; /*reset reentry_guard_UART_EXCLUSIVE_AREA_06 for the next test in the suite*/

    EU_ASSERT(0UL == reentry_guard_UART_EXCLUSIVE_AREA_07[u32CoreId]);
    reentry_guard_UART_EXCLUSIVE_AREA_07[u32CoreId] = 0UL; /*reset reentry_guard_UART_EXCLUSIVE_AREA_07 for the next test in the suite*/

    EU_ASSERT(0UL == reentry_guard_UART_EXCLUSIVE_AREA_08[u32CoreId]);
    reentry_guard_UART_EXCLUSIVE_AREA_08[u32CoreId] = 0UL; /*reset reentry_guard_UART_EXCLUSIVE_AREA_08 for the next test in the suite*/


}
#endif /*MCAL_TESTING_ENVIRONMENT*/

#define RTE_STOP_SEC_CODE
#include "Rte_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
