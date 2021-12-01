/**
 * @file hard_fault_handler.c
 * @brief Hard-fault handler.
 *
 * For further details see:
 *   + http://www.keil.com/appnotes/docs/apnt_209.asp
 *   + https://interrupt.memfault.com/blog/cortex-m-fault-debug
 *   + https://community.st.com/s/question/0D50X0000Az475VSQQ/
 *
 * @copyright (c) 2020 TOPTICA Photonics AG
 * @author Emilio Lopes <emilio.lopes@toptica.com>
 * @date 2020-02-07
 */

#include "stm32h743xx.h"
#include <stdint.h>

void enable_extra_usage_faults(void)
{
    SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk; /* enable trapping of division by zero */

    // Almost all Cortex processors (but not the M0!) can handle unaligned
    // access just fine and indeed GCC generates under some circumstances code
    // that perform unaligned access.  So we refrain from enabling trapping of
    // unaligned access here.
    // SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk; /* enable trapping of unaligned access */
}

/*
 * Put register values in structs to make printing them in the
 * debugger easier.
 */

typedef struct __attribute__((packed)) stack_frame_t
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t return_address;
    uint32_t xpsr;
} stack_frame_t;

typedef struct __attribute__((packed)) fault_status_registers_t
{
    uint16_t ufsr;  /**< Usage fault status register. */
    uint8_t  bfsr;  /**< Bus fault status register. */
    uint8_t  mmfsr; /**< Memory management fault status register. */
    uint32_t hfsr;  /**< Hard fault status register. */
    uint32_t dfsr;  /**< Debug fault status register. */
    uint32_t afsr;  /**< Auxiliary fault status register. */
    uint32_t bfar;  /**< Bus fault address register. */
    uint32_t mmar;  /**< Memory management fault address register. */
} fault_status_registers_t;

/*
 * We will assign the values of the registers for fault exceptions to
 * this variable in the call to `hard_fault_handler'.  We define it
 * here as a static global variable to avoid stack allocation in the
 * hard-fault handler.
 */
static volatile fault_status_registers_t fault_status;

/**
 * @brief Handler for hard-fault interrupts.
 *
 * Call the high-level hard-fault handler with the current stack
 * pointer as argument.
 *
 * This definition overwrites the _weak_ definition in the assembly
 * startup file.
 *
 * The attribute `naked` tells the compiler that this function is pure
 * assembly code and thus prologue and epilogue sequences (which would
 * change register values) are not necessary.
 */
void __attribute__((naked)) HardFault_Handler(void)
{
    __asm volatile("tst lr, #4           \n"
                   "ite eq               \n"
                   "mrseq r0, msp        \n"
                   "mrsne r0, psp        \n"
                   "b hard_fault_handler \n");
}

/**
 * @brief High-level hard-fault handler.
 *
 * This function is called by the corresponding assembly wrapper.
 * We avoid using the stack in this function and instruct the compiler
 * to refrain from any optimizations which could make debugging more
 * difficult.
 *
 * @param[in] frame Pointer to the stack frame at the time of the hard-fault.
 */
static void __attribute__((used, optimize("O0"))) hard_fault_handler(stack_frame_t* frame)
{
    /* the first three registers are accessible together as SCB->CFSR */
    fault_status.ufsr  = *(uint16_t*)0xE000ED2A;
    fault_status.bfsr  = *(uint8_t*)0xE002ED29;
    fault_status.mmfsr = *(uint8_t*)0xE000ED28;
    fault_status.hfsr  = SCB->HFSR;
    fault_status.dfsr  = SCB->DFSR;
    fault_status.afsr  = SCB->AFSR;
    fault_status.bfar  = SCB->BFAR;
    fault_status.mmar  = SCB->MMFAR;

    /* Trigger a break-point during debugging.
     *
     * To see the calling site of the offending instruction:
     *
     *   (gdb) list *(frame->lr)
     *
     * To inspect the last stack frame:
     *
     *   (gdb) p/a *frame
     *
     * To examine the fault status registers:
     *
     *   (gdb) p/x fault_status
     *
     */
    if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) {
        __asm volatile("bkpt #1");
    }

    /* endless loop */
    while (1) {
        ;
    }
}
