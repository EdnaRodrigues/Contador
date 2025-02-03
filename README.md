# Matriz 5x5 de LEDs WS2812 com Controle de Números e Interrupções

## 📌 Descrição do Projeto
Este projeto implementa uma matriz 5x5 de LEDs WS2812 controlada por um microcontrolador. A matriz exibe os números de **0 a 9**, que podem ser alterados utilizando botões físicos. Além disso, um LED vermelho extra pisca continuamente **5 vezes por segundo**. O projeto também conta com um **modo de gravação** ativado por um botão do joystick.

---

## 🛠️ Funcionalidades
- **Exibição de números (0 a 9) na matriz de LEDs**.
- **LED vermelho extra piscando a 5 Hz**.
- **Interrupções via botões físicos**:
  - **Botão A**: Incrementa o número exibido na matriz.
  - **Botão B**: Decrementa o número exibido na matriz.
  - **Botão do Joystick**: Ativa o modo de gravação do microcontrolador.
- **Controle via microcontrolador com saída para LEDs WS2812**.

---

## 🔧 Hardware Necessário - Presentes na Placa de Desenvolvimento BitDogLab
- **Microcontrolador Raspberry Pi Pico W**
- **Matriz de LEDs WS2812 (5x5)**
- **LED RGB**
- **Botões físicos**
  - **Botão A** (Incremento)
  - **Botão B** (Decremento)
  - **Botão do Joystick** (Modo de gravação)
- **Fonte de alimentação adequada**

---

## 📝 Código Principal
O código principal:
- **Gerencia a exibição de números na matriz**.
- **Lida com as interrupções dos botões**.
- **Pisca o LED extra continuamente**.

---

## 📌 Como Rodar o Projeto
1. **Faça git clone** deste repositório no Visual Studio Code.
2. **Importe a pasta como projeto Raspberry Pi Pico**.
2. **Compile e grave o código na placa BitDogLab**.
3. **Observe o acionamento dos LEDs** e observe a matriz exibindo os números.
4. **Pressione os botões** para alterar o número exibido.
5. **Pressione o botão do joystick** para ativar o modo de gravação caso queira modificar o arquivo `.uf2` na placa de desenvolvimento.

---

## 📽️ Vídeo de Demonstração

[Assista ao vídeo explicativo aqui!](https://youtu.be/gILFplkfyM4?si=wrwLuL3aTqvUpNtY)

---

## ✉️ Contato
Caso tenha dúvidas ou sugestões, entre em contato com o desenvolvedor.
