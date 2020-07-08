
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include "DHT.h" 

// DHT 
#define DHTPIN 7
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE); 

// Bodenfeuchte
#define MST A0

// Variablen für Sensoren

float temperature;
float humedy;
int wait = 10000;
int moisture = 0;
String output;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  Serial.begin(9600);
  dht.begin();
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
}

void loop() {
  
  checkMoisture();
  checkDht();
  showTextTest(temperature, humedy, output);  
  serialOutput(); 
  
}

// Holte DHT11 Infos und schreibt sie in Variablen
void checkDht() {
  
  humedy = dht.readHumidity(); //die Luftfeuchtigkeit auslesen und unter „Luftfeutchtigkeit“ speichern
  temperature = dht.readTemperature();//die Temperatur auslesen und unter „Temperatur“ speichern
  delay(wait); //Zwei Sekunden Vorlaufzeit bis zur Messung (der Sensor ist etwas träge)
  
}

// Holt Bodenfeuchte und schreibt sie in Variablen
void checkMoisture() {
  
  moisture = analogRead(MST);
  
  if (moisture > 0 && moisture <= 200) {
    output = "zu nass"; 
  } else if (moisture >= 201 && moisture < 500) {
    output = "gut so"; 
  } else if (moisture >= 501 && moisture < 600) {
    output = "noch gut so";
  } else if (moisture >= 601) {
    output = "zu trocken";
  } else {
    output = "???";
  }
  
}

// Gibt den Text auf das kleine OLED display aus
void showTextTest(float t, float h, String s) {
  
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(F("Boden:   "));
  display.print(s);
  display.println(F("!"));
  display.setCursor(0, 10);
  display.print(F("Temp:    "));
  display.print(t);
  display.print(F(" "));
  display.print(char(247)); // Grad-Zeichen
  display.println(F("C"));
  display.setCursor(0, 20);
  display.print(F("Feuchte: "));
  display.print(h);
  display.println(F(" %"));
  display.display();  
  delay(100);
  
}

void serialOutput() {
  
  Serial.print("Luftfeuchtigkeit: "); //Im seriellen Monitor den Text und 
  Serial.print(humedy); //die Dazugehörigen Werte anzeigen
  Serial.println(" %");
  Serial.print("Temperatur: ");
  Serial.print(temperature);
  Serial.println(" Grad Celsius");
  Serial.print("Bodenfeuchte: ");
  Serial.println(moisture);
}
