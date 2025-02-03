# Projeto: Controle de LEDs e Botões com Interrupções no RP2040

Este projeto foi desenvolvido como parte de uma tarefa prática para consolidar o entendimento sobre o uso de interrupções no microcontrolador RP2040, utilizando a placa de desenvolvimento BitDogLab. O objetivo é controlar LEDs comuns e LEDs WS2812, além de implementar debouncing via software para botões.

## Descrição do Projeto

O projeto consiste em:
1. **LED RGB**: O LED vermelho pisca 5 vezes por segundo.
2. **Botão A**: Incrementa o número exibido na matriz de LEDs WS2812.
3. **Botão B**: Decrementa o número exibido na matriz de LEDs WS2812.
4. **Matriz de LEDs WS2812**: Exibe números de 0 a 9 com efeitos visuais.

## Componentes Utilizados
- Matriz 5x5 de LEDs WS2812 (conectada à GPIO 7).
- LED RGB (conectado às GPIOs 11, 12 e 13).
- Botão A (conectado à GPIO 5).
- Botão B (conectado à GPIO 6).

## Funcionalidades Implementadas
- Uso de interrupções para tratar eventos dos botões.
- Debouncing via software para evitar leituras incorretas dos botões.
- Controle de LEDs comuns (RGB) e LEDs endereçáveis (WS2812).
- Exibição de números na matriz de LEDs com padrões pré-definidos.

## Estrutura do Código
O projeto está organizado nos seguintes arquivos:
- **main.c**: Contém a lógica principal do projeto.
- **ws2812.c**: Implementa o controle da matriz de LEDs WS2812.
- **ws2812.h**: Cabeçalho para o controle da matriz de LEDs.
- **CMakeLists.txt**: Configuração do projeto para compilação com CMake.
- **ws2812.pio**: Código PIO para controle dos LEDs WS2812.

## Video de demonstração
- https://www.youtube.com/watch?v=27_01_Tarefa_Interrupcoes_RP2040
