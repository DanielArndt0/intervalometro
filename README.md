# Intervalômetro com Arduino Nano

Intervalômetro para câmeras DSLR com Arduino Nano (ATmega328P), display OLED SSD1306 de 128×64 pixels e intervalo ajustável de 1 a 120 segundos. A tela mostra o tempo decorrido e o número de disparos.

## Conexões

- **A0:** potenciômetro para ajustar o intervalo.
- **D2:** botão de confirmação (ativo em HIGH, com resistor pull-down).
- **D3:** sinal de disparo (pulso HIGH de 2 ms).
- **I²C:** display OLED SSD1306, endereço `0x3C`.

## Como usar

Abra `CameraIntervalometer/CameraIntervalometer.ino` na Arduino IDE, instale as bibliotecas **Adafruit GFX** e **Adafruit SSD1306** e selecione a placa **Arduino Nano (ATmega328P)**. Escolha o intervalo no potenciômetro e confirme com o botão.

Para conectar a saída D3 à câmera, utilize uma interface isolada, como um optoacoplador. Não aplique 5 V diretamente ao conector de disparo da câmera.

