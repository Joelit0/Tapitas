#include <string.h>
#include "COLOR.h"
#define CALIBRAR 0
#define CLASIFICAR 1

int wait = 5;
int btn_pin = 12;
int state = CALIBRAR;
String leeCadena;

int pin_1_PAP1 = 11;
int pin_2_PAP1 = 10;
int pin_3_PAP1 = 9;
int pin_4_PAP1 = 8;
int dato_rx_PAP1;
int numero_pasos_PAP1 = 0;
int valor_pp_PAP1;
int potenciometro_PAP1 = 0;
int valor_potenciometro_PAP1;

int pin_1_PAP2 = 7;
int pin_2_PAP2 = 6;
int pin_3_PAP2 = 5;
int pin_4_PAP2 = 4;
int dato_rx_PAP2;
int numero_pasos_PAP2 = 0;
int valor_pp_PAP2;
int potenciometro_PAP2 = 1;
int valor_potenciometro_PAP2;

void setup() {
  Serial.begin(9600);
  COLOR_init();

  pinMode(pin_1_PAP1, OUTPUT);    // Pin 11 conectar a IN4
  pinMode(pin_2_PAP1, OUTPUT);    // Pin 10 conectar a IN3
  pinMode(pin_3_PAP1, OUTPUT);     // Pin 9 conectar a IN2
  pinMode(pin_4_PAP1, OUTPUT);     // Pin 8 conectar a IN1

  pinMode(pin_1_PAP2, OUTPUT);    // Pin 7 conectar a IN4
  pinMode(pin_2_PAP2, OUTPUT);    // Pin 6 conectar a IN3
  pinMode(pin_3_PAP2, OUTPUT);     // Pin 5 conectar a IN2
  pinMode(pin_4_PAP2, OUTPUT);     // Pin 4 conectar a IN1

  pinMode(btn_pin, INPUT);
}

void loop() {
  actualizar_btn();
  Serial. println(state);

  if (state == CALIBRAR) {
    valor_potenciometro_PAP1 = map(analogRead(potenciometro_PAP1), 0, 1023, 0, 360);

    if (valor_potenciometro_PAP1 != dato_rx_PAP1) {
      dato_rx_PAP1 = (valor_potenciometro_PAP1 * 1.4222222222);

      while (dato_rx_PAP1 > numero_pasos_PAP1) {
        paso_izq(pin_1_PAP1, pin_2_PAP1, pin_3_PAP1, pin_4_PAP1);
        numero_pasos_PAP1 = numero_pasos_PAP1 + 1;
      }

      while (dato_rx_PAP1 < numero_pasos_PAP1) {
        paso_der(pin_1_PAP1, pin_2_PAP1, pin_3_PAP1, pin_4_PAP1);
        numero_pasos_PAP1 = numero_pasos_PAP1 - 1;
      }
    }

    apagado(pin_1_PAP1, pin_2_PAP1, pin_3_PAP1, pin_4_PAP1);

    valor_potenciometro_PAP2 = map(analogRead(potenciometro_PAP2), 0, 1023, 0, 360);

    if (valor_potenciometro_PAP2 != dato_rx_PAP2) {
      dato_rx_PAP2 = (valor_potenciometro_PAP2 * 1.4222222222);

      while (dato_rx_PAP2 > numero_pasos_PAP2) {
        paso_izq(pin_1_PAP2, pin_2_PAP2, pin_3_PAP2, pin_4_PAP2);
        numero_pasos_PAP2 = numero_pasos_PAP2 + 1;
      }

      while (dato_rx_PAP2 < numero_pasos_PAP2) {
        paso_der(pin_1_PAP2, pin_2_PAP2, pin_3_PAP2, pin_4_PAP2);
        numero_pasos_PAP2 = numero_pasos_PAP2 - 1;
      }

      apagado(pin_1_PAP2, pin_2_PAP2, pin_3_PAP2, pin_4_PAP2);
    }
    
    delay(800);
  } else if (state == CLASIFICAR) {
    sum_90();
    delay(1000);
  
    RGB_t c = COLOR_getRGB();
    Serial.println (COLOR_getColorName());
  
    while (Serial.available()) {
      char c  = Serial.read();
      leeCadena += c;
    }
  
    if (leeCadena.length()>0){
      dato_rx_PAP1 = leeCadena.toInt();
      Serial.print(dato_rx_PAP1);
      Serial.println("Grados");
      delay(wait);
      dato_rx_PAP1 = (dato_rx_PAP1 * 1.4222222222);
    }
  
    while (dato_rx_PAP1 > numero_pasos_PAP1){
      paso_izq(pin_1_PAP1, pin_2_PAP1, pin_3_PAP1, pin_4_PAP1);
      numero_pasos_PAP1 = numero_pasos_PAP1 + 1;
    }
  
    while (dato_rx_PAP1 < numero_pasos_PAP1){
      paso_der(pin_1_PAP1, pin_2_PAP1, pin_3_PAP1, pin_4_PAP1);
      numero_pasos_PAP1 = numero_pasos_PAP1 - 1;
    }
  
    leeCadena = "";
    apagado(pin_1_PAP1, pin_2_PAP1, pin_3_PAP1, pin_4_PAP1);
  }
}

void actualizar_btn() {
  if (digitalRead(btn_pin) == HIGH && state == CALIBRAR) {
    state = CLASIFICAR;
  }
}

void sum_90() {
  dato_rx_PAP2 += 90;
  delay(wait);
  valor_pp_PAP2 = (dato_rx_PAP2 * 1.4222222222);

  while (valor_pp_PAP2 > numero_pasos_PAP2) {
    paso_izq(pin_1_PAP2, pin_2_PAP2, pin_3_PAP2, pin_4_PAP2);
    numero_pasos_PAP2 = numero_pasos_PAP2 + 1;
  }

  while (valor_pp_PAP2 < numero_pasos_PAP2) {
    paso_der(pin_1_PAP2, pin_2_PAP2, pin_3_PAP2, pin_4_PAP2);
    numero_pasos_PAP2 = numero_pasos_PAP2 - 1;
  }

  apagado(pin_1_PAP2, pin_2_PAP2, pin_3_PAP2, pin_4_PAP2);
}

void paso_der(int pin_1, int pin_2, int pin_3, int pin_4) {
  digitalWrite(pin_1, LOW);
  digitalWrite(pin_2, LOW);
  digitalWrite(pin_3, HIGH);
  digitalWrite(pin_4, HIGH);
  delay(wait);
  digitalWrite(pin_1, LOW);
  digitalWrite(pin_2, HIGH);
  digitalWrite(pin_3, HIGH);
  digitalWrite(pin_4, LOW);
  delay(wait);
  digitalWrite(pin_1, HIGH);
  digitalWrite(pin_2, HIGH);
  digitalWrite(pin_3, LOW);
  digitalWrite(pin_4, LOW);
  delay(wait);
  digitalWrite(pin_1, HIGH);
  digitalWrite(pin_2, LOW);
  digitalWrite(pin_3, LOW);
  digitalWrite(pin_4, HIGH);
  delay(wait);
}

void paso_izq(int pin_1, int pin_2, int pin_3, int pin_4) {
  digitalWrite(pin_1, HIGH);
  digitalWrite(pin_2, HIGH);
  digitalWrite(pin_3, LOW);
  digitalWrite(pin_4, LOW);
  delay(wait);
  digitalWrite(pin_1, LOW);
  digitalWrite(pin_2, HIGH);
  digitalWrite(pin_3, HIGH);
  digitalWrite(pin_4, LOW);
  delay(wait);
  digitalWrite(pin_1, LOW);
  digitalWrite(pin_2, LOW);
  digitalWrite(pin_3, HIGH);
  digitalWrite(pin_4, HIGH);
  delay(wait);
  digitalWrite(pin_1, HIGH);
  digitalWrite(pin_2, LOW);
  digitalWrite(pin_3, LOW);
  digitalWrite(pin_4, HIGH);
  delay(wait);
}

void apagado(int pin_1, int pin_2, int pin_3, int pin_4) {
  digitalWrite(pin_1, LOW);
  digitalWrite(pin_2, LOW);
  digitalWrite(pin_3, LOW);
  digitalWrite(pin_4, LOW);
}
