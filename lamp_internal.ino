#include <FlexiTimer2.h> // 引入計時中斷函式庫
#define usedDigitalPin D8 //紅外線遙控器
#define usedDigitalPinD4 D4//觸控器

unsigned int bitPattern=0, newKey=0;
unsigned char timerValue;
unsigned char msCount=0;
char pulseCount=0;
long unsigned int nt=0;
#define UPDATE_TIME 20
#define servoPin D3//頭頂掀蓋
int degree = 0;
int endcode;

#define UPDATE_TIMED5 20
#define servoPinD5 D5//控制門的馬達
int degreeD5 = 0;
int endcodeD5;

#define TRIG_PIN D6 
#define ECHO_PIN  D7
const unsigned int MAX_DIST = 23200;

int convertAngleToImp(int degree){
  float a = 2000/180;
  float b = 500;
  return int(a*degree+b);
}
int convertAngleToImpD5(int degreeD5){
  float a = 2000/180;
  float b = 500;
  return int(a*degreeD5+b);
}
void ServoControl(int degree) {
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(convertAngleToImp(degree));
  digitalWrite(servoPin,LOW);
  delay(UPDATE_TIME);
}
void ServoControlD5(int degreeD5) {
  digitalWrite(servoPinD5, HIGH);
  delayMicroseconds(convertAngleToImpD5(degreeD5));
  digitalWrite(servoPinD5,LOW);
  delay(UPDATE_TIMED5);
}
void timer_isr_routine() { // 計時中斷函數
   FlexiTimer2::timer_stop();
   if(msCount<50)
    msCount++;
   FlexiTimer2::SetPeriod(1);
   FlexiTimer2::start(); 
} 

void exIR_int0() // INT0中斷服務程式
{  
  noInterrupts(); // 停止中斷
  FlexiTimer2::timer_stop();
  timerValue = msCount;
  msCount = 0;
  FlexiTimer2::SetPeriod(1);
  pulseCount++;
  if(timerValue>=50){
    pulseCount = -2;
    bitPattern = 0;
  }
  else if((pulseCount>=0)&&(pulseCount<32)){
    if(timerValue>=2)
      bitPattern |= (unsigned int)1<<(31-pulseCount);     
  }
  else if(pulseCount>=32){
    newKey = bitPattern;
    pulseCount = 0;
  }
  FlexiTimer2::start(); 
  interrupts(); // 恢復中斷
}
void setup() {
  Serial.begin(9600); 
  FlexiTimer2::set(1, timer_isr_routine);
  FlexiTimer2::start();
  pinMode(usedDigitalPin , INPUT_PULLUP);
  attachInterrupt(usedDigitalPin , exIR_int0, FALLING);
  pinMode(servoPin, OUTPUT);
  Serial.begin(9600);
  pinMode(servoPinD5, OUTPUT);
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600); 
  pinMode(LED1, OUTPUT);
  pinMode(usedDigitalPinD4, INPUT);

}
void loop() {
  if(newKey != 0){
    //Serial.println(newKey, HEX);
    switch(newKey){
      case 0x7FB44B: degree=0; break;
      case 0x7F9867: degree=30; break;
      case 0x7F8C73: degree=60; break;
      case 0x7FBD42: degree=90; break;

      default: break;
    }
    newKey = 0;  
  }
  ServoControl(degree);
  
  unsigned long t1, t2;
  unsigned long pulse_width;
  float cm;
// Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Wait for pulse on echo pin
  while ( digitalRead(ECHO_PIN) == 0 );
     // Measure how long the echo pin was held high (pulse width)
  t1 = micros();
  while ( digitalRead(ECHO_PIN) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

// Calculate distance in centimeters. This is calculated from the assumed speed of sound in air at sea level (~340 m/s).
    cm = pulse_width / 58.0;
  if((micros()/270000)>(nt)){//270000
    nt=micros()/270000;
    if ( pulse_width > MAX_DIST||cm<7){// pulse_width > MAX_DIST||cm<4
      Serial.println("Out of range");

      ServoControlD5(0);
    }
    else{
      Serial.print(cm);
      Serial.print("cm\n");
      ServoControlD5(90);
    }
  }
  if (digitalRead(usedDigitalPinD4) == HIGH){
            digitalWrite(LED1, HIGH);
            ServoControlD5(90);
    }else
            digitalWrite(LED1, LOW);
}