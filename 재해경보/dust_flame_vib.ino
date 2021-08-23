#define BLYNK_PRINT Serial //시리얼 모니터 활성화
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "**************************************"; //인증 토큰 입력 
char ssid[] = "U+Net****"; //사용 WiFi의 SSID입력 
char pass[] = "**********"; //사용 WiFi의 password입력
BlynkTimer timer;
WidgetLED led1(V1); //LED위젯 객체 설정(Blynk V1 PIN) 
WidgetLED led2(V2); //LED위젯 객체 설정(Blynk V2 PIN) 
WidgetLED led3(V3); //LED위젯 객체 설정(Blynk V3 PIN) 
#define BLYNK_GREEN "#23C48E" //색상 키워드 설정 
#define BLYNK_BLUE "#04C0F8"
#define BLYNK_YELLOW "#ED9D00"
#define BLYNK_RED "#D3435C"
int ledDust = 5; //미세먼지센서 핀설정,D1, GPIO5
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
int vib= 4; //진동 센서 핀설정, D2, GPIO4
int fir= 14; //불꽃 센서 핀설정, D5, GPIO14
void sendSensor(){
 digitalWrite(ledDust,LOW); // power on the LED(미세먼지센서)
 delayMicroseconds(samplingTime); 
 voMeasured = analogRead(0); // read the dust value 
 delayMicroseconds(deltaTime);
 digitalWrite(ledDust,HIGH); // turn the LED off(미세먼지센서)
 delayMicroseconds(sleepTime);
 calcVoltage = voMeasured * (6.0 / 1024); //외부전원 6V 기준
 dustDensity = (0.17 * calcVoltage - 0.1)*1000; //미세먼지 농도 계산(unit: ug/m^3)
 Blynk.virtualWrite(V0, dustDensity); //Gauge위젯에 미세먼지농도 표기
 Serial.println(dustDensity);
 if(dustDensity<31){
 led1.setColor(BLYNK_BLUE); // blue, 좋음 
 led1.on();
 }else if(dustDensity<81){
 led1.setColor(BLYNK_GREEN); // green, 보통 
 led1.on();
 }else if(dustDensity<151){
 led1.setColor(BLYNK_YELLOW); //yellow, 나쁨 
 led1.on();
 Blynk.notify("Bad air!!"); //Notifiation위젯에서 "Bad air!!" 알림
 }else{
 led1.setColor(BLYNK_RED); //red, 매우나쁨 
 led1.on();
 Blynk.notify("The air is so bad!!"); //Notifiation위젯에서 "The air is so bad!!" 알림
 } 
}
void setup() {
 pinMode(5, OUTPUT);
 pinMode(vib, INPUT);
 pinMode(fir, INPUT);
 Serial.begin(9600);
 Blynk.begin(auth, ssid, pass); //아두이노가 Blynk Cloud에 연결. 
 timer.setInterval(1000L, sendSensor); //1초 마다 sendSensor함수 실행 
}
void loop() {
 Blynk.run();
 timer.run(); 
 int eq=digitalRead(vib); //진동센서 신호 읽어들임 
 int fire=digitalRead(fir); //불꽃센서 신호 읽어들임 
 if(eq==LOW){ //진동이 감지되면 
 Blynk.notify("Earthquake!!!!"); //Notifiation위젯에서 "Earthquake!!!" 알림
 led2.on(); //LED위젯 켜기 
 }else{
 led2.off();
 }
 if(fire==LOW){ //불이 감지되면 
 Blynk.notify("Fire!!!!"); //Notifiation위젯에서 "Fire!!!" 알림
 led3.on(); //LED위젯 켜기 
 }else{
 led3.off();
 }
}
