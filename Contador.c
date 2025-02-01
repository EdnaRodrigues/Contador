#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"

//Arquivo .pio
#include "Contador.pio.h"

//Número de LEDs
#define NUM_PIXELS 25

//Pino de saída
#define OUT_PIN 7
#define pisca_led 13

//Botões de interupção
#define button_A 5
#define button_B 6
#define button_Joy 22

//Rotina para interrupção
static void gpio_irq_handler(uint gpio, uint32_t events) {
    printf("Interrupção ocorreu no pino %d, no evento %d\n", gpio, events);
    printf("HABILITANDO O MODO GRAVAÇÃO");
	reset_usb_boot(0,0); //Habilita o modo de gravação do microcontrolador
}

void inicializa() {
    PIO pio = pio0; 
    bool ok;
    double r = 0.0, b = 0.0 , g = 0.0;

    //Coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
    ok = set_sys_clock_khz(128000, false);

    // Inicializa todos os códigos stdio padrão que estão ligados ao binário.
    stdio_init_all();

    //Configurações da PIO
    uint offset = pio_add_program(pio, &Contador_program);
    uint sm = pio_claim_unused_sm(pio, true);
    Contador_program_init(pio, sm, offset, OUT_PIN);

    //Inicializar o LED que irá piscar continuamente
    gpio_init(pisca_led);
    gpio_set_dir(pisca_led, GPIO_OUT);

    //Inicializar o botão de interrupção para incrementar
    gpio_init(button_A);
    gpio_set_dir(button_A, GPIO_IN);
    gpio_pull_up(button_A);

    //Inicializar o botão de interrupção para decrementar
    gpio_init(button_B);
    gpio_set_dir(button_B, GPIO_IN);
    gpio_pull_up(button_B);

    //Inicializar o botão de interrupção para reboot
    gpio_init(button_Joy);
    gpio_set_dir(button_Joy, GPIO_IN);
    gpio_pull_up(button_Joy);

    //Interrupções da gpio habilitadas
    gpio_set_irq_enabled_with_callback(button_A, GPIO_IRQ_EDGE_FALL, true, & gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_B, GPIO_IRQ_EDGE_FALL, true, & gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_Joy, GPIO_IRQ_EDGE_FALL, true, & gpio_irq_handler);
}

uint32_t matrix_rgb(double b, double r, double g) {
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

int main() {
    
    inicializa(); //

    while (true) {
        gpio_put(pisca_led, true);
        sleep_ms(100);
        gpio_put(pisca_led, false);
        sleep_ms(100);
    }
}
