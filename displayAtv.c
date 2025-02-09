#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "include/ssd1306.h"
#include "include/font.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "ws2812.pio.h"
#include "hardware/pio.h"
#include "include/matriz.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO_OLED 0x3C

#define LED_VERDE 11
#define LED_AZUL 12

#define BOTAO_A 5
#define BOTAO_B 6

#define NUM_LEDS 25
#define PIN_MATRIZ 7

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

void animar_matriz(int numero, PIO pio, uint sm) {
    if (numero >= 1 && numero <= 9) {
        double* desenho = desenhos[numero - 1];
        desenho_pio(desenho, NUM_LEDS, pio, sm, 0.3, 0.0, 0.0);
    }
}

static void tratar_interrupcao(uint gpio, uint32_t events);

static volatile uint32_t ultimo_evento = 0;
bool estado_led_verde = false;
bool estado_led_azul = false;

ssd1306_t display;

int main() {
    stdio_init_all();

    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(UART_ID, true);

    PIO pio = pio0;
    sleep_ms(500);
    printf("Iniciando...\n");

    set_sys_clock_khz(128000, false);

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init(&display, WIDTH, HEIGHT, false, ENDERECO_OLED, I2C_PORT);
    ssd1306_config(&display);
    ssd1306_send_data(&display);
    ssd1306_fill(&display, false);
    ssd1306_send_data(&display);

    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_init(LED_AZUL);
    gpio_set_dir(LED_AZUL, GPIO_OUT);

    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);

    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);

    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &tratar_interrupcao);
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &tratar_interrupcao);

    uint offset = pio_add_program(pio, &Atv_Display_program);
    uint sm = pio_claim_unused_sm(pio, true);
    Atv_Display_program_init(pio, sm, offset, PIN_MATRIZ, 800000, false);

    while (true) {
        if (stdio_usb_connected) {
            char c;
            printf("Digite um caractere ou número de 0 a 9: ");
            if (scanf("%c", &c) == 1) {
                printf("Texto recebido %c\n", c);
                ssd1306_fill(&display, false);
                
                if (c >= '0' && c <= '9') {
                    int numero = c - '0';
                    ssd1306_fill(&display, false);
                    char texto[2] = {c, '\0'};
                    ssd1306_draw_string(&display, "MATRIZ ATUALIZADA", 10, 15);
                    ssd1306_draw_string(&display, "NUMERO", 25, 37);
                    ssd1306_draw_string(&display, texto, 40, 50);
                    ssd1306_send_data(&display);
                    desenho_pio(desenhos[numero], 0, pio0, sm, 0.3, 0.0, 0.0);
                } else {
                    ssd1306_fill(&display, false);
                    char texto[2] = {c, '\0'};
                    ssd1306_draw_string(&display, "CARACTERE RECEBIDO", 20, 30);
                    ssd1306_draw_string(&display, texto, 45, 50);
                    ssd1306_send_data(&display);
                }
            }
            sleep_ms(1000);
        }
    }
}

void tratar_interrupcao(uint gpio, uint32_t events) {
    uint32_t tempo_atual = to_us_since_boot(get_absolute_time());
    if (tempo_atual - ultimo_evento > 300000) {
        ultimo_evento = tempo_atual;

        if (gpio == BOTAO_A) {
            estado_led_verde = !estado_led_verde;
            gpio_put(LED_VERDE, estado_led_verde);
            printf("Botão A pressionado\n");
            ssd1306_fill(&display, false);
            ssd1306_draw_string(&display, estado_led_verde ? "LED VERDE: ON" : "LED VERDE: OFF", 10, 30);
            ssd1306_send_data(&display);
        }

        if (gpio == BOTAO_B) {
            estado_led_azul = !estado_led_azul;
            gpio_put(LED_AZUL, estado_led_azul);
            printf("Botão B pressionado\n");
            ssd1306_fill(&display, false);
            ssd1306_draw_string(&display, estado_led_azul ? "LED AZUL: ON" : "LED AZUL: OFF", 10, 30);
            ssd1306_send_data(&display);
        }
    }
}