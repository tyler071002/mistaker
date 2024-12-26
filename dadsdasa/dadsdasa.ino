#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <RtcDS1302.h>
#include <SPI.h>
#include <SD.h>

#define ONE_WIRE_BUS 2
#define TdsSensorPin A1
#define TakSensorPin A2
#define PHSensorPin A0

LiquidCrystal_I2C lcd(0x3F, 16, 2);
ThreeWire myWire(6, 5, 7);
RtcDS1302<ThreeWire> Rtc(myWire);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float tdsValue = 0.0;
float phValue = 0.0;
float tempValue = 0.0;
float takdo = 0.0;
RtcDateTime currentTime;

unsigned long prev = 0;
const long in = 300000;

File myFile;

void setup() {
  Serial.begin(9600);
  sensors.begin();
  Rtc.Begin();

  RtcDateTime now = Rtc.GetDateTime();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  if (now < compiled) {
    Rtc.SetDateTime(compiled);
  }
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  // SD 카드 초기화
  if (!SD.begin(4)) {
    Serial.println("SD 카드 초기화 실패");
    while (1);
  }
  Serial.println("SD 카드 초기화 성공");
}

void loop() {
  sensors.requestTemperatures();
  
  // 5분마다 데이터 수집
  unsigned long current = millis();
  if (current - prev >= in) {
    prev = current;

    // 센서 데이터 수집
    tdsValue = getTDSValue();
    phValue = getPHValue();
    tempValue = sensors.getTempCByIndex(0);
    currentTime = Rtc.GetDateTime();
    takdo = TakDo();

    // SD 카드에 데이터 저장
    saveDataToSD(tdsValue, phValue, tempValue, currentTime, takdo);

    // 시리얼로 데이터 출력
    Serial.print("TDS: ");
    Serial.print(tdsValue);
    Serial.print(" | pH: ");
    Serial.print(phValue);
    Serial.print(" | Temp: ");
    Serial.print(tempValue);
    Serial.print(" | Time: ");
    printDateTime(currentTime);
    Serial.print(" | Tak: ");
    Serial.println(takdo);
  }
}

void saveDataToSD(float tds, float ph, float temp, RtcDateTime dt, float tak) {
  myFile = SD.open("data.txt", FILE_WRITE);
  if (myFile) {
    char dateStr[20];
    char timeStr[20];
    snprintf(dateStr, sizeof(dateStr), "%02u/%02u/%04u", dt.Month(), dt.Day(), dt.Year());
    snprintf(timeStr, sizeof(timeStr), "%02u:%02u:%02u", dt.Hour(), dt.Minute(), dt.Second());

    myFile.print(tds);
    myFile.print(",");
    myFile.print(ph);
    myFile.print(",");
    myFile.print(temp);
    myFile.print(",");
    myFile.print(dateStr);
    myFile.print(",");
    myFile.print(timeStr);
    myFile.print(",");
    myFile.println(tak);

    myFile.flush();
    myFile.close();
    Serial.println("데이터가 SD 카드에 저장되었습니다.");
  } else {
    Serial.println("SD 카드에 파일을 열 수 없습니다.");
  }
}

float TakDo() {
  int sensorValue = analogRead(TakSensorPin);
  float dotak = sensorValue * (5.0 / 1024.0);
  return dotak;
}

float getTDSValue() {
  int sensorValue = analogRead(TdsSensorPin);
  float voltage = (5.0 / 1024.0) * sensorValue;
  return voltage;
}

float getPHValue() {
  int sensorValue = analogRead(PHSensorPin);
  float voltage = (5.0 / 1024.0) * sensorValue;
  return 14.5 + ((2.5 - voltage) / 0.18);
}

void printDateTime(const RtcDateTime& dt) {
  char dateStr[20];
  snprintf(dateStr, sizeof(dateStr), "%02u/%02u/%04u %02u:%02u:%02u",
           dt.Month(), dt.Day(), dt.Year(), dt.Hour(), dt.Minute(), dt.Second());
  Serial.print(dateStr);
}
