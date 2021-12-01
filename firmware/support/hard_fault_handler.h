/**
 * @file hard_fault_handler.h
 * @brief Hard-fault handler.
 * @copyright (c) 2020 TOPTICA Photonics AG
 * @author Emilio Lopes <emilio.lopes@toptica.com>
 * @date 2020-02-07
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enable extra usage fault interrupts.
 *
 * Enable usage-fault interrupts in the following cases:
 *   + Division by zero.
 *   + Memory access to unaligned addresses.
 */
void enable_extra_usage_faults(void);

#ifdef __cplusplus
}
#endif
