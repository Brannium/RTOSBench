/*
 * FreeRTOS V202212.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#include <FreeRTOS.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "riscv-virt.h"
#include "htif.h"

#ifndef HTIF_SPIKE
#include "uart.h"
#include "platform.h"
static struct uart uart0;
#endif

int xGetCoreID( void )
{
	int id;

	__asm ("csrr %0, mhartid" : "=r" ( id ) );

	return id;
}

/* Use a debugger to set this to 0 if this binary was loaded through gdb instead
 * of spike's ELF loader. HTIF only works if spike's ELF loader was used. */
void initUARTNova() {
	uart_initialize(&uart0, (volatile void *)PLATFORM_UART0_BASE);
	uart_set_divisor(&uart0, uart_baud2divisor(115200, PLATFORM_SYSCLK_FREQ));
}

void vSendString( const char *s )
{
	portENTER_CRITICAL();

	#ifdef HTIF_SPIKE
		while (*s) {
			htif_putc(*s);
			s++;
		}
		htif_putc('\n');
	#else			// use NOVA UART
		uart_tx_string(&uart0, s);
	#endif

	portEXIT_CRITICAL();
}

void vSendFormatted(const char *format, ...) {
	#define MAX_BUFFER_SIZE 512
    char buffer[MAX_BUFFER_SIZE];
    va_list args;

    // Initialize the va_list with the arguments
    va_start(args, format);

    // Format the string and store it in buffer
    vsnprintf(buffer, MAX_BUFFER_SIZE, format, args);

    // Clean up the va_list
    va_end(args);

    // Send the formatted string using vSendString
    vSendString(buffer);
}

void handle_trap(void)
{
	while (1)
		;
}
