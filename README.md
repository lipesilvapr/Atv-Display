# Projeto de Controle de LEDs e Display com RP2040

Este projeto foi desenvolvido para consolidar os conceitos de comunicação serial e controle de hardware utilizando o microcontrolador RP2040. Ele inclui a manipulação de LEDs RGB, uma matriz de LEDs WS2812, um display OLED SSD1306 e botões de interação. O código foi escrito em C e utiliza as bibliotecas do SDK do Raspberry Pi Pico.

## Funcionalidades Principais

### Display OLED SSD1306:
- Exibe caracteres digitados pelo usuário via UART.
- Mostra mensagens informativas sobre o estado dos LEDs RGB.

### Matriz de LEDs WS2812:
- Exibe números de 0 a 9 quando o usuário digita um número no Serial Monitor.
- Permite ligar e desligar a matriz de LEDs com comandos específicos.

### LEDs RGB:
- O LED verde é controlado pelo Botão A.
- O LED azul é controlado pelo Botão B.
- O estado dos LEDs é exibido no display OLED e no Serial Monitor.

### Botões de Interação:
- **Botão A:** Alterna o estado do LED verde.
- **Botão B:** Alterna o estado do LED azul.
- As interrupções são usadas para detectar os pressionamentos dos botões.

### Comunicação UART:
- Permite a entrada de caracteres via Serial Monitor.

## Como Usar

### Pré-requisitos
- Placa de desenvolvimento BitDogLab com RP2040.
- Ambiente de desenvolvimento configurado com o SDK do Raspberry Pi Pico.



### Compilação e Upload
1. Clone o repositório do projeto.
2. Compile o código e faça o upload para a placa BitDogLab.

### Uso do Projeto
1. Abra o Serial Monitor no VS code.
2. Digite um caractere ou número:
   - **Números de 0 a 9:** Exibe o número na matriz de LEDs e no display OLED.
   - **Outros caracteres:** Exibe o caractere no display OLED.
3. Pressione os botões:
   - **Botão A:** Alterna o estado do LED verde.
   - **Botão B:** Alterna o estado do LED azul.

## Estrutura do Projeto

- **`displayAtv.c`:**
  Contém o código principal do projeto. Gerencia a comunicação UART, interrupções dos botões e controle dos LEDs e display.

- **`matriz.h` e `matriz.c`:**
  Contém as definições dos desenhos dos números na matriz de LEDs WS2812.

- **`ssd1306.h` e `ssd1306.c`:**
  Biblioteca para controlar o display OLED SSD1306.

- **`font.h`:**
  Contém a definição da fonte usada para exibir caracteres no display OLED.


## Vídeo de Demonstração

Um vídeo de demonstração do projeto em funcionamento pode ser encontrado aqui.
https://drive.google.com/file/d/1eu5g_LfjjEi63QBsW5S9Pecan9ycE_pt/view?usp=sharing

