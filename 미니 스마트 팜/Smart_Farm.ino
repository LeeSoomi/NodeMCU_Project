#define BLYNK_PRINT Serial //시리얼 모니터 활성화
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
char auth[] = "*************************************"; //인증 토큰 입력 
char ssid[] = "U+Net****"; //사용 WiFi의 SSID 입력 
char pass[] = "**********"; //사용 WiFi의 password입력 
WidgetLED led1(V3); //LED위젯 객체 설정(Blynk V3 PIN) 
WidgetLED led2(V4); //LED위젯 객체 설정(Blynk V4 PIN) 

#define DHTPIN 12 //온습도센서 핀설정, GPIO12, D6
#define M1 5 //모터 핀설정, GPIO5, D1 
#define M2 4 //모터 핀설정, GPIO4, D2 
#define DHTTYPE DHT11 //온습도 센서 type설정, DHT 11
DHT dht(DHTPIN, DHTTYPE); //온습도센서 객체 함수 설정 
BlynkTimer timer; //Blynk 타이머함수
void sendSensor(){
 float t=dht.readTemperature();
 float h=dht.readHumidity();
 if(isnan(h) || isnan(t)){
 Serial.println("Failed to read from DHT sensor!");
 return; 
 }
 Blynk.virtualWrite(V0, t); //V0핀으로 온도값 보냄.
 Blynk.virtualWrite(V1, h); //V1핀으로 습도값 보냄 
}
BLYNK_WRITE(V2)
{
 int pinValue = param.asInt(); // V2핀을 통해 받은 값을 변수에 할당
 Serial.print("V1 Slider value is: ");
 Serial.println(pinValue);
 analogWrite(M1, 0);
 analogWrite(M2, pinValue); //모터 M2핀에 V2를통해 받은 값을 출력 
}
void setup() {
 pinMode(M1, OUTPUT);
 pinMode(M2, OUTPUT);
 Serial.begin(9600);
 Blynk.begin(auth, ssid, pass); //아두이노가 Blynk Cloud에 연결. 

 timer.setInterval(1000L, sendSensor); //1초 마다 sendSensor함수 실행 
}
void loop() {
 Blynk.run();
 timer.run();
 if(analogRead(A0)<300){ //토양 수분센서의 값이 300미만이라면 
 led1.on(); //첫 번째 LED를 켠다. 
 led2.off();
 }else{ //토양 수분 센서의 값이 300이상이라면 
 led1.off();
 led2.on(); //두 번째 LED를 켠다. 
 }
}
