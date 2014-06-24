/**
 * @file tiny_io.c
 * @author R. Matt McCann, Battelle Memorial Institute
 * @brief VITAL module specific implementation of stdio functionality required by tiny_runner
 *
 * @subsection Copyright Notice
 * &copy; 2014 Battelle Memorial Institute
 */

#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "tiny_io.h"
#include "usart.h"

uint8_t tiny_getchar() {
    uint8_t  data;
    uint16_t numBytesRead;
    uint16_t size = 1;

    // Block until the next character is received
    while (false == UartTestPortHasNewData());

    UartTestPortGetNewData(&data, size, &numBytesRead);

    return data;
}

void tiny_printf(const uint8_t* format, ...) {
    va_list args;
    uint8_t buffer[5000];

    // Fetch the variable length arguments
    va_start(args, format);

    // Prepare the message to be printed
    vsprintf(buffer, format, args);

    // Clean up the variable length arguments
    va_end(args);

    UartTestPortSend(buffer, strlen(buffer));
}

void setupIO() {
    UART_ABTP_Init();
}
