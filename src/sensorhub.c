/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zephyr.h>
#include <soc.h>
#include <gpio.h>
#include <misc/printk.h>

#include "ota_debug.h"

int shub_temp;
unsigned int shub_humidity;

#if defined(CONFIG_CONSOLE_HANDLER)

#include <drivers/console/console.h>
#include <drivers/console/uart_console.h>

#define STACKSIZE 512
static char __stack stack[STACKSIZE];

static struct k_fifo avail_queue;
static struct k_fifo avail_lines;

#define MAX_SHUB_UART_LINES 5
static struct console_input buf[MAX_SHUB_UART_LINES];

static void sensorhub(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	while (1) {
		struct console_input *input;
		input = k_fifo_get(&avail_lines, K_FOREVER);
		/* Valid SHUB data: "SHUB: T XX H XX" */
		if ((strlen(input->line) == 15) &&
				(strncmp("SHUB: T ", input->line, 8) == 0)) {
			/* Assume valid data from here on */
			input->line[10] = '\0';
			shub_temp = atoi(input->line + 8);
			shub_humidity = atoi(input->line + 13);
			OTA_DBG("temp: %d, humidity: %d\n",
					shub_temp, shub_humidity);
		} else {
			OTA_DBG("invalid data %s\n", input->line);
		}
		k_fifo_put(&avail_queue, input);
	}
}
#endif /* CONFIG_CONSOLE_HANDLER */

void sensorhub_init(void)
{
	shub_temp = 0;
	shub_humidity = 0;

#if defined(CONFIG_CONSOLE_HANDLER)
	size_t i;

	k_fifo_init(&avail_queue);
	k_fifo_init(&avail_lines);

	/* Make buffers available */
	for (i = 0; i < MAX_SHUB_UART_LINES; i++) {
		k_fifo_put(&avail_queue, &buf[i]);
	}

	k_thread_spawn(stack, STACKSIZE, sensorhub, NULL, NULL, NULL,
		       K_PRIO_COOP(7), 0, K_NO_WAIT);
	uart_register_input(&avail_queue, &avail_lines, NULL);
#endif /* CONFIG_CONSOLE_HANDLER */
}
