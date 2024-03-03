#include <SPI.h>
const int dataPin = 15;     // Pin DS pada SN74HC595N
const int clockPin = 4;    // Pin SH_CP pada SN74HC595N
const int latchPin = 5;    // Pin ST_CP pada SN74HC595N
const int sensorPin = 2;  // Pin sensor getar

const int jumlahLED = 8;

void setup() {
  pinMode(sensorPin, INPUT);
  // Set pin mode untuk pin yang digunakan
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  Serial.begin(115200);  // Inisialisasi Serial Monitor
}

void loop() {
  // Baca nilai sensor getar
  int getaran = digitalRead(sensorPin);
  Serial.println(getaran);
  // Jika ada getaran, nyalakan LED dan kirim pesan ke Serial Monitor
  if (getaran == HIGH) {

    Serial.println("TErdeteksi Getaran!");
    // Nyala 4 LED kanan dan 4 LED kiri secara bergantian
    for (int m = 0; m < 2; m++) {
      for (int i = 0; i < 4; i++) {
        updateShiftRegister(1 << i | 1 << (7 - i));
        delay(50);
      }
      updateShiftRegister(0);
      delay(100);  // Delay tambahan setelah Nyala 4 LED kanan dan 4 LED kiri secara bergantian

      // Nyala 4 LED kanan dan 4 LED kiri secara bergantian dan sebaliknya
      for (int i = 3; i >= 0; i--) {
        updateShiftRegister(1 << i | 1 << (7 - i));
        delay(50);
      }
      updateShiftRegister(0);
      delay(100);  // Delay tambahan setelah Nyala 4 LED kanan dan 4 LED kiri secara bergantian dan sebaliknya
    }
  } else {
    Serial.println("Tidak Terdeteksi Getaran!");
    // Berjalan maju mundur setelah Nyala 4 LED kanan dan 4 LED kiri secara bergantian delay 100
    for (int n = 0; n < 1; n++) {
      // Berjalan maju
      for (int i = 0; i < 8; i++) {
        shiftOut(dataPin, clockPin, MSBFIRST, 1 << i);
        digitalWrite(latchPin, HIGH);
        delay(100);
        digitalWrite(latchPin, LOW);
      }

      // Berjalan mundur
      for (int i = 7; i >= 0; i--) {
        shiftOut(dataPin, clockPin, MSBFIRST, 1 << i);
        digitalWrite(latchPin, HIGH);
        delay(100);
        digitalWrite(latchPin, LOW);
      }
    }
  }

  delay(1000);  // Delay untuk mengurangi sensitivitas dan mencegah ledakan LED yang cepat
}

void updateShiftRegister(byte data) {
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  digitalWrite(latchPin, HIGH);
  delay(100);
  digitalWrite(latchPin, LOW);
}
