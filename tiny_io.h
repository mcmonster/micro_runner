/**
 * @file tiny_io.h
 * @author R. Matt McCann
 *
 * Used by Battelle Memorial Institute with permission of the original author.
 */

#ifndef TINY_IO_H_
#define TINY_IO_H_

#include "stdint.h"

/**
 * @returns Next character sent by the PC's test manager.
 */
extern uint8_t tiny_getchar();

/**
 * Prints messages to the PC's test manager.
 *
 * @param format Format of the message
 * @param ... Format arguments
 */
extern void tiny_printf(const uint8_t* format, ...);

/**
 * Called once by the test runner before starting. Use this function to initialize
 * any resources used by your getchar and printf implementations.
 */
extern void setupIO();

#endif /* TINY_IO_H_ */
