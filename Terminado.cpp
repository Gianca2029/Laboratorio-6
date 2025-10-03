//Librerías

#include <Arduino.h>
#include <LiquidCrystal.h>

//Pines

#define Pot_Rojo 34   
#define Pot_Verde 33
#define Led_Rojo   14
#define Led_Verde  27
#define Led_Azul   26

//Señales PWM 

#define Channel_Rojo   0
#define Channel_Verde  1
#define Channel_Azul   2

// Variables globales 

uint16_t valor_Rojo = 0;
uint16_t valor_Verde = 0;
LiquidCrystal lcd(23, 22, 21, 19, 18, 5, 4, 2, 15, 13);

// Ajustados a 8 bits 

uint8_t intensidad_Rojo = 0;  
uint8_t intensidad_Verde = 0;
uint8_t intensidad_Azul = 0; 

void setup() {

  Serial.begin(115200);

  // Inicializar LCD

  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("Rojo Verde Azul");

  //Inicializar Leds

  ledcSetup(Channel_Rojo, 5000, 8);   
  ledcSetup(Channel_Verde, 5000, 8);
  ledcSetup(Channel_Azul, 5000, 8);

  // Asociar cada canal con su pin

  ledcAttachPin(Led_Rojo, Channel_Rojo);
  ledcAttachPin(Led_Verde, Channel_Verde);
  ledcAttachPin(Led_Azul, Channel_Azul);

}

void loop() {

  // Leer los valores del ADC en 12 bits

  valor_Rojo = analogRead(Pot_Rojo);
  valor_Verde = analogRead(Pot_Verde);

  // Ajustarlos a 8 bits 

  intensidad_Rojo = map(valor_Rojo, 0, 4095, 0, 255);
  intensidad_Verde = map(valor_Verde, 0, 4095, 0, 255);

  //Leer el mas o menos

  if (Serial.available() > 0) {
    char maso = Serial.read();

    if (maso == '+') {
      if (intensidad_Azul < 255) {   // Tope mayor
        intensidad_Azul++;
      }
    } 
    else if (maso == '-') {
      if (intensidad_Azul > 0) {     // Tope menor
        intensidad_Azul--;
      }
    }
  }

  // Enviar datos

  Serial.print("Intensidad Rojo: ");
  Serial.println(intensidad_Rojo);
  Serial.print("Intensidad Verde: ");
  Serial.println(intensidad_Verde);
  Serial.print("Intensidad Azul: ");
  Serial.println(intensidad_Azul); 

  ledcWrite(Channel_Rojo, intensidad_Rojo);
  ledcWrite(Channel_Verde, intensidad_Verde);
  ledcWrite(Channel_Azul, intensidad_Azul);

  lcd.setCursor(0, 1);  
  lcd.print(intensidad_Rojo);

  lcd.setCursor(6, 1);  
  lcd.print(intensidad_Verde);

  lcd.setCursor(12, 1);  
  lcd.print(intensidad_Azul);

  delay(500);

}
