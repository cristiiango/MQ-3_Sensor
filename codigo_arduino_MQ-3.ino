#include <Wire.h>
#include <U8g2lib.h>

// Inicializa o display OLED
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Pino do sensor MQ-3
const int mq3Pin = A0;

// Variáveis para armazenar o valor do sensor e concentração
int sensorValue;
float alcoholPPM;

// PPM normal medido em condições sem etanol (valor de referência)
const float basePPM = 225;  // Condição normal sem etanol

// Limite de álcool em PPM para determinar intoxicação
const float intoxicationThreshold = 300;  // Ajuste com base na presença de etanol

void setup() {
  // Inicializa o display OLED
  u8g2.begin();
  
  // Inicializa a comunicação serial
  Serial.begin(9600);

  // Mensagem inicial no display
  u8g2.clearBuffer(); 
  u8g2.setFont(u8g2_font_ncenB08_tr); 
  u8g2.drawStr(10, 30, "Iniciando...");
  u8g2.sendBuffer();
  
  delay(2000); // Pausa para exibir a mensagem inicial
}

void loop() {
  // Leitura do valor do sensor MQ-3
  sensorValue = analogRead(mq3Pin);

  // Conversão do valor analógico para uma estimativa de concentração de álcool (em PPM)
  alcoholPPM = map(sensorValue, 0, 1023, 0, 500);  // Mapeia o valor para PPM (ajuste conforme necessário)

  // Subtrai a PPM base (225) para considerar o aumento real da concentração de etanol
  float adjustedPPM = alcoholPPM - basePPM;
  if (adjustedPPM < 0) adjustedPPM = 0;  // Evitar valores negativos

  // Exibe o valor do sensor e a concentração estimada no monitor serial
  Serial.print("Valor do sensor: ");
  Serial.print(sensorValue);
  Serial.print("\tConcentracao de Alcool Ajustada: ");
  Serial.print(adjustedPPM);
  Serial.println(" PPM");

  // Limpa o buffer do display
  u8g2.clearBuffer();

  // Exibe o valor do sensor no display OLED
  u8g2.setFont(u8g2_font_ncenB08_tr);  // Define a fonte
  u8g2.drawStr(0, 20, "SENSOR MQ-3");

  // Exibe o valor analógico do sensor
  u8g2.setCursor(0, 40);
  u8g2.print("Valor: ");
  u8g2.print(sensorValue);

  // Verifica se o nível de álcool ajustado excede o limite definido
  if (adjustedPPM > (intoxicationThreshold - basePPM)) {
    // Exibe uma mensagem dizendo que a pessoa está alcoolizada
    u8g2.setCursor(0, 55);
    u8g2.print("Status: Bebado!");
  } else {
    // Exibe uma mensagem dizendo que a pessoa está sóbria
    u8g2.setCursor(0, 55);
    u8g2.print("Status: Sobrio");
  }

  // Atualiza o display
  u8g2.sendBuffer();

  // Pausa para leitura
  delay(1000);
}
