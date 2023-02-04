/* Receiver
 * Rama abimanyu
 */

#include <SPI.h>
#include <nRF24L01.h> //library nrf24l01
#include <RF24.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h> //library LCD 16x2 dengan i2c
LiquidCrystal_I2C lcd(0x27, 20, 4);

const byte address[6] = "00001"; //alamat nrf24l01
RF24 radio(7, 8); // CE, CSN

float h,t;
float ppm;

void setup() {
  Serial.begin(9600);
  radio.begin();
  
  lcd.init(); //memulai lcd
  lcd.backlight(); //agar lcd tidak blacklight
  
  radio.setAutoAck(false);
  radio.openReadingPipe(0, address); //menerima alamat
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  lcd.setCursor(0,0);
  lcd.print("Temp & Gas");
  lcd.setCursor(0,1);
  lcd.print("By ramsky46");
  delay(2000);
  lcd.clear();
  lcd.print("Starting....");
  delay(2000);
}
/*void recvData()
{
  if ( radio.available() ) {
    radio.read(&data, sizeof(MyData));
    }
}*/
void loop() {
  lcd.clear();
  delay(500);

  if(radio.available()){
  delay(50);
  
  radio.read(&ppm, sizeof(ppm));
  lcd.setCursor(3,1);
  lcd.print(" ");
  lcd.setCursor(6,1);
  lcd.print(ppm);
  lcd.print("ppm");
  
  radio.read(&h, sizeof(h));
  lcd.setCursor(0,0);
  lcd.print(h);             //membaca nilai kelembaban
  lcd.print("%");

  radio.read(&t, sizeof(t));
  lcd.setCursor(7,0);
  lcd.print(t);
  lcd.setCursor(12,0);
  lcd.print("C");

  
  }else {
    lcd.println("No radio available");
  }
  
  
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print("Kelembaban: ");
  Serial.println(h);
  Serial.print("amonia: ");
  Serial.println(ppm);
  delay(500);  
  //Serial.print("\n");
}
