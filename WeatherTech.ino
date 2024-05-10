#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <RTClib.h>
#include <EEPROM.h> // Include EEPROM library

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Address 0x27, 16 columns and 2 rows
RTC_DS3231 rtc;
#define DHTPIN 4    // You can use pins 2, 3, 4, 5, 12, 13, or 14
#define DHTTYPE DHT11 // Or DHT21, DHT22 (choose the appropriate sensor type)
DHT dht(DHTPIN, DHTTYPE);
#define UV_PIN A1

int buz = 8;  //buzzer connected to pin 8
int led = 9;  //led connected to pin 9

// Air quality sensor configuration
const int aqsensor = A0;  // Output of MQ135 connected to A0 pin
int threshold = 250;    // Threshold level for Air Quality

// Display durations (in milliseconds)
const int tempHumidDisplayTime = 3000; // Display temperature and humidity time
const int airQualityDisplayTime = 3000; // Display air quality time

void setup() {
  Serial.begin(9600); // Initialize serial communication
  dht.begin();
  rtc.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {
  // Read temperature, humidity, and air quality (ppm)
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int ppm = analogRead(aqsensor);
  DateTime now = rtc.now(); 

  // Store data in EEPROM
  int address = 0;
  EEPROM.put(address, humidity);
  address += sizeof(humidity);
  EEPROM.put(address, temperature);
  address += sizeof(temperature);
  EEPROM.put(address, ppm);
  address += sizeof(ppm);
  // Assuming uvIndex is a float value
  float uvIndex = getUVIndex(); // A function to get UV index
  EEPROM.put(address, uvIndex);
  address += sizeof(uvIndex);

  // Display temperature and humidity
  lcd.setCursor(0,0);
  lcd.print("Humidity= ");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("Temp=");
  lcd.print(temperature);
  lcd.print(" C      ");
  delay(tempHumidDisplayTime);

  // Display air quality
  lcd.clear(); // Clear the LCD screen
  lcd.setCursor(0, 0);
  lcd.print("Air Quality:");
  Serial.print("Air Quality: ");
  Serial.println(ppm);
  lcd.setCursor(12, 0);
  lcd.print(ppm );

  if (ppm > threshold) {
    lcd.setCursor(0, 1);
    lcd.print("AQ Level HIGH");
    Serial.println("AQ Level HIGH");
    tone(buz, 1000, 200); // Turn ON Buzzer
    digitalWrite(led, HIGH); // Turn ON LED
  } else {
    digitalWrite(led, LOW); // Turn OFF LED
    noTone(buz); // Turn OFF Buzzer
    lcd.setCursor(0, 1);
    lcd.print("AQ Level Good");
    Serial.println("AQ Level Good");
  }
  delay(airQualityDisplayTime);
  
  // Display UV index
  lcd.setCursor(0,0);
  lcd.print("UV Index:");
  lcd.print(uvIndex);
  if (uvIndex > 3) {
    lcd.setCursor(0, 1);
    lcd.print("UV Level HIGH");
    Serial.println("UV Level HIGH");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("UV Level Good");
    Serial.println("UV Level Good");
  }
  
  Serial.print("UV Index: ");
  Serial.println(uvIndex);
  
  delay(3000);  // Delay for stability

  // Display date and time
  lcd.setCursor(0, 0);
  lcd.print("Date: ");
  lcd.print(now.year(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.day(), DEC);
  
  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);
  
  delay(3000);     

  // Retrieve data from EEPROM and print
  float storedHumidity;
  float storedTemperature;
  int storedPPM;
  float storedUVIndex;

  address = 0; // Reset address to start of EEPROM
  EEPROM.get(address, storedHumidity);
  address += sizeof(storedHumidity);
  EEPROM.get(address, storedTemperature);
  address += sizeof(storedTemperature);
  EEPROM.get(address, storedPPM);
  address += sizeof(storedPPM);
  EEPROM.get(address, storedUVIndex);

  Serial.println("Retrieved data from EEPROM:");
  Serial.print("Humidity: ");
  Serial.println(storedHumidity);
  Serial.print("Temperature: ");
  Serial.println(storedTemperature);
  Serial.print("Air Quality (ppm): ");
  Serial.println(storedPPM);
  Serial.print("UV Index: ");
  Serial.println(storedUVIndex);
}

// Function to get UV index
float getUVIndex() {
  int sensorValue = analogRead(UV_PIN);  // Read analog value from sensor
  float voltage = sensorValue * (5.0 / 1023.0);  // Convert analog value to voltage (5V reference voltage)
  
  // Convert voltage to UV index
  return voltage * 3.33;  // Calibration factor (you may need to adjust this)
}
