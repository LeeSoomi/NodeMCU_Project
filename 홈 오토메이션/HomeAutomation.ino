#define BLYNK_PRINT Serial //시리얼 모니터 활성화
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
char auth[] = "**********************************"; //인증 토큰 입력 
char ssid[] = "U+Net****"; //사용 WiFi의 SSID 입력 
char pass[] = "**********"; //사용 WiFi의 password입력 
#define DHTPIN 14 //온습도센서 핀설정, GPIO14, D5 
#define DHTTYPE DHT11 // 온습도 센서 type설정, DHT 11
DHT dht(DHTPIN, DHTTYPE); //온습도센서 객체 함수 설정 
int PIRled = 15; //모션 센서에 따른 LED 핀설정, GPIO15, D8 
BlynkTimer timer; //Blynk 타이머함수 
void sendSensor(){
 float t=dht.readTemperature();
 float h=dht.readHumidity();
 if(isnan(h) || isnan(t)){
 Serial.println("Failed to read from DHT sensor!");
 return; 
 }
 int motion=digitalRead(2); //모션센서 핀설정, GPIO2, D4
 Blynk.virtualWrite(V0, t); //V0핀으로 온도값 보냄.
 Blynk.virtualWrite(V1, h); //V1핀으로 습도값 보냄 

 if(motion==HIGH){
 Blynk.virtualWrite(V2, "Motion Detected"); 
 //동작 감지하면 V2핀에 "Motion Detected" print
 digitalWrite(PIRled, HIGH);
 }else{
 Blynk.virtualWrite(V2, "No Motion Detected"); 
 //동작 감지하지 않으면 V2핀에 "No Motion Detected" print
 digitalWrite(PIRled, LOW);
 } 
}
void setup() {
 pinMode(2, INPUT);
 pinMode(15, OUTPUT);
 Serial.begin(9600);
 Blynk.begin(auth, ssid, pass); //아두이노가 Blynk Cloud에 연결. 
 timer.setInterval(1000L, sendSensor); //1초 마다 sendSensor함수 실행 
}
void loop() {
 Blynk.run();
 timer.run();
}
