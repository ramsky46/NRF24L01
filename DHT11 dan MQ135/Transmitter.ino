/* Transmitter
 * DHT11 dan MQ135
 * Rama abimanyu
 */
#include <SPI.h>
#include <nRF24L01.h> //library nrf24l01
#include <RF24.h>

#include <DHT.h> //library DHT
#define DHTPIN 2 //pin data DHT terhubung ke pin 2 arduino
#define DHTTYPE DHT11 //Tipe sensor DHT
DHT dht(DHTPIN, DHTTYPE);

#define RL 10.0 //jika Resistor <10k maka RL=10k, jika Resistor >10K sesuaikan dengan nilai resistor
#define m -0.396 //Hasil perhitungan berdasarkan datasheet untuk gas NH4
#define Ratiocleanair 3.6

/*        m     |  b
 * CO2   0.336    -0.306
 * CO   -0.233     0.693
 * NH4  -0.396     0.792
 */


const int MQ = A0; //pin analog MQ terhubung ke pin A0 arduino
const byte address[6] = "00001"; //alamat NRF24L01
const float b = 0.792; //hasil perhitungan berdasarkan datasheet untuk gas NH4

float VRL;
float Rs;
float Ro;
float ratio;
float ppm;

RF24 radio(7, 8); // CE, CSN

float h,t;


void setup() {
  Serial.begin(9600); //memberi perintah untuk serial monitor memulai
  pinMode(A0, INPUT); //deklarasi a0 sebagai input data
  
  dht.begin(); //memberi perintah untuk dht memulai
  radio.begin(); //memberi perintah untuk nrf24l01 memulai
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(address); //menyebarkan alamat
  radio.setPALevel(RF24_PA_MIN); 
  radio.stopListening(); //nrf24l01 pengirim
}

void loop() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  VRL = analogRead(MQ)*(5.0/1023.0);
  Rs = ((5/VRL)-1) * RL;
  Ro = Rs/Ratiocleanair;
  ratio = Rs/Ro;
  ppm = pow(10, ((log10(ratio)-b)/m));

  Serial.println(ppm);
  Serial.println(h);
  Serial.println(t);

  radio.write(&ppm, sizeof(ppm));
  radio.write(&h, sizeof(h));
  radio.write(&t, sizeof(t));
  delay(1000);
}
