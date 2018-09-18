#include <asf.h>
#include <board.h>
#include <gpio.h>
#include <sysclk.h>
#include "busy_delay.h"

#define CONFIG_USART_IF (AVR32_USART2)

// defines for BRTT interface
#define TEST_A      AVR32_PIN_PA31
#define RESPONSE_A  AVR32_PIN_PA30
#define TEST_B      AVR32_PIN_PA29
#define RESPONSE_B  AVR32_PIN_PA28
#define TEST_C      AVR32_PIN_PA27
#define RESPONSE_C  AVR32_PIN_PB00

void task_A (void);
void task_B (void);
void task_C (void);
void task_D (void);
void task_C_interrupt (void);
void task_D_interrupt (void);
void task_D_do_interrupt_tasks(void);

__attribute__((__interrupt__)) static void interrupt_J3(void);

void init(){
    sysclk_init();
    board_init();
    busy_delay_init(BOARD_OSC0_HZ);
    
    cpu_irq_disable();
    INTC_init_interrupts();
    INTC_register_interrupt(&interrupt_J3, AVR32_GPIO_IRQ_3, AVR32_INTC_INT1);
    cpu_irq_enable();
    
    stdio_usb_init(&CONFIG_USART_IF);

    #if defined(__GNUC__) && defined(__AVR32__)
        setbuf(stdout, NULL);
        setbuf(stdin,  NULL);
    #endif
}

__attribute__((__interrupt__)) static void interrupt_J3(void){ 
	task_D_interrupt();
}

int main (void){
	init();
	task_D();
}


void task_A () {
	gpio_configure_pin(TEST_A, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(RESPONSE_A, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
		
	while(1){
		// wait until testA = 0
		while(gpio_pin_is_high(TEST_A))
		;
			
		// responseA = 0
		gpio_set_pin_low(RESPONSE_A);
			
		// wait 5us
		busy_delay_us(5);
			
		// response = 1
		gpio_set_pin_high(RESPONSE_A);
	}
}

void task_B () {
	gpio_configure_pin(TEST_A, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(TEST_B, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(TEST_C, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	
	gpio_configure_pin(RESPONSE_A, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(RESPONSE_B, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(RESPONSE_C, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	
	while(1){
		if(gpio_pin_is_low(TEST_A)) {
			gpio_set_pin_low(RESPONSE_A);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_A);
		}
		
		if(gpio_pin_is_low(TEST_B)) {
			gpio_set_pin_low(RESPONSE_B);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_B);
		}
		
		if(gpio_pin_is_low(TEST_C)) {
			gpio_set_pin_low(RESPONSE_C);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_C);
		}
	}
}

void task_C_interrupt () {
	if (gpio_get_pin_interrupt_flag(TEST_A)) {
		gpio_clear_pin_interrupt_flag(TEST_A);
		gpio_set_pin_low(RESPONSE_A);
		busy_delay_us(5);
		gpio_set_pin_high(RESPONSE_A);
	}
	if (gpio_get_pin_interrupt_flag(TEST_B)) {
		gpio_clear_pin_interrupt_flag(TEST_B);
		busy_delay_us(100);
		gpio_set_pin_low(RESPONSE_B);
		busy_delay_us(5);
		gpio_set_pin_high(RESPONSE_B);
	}
	if (gpio_get_pin_interrupt_flag(TEST_C)) {
		gpio_clear_pin_interrupt_flag(TEST_C);
		gpio_set_pin_low(RESPONSE_C);
		busy_delay_us(5);
		gpio_set_pin_high(RESPONSE_C);
	}
}

void task_C () {
	gpio_configure_pin(TEST_A, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(TEST_B, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(TEST_C, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	
	gpio_configure_pin(RESPONSE_A, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(RESPONSE_B, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(RESPONSE_C, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	
	gpio_enable_pin_interrupt(TEST_A, GPIO_FALLING_EDGE);
	gpio_enable_pin_interrupt(TEST_B, GPIO_FALLING_EDGE);
	gpio_enable_pin_interrupt(TEST_C, GPIO_FALLING_EDGE);
	
	while(1)
		;
}

volatile int task_D_interrupt_flag_A = 0;
volatile int task_D_interrupt_flag_B = 0;
volatile int task_D_interrupt_flag_C = 0;

void task_D_interrupt () {
	if (gpio_get_pin_interrupt_flag(TEST_A)) {
		gpio_clear_pin_interrupt_flag(TEST_A);
		task_D_interrupt_flag_A++;
	}
	
	if (gpio_get_pin_interrupt_flag(TEST_B)) {
		gpio_clear_pin_interrupt_flag(TEST_B);
		task_D_interrupt_flag_B++;
	}
	
	if (gpio_get_pin_interrupt_flag(TEST_C)) {
		gpio_clear_pin_interrupt_flag(TEST_C);
		task_D_interrupt_flag_C++;
	}
}

void task_D_do_interrupt_tasks() {
	if (task_D_interrupt_flag_A) {
		gpio_set_pin_low(RESPONSE_A);
		busy_delay_us(5);
		gpio_set_pin_high(RESPONSE_A);
		task_D_interrupt_flag_A = 0;
	}
	
	if (task_D_interrupt_flag_B) {
		busy_delay_us(100);
		gpio_set_pin_low(RESPONSE_B);
		busy_delay_us(5);
		gpio_set_pin_high(RESPONSE_B);
		task_D_interrupt_flag_B = 0;
	}
	
	if (task_D_interrupt_flag_C) {
		gpio_set_pin_low(RESPONSE_C);
		busy_delay_us(5);
		gpio_set_pin_high(RESPONSE_C);
		task_D_interrupt_flag_C = 0;
	}
	
}

void task_D () {
	gpio_configure_pin(TEST_A, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(TEST_B, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(TEST_C, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	
	gpio_configure_pin(RESPONSE_A, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(RESPONSE_B, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(RESPONSE_C, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	
	gpio_enable_pin_interrupt(TEST_A, GPIO_FALLING_EDGE);
	gpio_enable_pin_interrupt(TEST_B, GPIO_FALLING_EDGE);
	gpio_enable_pin_interrupt(TEST_C, GPIO_FALLING_EDGE);
	
	while(1) {
		if (task_D_interrupt_flag_A) {
			gpio_set_pin_low(RESPONSE_A);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_A);
			task_D_interrupt_flag_A = 0;
		}
		
		if (task_D_interrupt_flag_B) {
			busy_delay_us(100);
			gpio_set_pin_low(RESPONSE_B);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_B);
			task_D_interrupt_flag_B = 0;
		}
		
		if (task_D_interrupt_flag_C) {
			gpio_set_pin_low(RESPONSE_C);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_C);
			task_D_interrupt_flag_C = 0;
		}
	}
}
