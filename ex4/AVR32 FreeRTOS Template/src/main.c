#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gpio.h>
#include <power_clocks_lib.h>
#include <sysclk.h>
#include <compiler.h>
#include <board.h>
#include <stdio_usb.h>


#define CONFIG_USART_IF (AVR32_USART2)

#include "FreeRTOS.h"
#include "task.h"

#define TEST_A      AVR32_PIN_PA31
#define RESPONSE_A  AVR32_PIN_PA30
#define TEST_B      AVR32_PIN_PA29
#define RESPONSE_B  AVR32_PIN_PA28
#define TEST_C      AVR32_PIN_PA27
#define RESPONSE_C  AVR32_PIN_PB00


void busy_delay_ms(int delay){
    for(; delay != 0; delay--){
        for(int i = 0; i < 2108; i++){
            asm volatile ("" ::: "memory");
        }
    }
}

void busy_delay_short(void){
    for(int i = 0; i < 10; i++){
        asm volatile ("" ::: "memory");
    }
}

void init(){
	board_init();
	
    gpio_configure_pin(TEST_A, GPIO_DIR_INPUT);
    gpio_configure_pin(TEST_B, GPIO_DIR_INPUT);
    gpio_configure_pin(TEST_C, GPIO_DIR_INPUT);
    gpio_configure_pin(RESPONSE_A, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
    gpio_configure_pin(RESPONSE_B, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
    gpio_configure_pin(RESPONSE_C, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);

	pcl_switch_to_osc(PCL_OSC0, FOSC0, OSC0_STARTUP);
	
	stdio_usb_init(&CONFIG_USART_IF);

    #if defined(__GNUC__) && defined(__AVR32__)
	    setbuf(stdout, NULL);
	    setbuf(stdin,  NULL);
    #endif
}

static void taskFn(void* args){
	const portTickType delay = 1000 / portTICK_RATE_MS;
	
    int iter = 0;

	while(1){
		gpio_toggle_pin(LED0_GPIO);
		printf("tick %d\n", iter++);
		
		vTaskDelay(delay);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TASK A
static void led0_blink(void* args) {
	for (;;) {
		gpio_toggle_pin(LED0_GPIO);
		vTaskDelay(200 /* ms */);
	}
}
static void led1_blink(void* args) {
	for (;;) {
		gpio_toggle_pin(LED1_GPIO);
		vTaskDelay(500 /* ms */);
	}
}

static inline void task_A() {
	xTaskCreate(led0_blink, "led0 200 ms blink", 1024, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(led1_blink, "led1 500 ms blink", 1024, NULL, tskIDLE_PRIORITY + 1, NULL);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TASK B
struct responseTaskArgs {
	uint32_t test;
	uint32_t response;
};

static void b_responseTask(void* args) {
	struct responseTaskArgs pin = *(struct responseTaskArgs*) args;
	
	gpio_configure_pin(pin.test, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(pin.response, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	
	for(;;) {
		if (gpio_pin_is_low(pin.test)) {
			gpio_set_pin_low(pin.response);
			vTaskDelay(1);
			gpio_set_pin_high(pin.response);
		}
	}
}

static inline void task_B() {
	xTaskCreate(b_responseTask, "test A", 1024, (&(struct responseTaskArgs){ TEST_A, RESPONSE_A }), tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(b_responseTask, "test B", 1024, (&(struct responseTaskArgs){ TEST_B, RESPONSE_B }), tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(b_responseTask, "test C", 1024, (&(struct responseTaskArgs){ TEST_C, RESPONSE_C }), tskIDLE_PRIORITY + 1, NULL);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TASK C
static void c_responseTask(void* args) {
	struct responseTaskArgs pin = *(struct responseTaskArgs*) args;
	
	gpio_configure_pin(pin.test, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(pin.response, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	
	for(;;) {
		if (gpio_pin_is_low(pin.test)) {
			if (pin.test == TEST_C) busy_delay_ms(3);
			gpio_set_pin_low(pin.response);
			vTaskDelay(1);
			gpio_set_pin_high(pin.response);
		}
	}
}
static inline void task_C() {
	xTaskCreate(c_responseTask, "test A", 1024, (&(struct responseTaskArgs){ TEST_A, RESPONSE_A }), tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(c_responseTask, "test B", 1024, (&(struct responseTaskArgs){ TEST_B, RESPONSE_B }), tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(c_responseTask, "test C", 1024, (&(struct responseTaskArgs){ TEST_C, RESPONSE_C }), tskIDLE_PRIORITY + 1, NULL);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TASK D

static void d_responseTask(void* args) {
	struct responseTaskArgs pin = *(struct responseTaskArgs*) args;
	
	gpio_configure_pin(pin.test, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(pin.response, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	
	for(;;) {
		if (gpio_pin_is_low(pin.test)) {
			if (pin.test == TEST_C) busy_delay_ms(3);
			gpio_set_pin_low(pin.response);
			vTaskDelay(1);
			gpio_set_pin_high(pin.response);
		} else {
			vTaskDelay(1);
		}
	}
}

static inline void task_D() {
	xTaskCreate(d_responseTask, "test A", 1024, (&(struct responseTaskArgs){ TEST_A, RESPONSE_A }), tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(d_responseTask, "test B", 1024, (&(struct responseTaskArgs){ TEST_B, RESPONSE_B }), tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(d_responseTask, "test C", 1024, (&(struct responseTaskArgs){ TEST_C, RESPONSE_C }), tskIDLE_PRIORITY + 1, NULL);
}

static inline void task_E() {
	xTaskCreate(d_responseTask, "test A", 1024, (&(struct responseTaskArgs){ TEST_A, RESPONSE_A }), tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(d_responseTask, "test B", 1024, (&(struct responseTaskArgs){ TEST_B, RESPONSE_B }), tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(d_responseTask, "test C", 1024, (&(struct responseTaskArgs){ TEST_C, RESPONSE_C }), tskIDLE_PRIORITY + 1, NULL);
}

int main(){
	init();
	task_E();
	vTaskStartScheduler();
}

