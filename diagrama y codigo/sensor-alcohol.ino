#include "Wire.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

// Pantalla OLED con dirección TWI
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(-1); // -1 = no reset pin
int valor_limite = 500;       // Fija el valor límite en el que se activa la alarma
float valor_alcohol;

const int pinNivelAlto = 8;
const int pinNivelMedio = 9;
const int pinNivelBajo = 10;
const int pinNivelMuyBajo = 11;

void setup() {
  // Inicializa y borra la pantalla OLED
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  // Muestra una línea de texto
  display.setTextSize(1.3);
  display.setTextColor(WHITE);

  // Inicializa el puerto serial
  Serial.begin(9600);

  // Configura los pines como salidas
  pinMode(pinNivelAlto, OUTPUT);
  pinMode(pinNivelMedio, OUTPUT);
  pinMode(pinNivelBajo, OUTPUT);
  pinMode(pinNivelMuyBajo, OUTPUT);

  pinMode(13, OUTPUT); // Configura el Pin 13 como salida para el Zumbador
}

void loop() {
  valor_alcohol = analogRead(A0);
  float porcentaje = (valor_alcohol / 10000); // Calcula el porcentaje

  // Limpia la línea anterior y muestra el nuevo valor del alcohol
  display.fillRect(0, 0, 128, 16, BLACK); // Borra la línea anterior del valor del alcohol
  display.setCursor(1, 0);
  display.print("Alcohol: ");
  display.print(valor_alcohol);

  // Limpia la línea anterior y muestra el nuevo porcentaje
  display.fillRect(0, 10, 128, 16, BLACK); // Borra la línea anterior del porcentaje
  display.setCursor(1, 10);
  display.print("Porcentaje: ");
  display.print(porcentaje * 100, 2); // Muestra el porcentaje en la pantalla
  display.print("%");
  display.display(); // Actualiza la pantalla

  Serial.print("Alcohol: ");
  Serial.println(valor_alcohol);
  Serial.print("Porcentaje: ");
  Serial.print(porcentaje * 100, 2);
  Serial.println("%");

  // Control de los LEDs en función del valor de alcohol
  if (valor_alcohol > valor_limite) {
    digitalWrite(pinNivelAlto, HIGH);
    digitalWrite(pinNivelMedio, LOW);
    digitalWrite(pinNivelBajo, LOW);
    digitalWrite(pinNivelMuyBajo, LOW);
    digitalWrite(13, HIGH); // Enciende el Zumbador conectado al Pin 13
  } else if (valor_alcohol > 370) {
    digitalWrite(pinNivelAlto, LOW);
    digitalWrite(pinNivelMedio, HIGH);
    digitalWrite(pinNivelBajo, LOW);
    digitalWrite(pinNivelMuyBajo, LOW);
    digitalWrite(13, LOW); // Apaga el Zumbador
  } else if (valor_alcohol > 340) {
    digitalWrite(pinNivelAlto, LOW);
    digitalWrite(pinNivelMedio, LOW);
    digitalWrite(pinNivelBajo, LOW);
    digitalWrite(pinNivelMuyBajo, HIGH);
    digitalWrite(13, LOW); // Apaga el Zumbador
  } else if (valor_alcohol < 340) {
    digitalWrite(pinNivelAlto, LOW);
    digitalWrite(pinNivelMedio, LOW);
    digitalWrite(pinNivelBajo, HIGH);
    digitalWrite(pinNivelMuyBajo, LOW);
    digitalWrite(13, LOW); // Apaga el Zumbador
  } else {
    digitalWrite(pinNivelAlto, LOW);
    digitalWrite(pinNivelMedio, LOW);
    digitalWrite(pinNivelBajo, LOW);
    digitalWrite(pinNivelMuyBajo, LOW);
    digitalWrite(13, LOW); // Apaga el Zumbador
  }

  delay(400); // Espera 400ms para realizar la próxima medida
}
