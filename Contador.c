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

//Variáveis Globais para permitir a função de saída da matriz de LEDs
PIO pio;
uint sm;
double r = 1, b = 1, g = 1;
int i;
static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)

double numeros[10][25] = {
    { // Número 0
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
        1, 1, 1, 1, 1
    },
    { // Número 1
        0, 0, 1, 0, 0,
        0, 1, 1, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 1, 0, 0,
        0, 1, 1, 1, 0
    },
    { // Número 2
        1, 1, 1, 1, 1,
        0, 0, 0, 0, 1,
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 0,
        1, 1, 1, 1, 1
    },
    { // Número 3
        1, 1, 1, 1, 1,
        0, 0, 0, 0, 1,
        0, 1, 1, 1, 1,
        0, 0, 0, 0, 1,
        1, 1, 1, 1, 1
    },
    { // Número 4
        1, 0, 0, 1, 0,
        1, 0, 0, 1, 0,
        1, 1, 1, 1, 1,
        0, 0, 0, 1, 0,
        0, 0, 0, 1, 0
    },
    { // Número 5
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 0,
        1, 1, 1, 1, 1,
        0, 0, 0, 0, 1,
        1, 1, 1, 1, 1
    },
    { // Número 6
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 0,
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 1,
        1, 1, 1, 1, 1
    },
    { // Número 7
        1, 1, 1, 1, 1,
        0, 0, 0, 0, 1,
        0, 0, 0, 1, 0,
        0, 0, 1, 0, 0,
        0, 1, 0, 0, 0
    },
    { // Número 8
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 1,
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 1,
        1, 1, 1, 1, 1
    },
    { // Número 9
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 1,
        1, 1, 1, 1, 1,
        0, 0, 0, 0, 1,
        1, 1, 1, 1, 1
    }
};

//Rotina para interrupção
void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - last_time > 150000) { //Apenas ativa as funções quando o intervalo entre acionamentos é superior a 0.15 segundos
        last_time = current_time; //Atualiza o tempo do último evento

        if (gpio == button_A) {
            if (i < 9 && i >= 0) {
                i++;
                desenho (i);
            }
        } else if (gpio == button_B) {
            if (i <= 9 && i > 0) {
                i--;
                desenho (i);
            }
            
        } else if (gpio == button_Joy) {
            reset_usb_boot(0,0); //Habilita o modo de gravação do microcontrolador
        } else {
            // Não faz nada
        }   
    }
}

uint32_t matrix_rgb (double r, double g, double b) {
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

void desenho (int n) {
    uint32_t valor_led;
    double *desenho = numeros[n];// Obtém o vetor de 25 elementos do número desejado

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(r*desenho[24-i], g*desenho[24-i], b*desenho[24-i]); // LED apagado para os espaços vazios
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

void inicializa() {
    pio = pio0; 
    bool ok;
    i = 0;

    //Coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
    ok = set_sys_clock_khz(128000, false);

    // Inicializa todos os códigos stdio padrão que estão ligados ao binário.
    stdio_init_all();

    //Configurações da PIO
    uint offset = pio_add_program(pio, &Contador_program);
    sm = pio_claim_unused_sm(pio, true);
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

    desenho(0);
}

int main() {
    
    inicializa(); //Inicializa variáveis, pio, botões, saídas e interrupções

    while (true) { //Rotina que faz com que o LED pisque 5 vezes por segundo
        gpio_put(pisca_led, true);
        sleep_ms(100);
        gpio_put(pisca_led, false);
        sleep_ms(100);
    }
}
