//RICK ROLL 相關函式參考自https://www.hackster.io/410027/rickroll-piezo-buzzer-a1cd11
#define  a3f    208     
#define  b3f    233     
#define  b3     247     
#define  c4     261     
#define  c4s    277     
#define  e4f    311     
#define  f4     349     
#define  a4f    415     
#define  b4f    466     
#define  b4     493     
#define  c5     523     
#define  c5s    554     
#define  e5f    622     
#define  f5     698    
#define  f5s    740     
#define  a5f    831     
#define NUM 17 
#define rest    -1
#define LATCH_DIO D15   
#define CLK_DIO D14
#define DATA_DIO D2
#include <LiquidCrystal.h>
#include <FlexiTimer2.h> 

static unsigned int num;
static  int num9=000;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int numa=1000;
int numap=1;
int dir;
int C=8;
int C9=0;
int cn=0;
int cnum=0;
int piezo = D3; 


volatile int beatlength = 100; 
float beatseparationconstant = 0.3;

unsigned char keypressed;
int key;
const byte col_pin[4]={A0,A1,A2,A3};
const byte row_pin[4]={D10,D11,D12,D13};

int threshold;

int leds[17] = {D0, D1, D2,  D10, D11, D12, D13, D14, D15,LED1, LED2, LED3, LED4,A0, A1, A2, A3};
int Alarm[4]={10,10,10,10};
int AlarmDate[3]={99,99,99};
char dayofmonth[12]={31,28,31,30,31,30,31,31,30,31,30,31};

int a; 
int b; 
int c; 

boolean flag;


int song1_intro_melody[] =
{c5s, e5f, e5f, f5, a5f, f5s, f5, e5f, c5s, e5f, rest, a4f, a4f};

int song1_intro_rhythmn[] =
{6, 10, 6, 6, 1, 1, 1, 1, 6, 10, 4, 2, 10};

int song1_verse1_melody[] =
{ rest, c4s, c4s, c4s, c4s, e4f, rest, c4, b3f, a3f,
  rest, b3f, b3f, c4, c4s, a3f, a4f, a4f, e4f,
  rest, b3f, b3f, c4, c4s, b3f, c4s, e4f, rest, c4, b3f, b3f, a3f,
  rest, b3f, b3f, c4, c4s, a3f, a3f, e4f, e4f, e4f, f4, e4f,
  c4s, e4f, f4, c4s, e4f, e4f, e4f, f4, e4f, a3f,
  rest, b3f, c4, c4s, a3f, rest, e4f, f4, e4f
};

int song1_verse1_rhythmn[] =
{ 2, 1, 1, 1, 1, 2, 1, 1, 1, 5,
  1, 1, 1, 1, 3, 1, 2, 1, 5,
  1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 3,
  1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 4,
  5, 1, 1, 1, 1, 1, 1, 1, 2, 2,
  2, 1, 1, 1, 3, 1, 1, 1, 3
};


int song1_chorus_melody[] =
{ b4f, b4f, a4f, a4f,
  f5, f5, e5f, b4f, b4f, a4f, a4f, e5f, e5f, c5s, c5, b4f,
  c5s, c5s, c5s, c5s,
  c5s, e5f, c5, b4f, a4f, a4f, a4f, e5f, c5s,
  b4f, b4f, a4f, a4f,
  f5, f5, e5f, b4f, b4f, a4f, a4f, a5f, c5, c5s, c5, b4f,
  c5s, c5s, c5s, c5s,
  c5s, e5f, c5, b4f, a4f, rest, a4f, e5f, c5s, rest
};

int song1_chorus_rhythmn[] =
{ 1, 1, 1, 1,
  3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
  1, 1, 1, 1,
  3, 3, 3, 1, 2, 2, 2, 4, 8,
  1, 1, 1, 1,
  3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
  1, 1, 1, 1,
  3, 3, 3, 1, 2, 2, 2, 4, 8, 4
};

//取得按鈕陣列函式1
void KeyScan(){
  unsigned char col, row, i;
  unsigned char ScanLine = 0x01;
  keypressed = 0xFF;
  for(col=0;col<4;col++){
    for(i=0;i<4;i++)
      digitalWrite(col_pin[i], bitRead(~ScanLine,i));     
    for(row=0;row<4;row++){
      pinMode(row_pin[row], INPUT);
      if(digitalRead(row_pin[row])==LOW){
        delay(100);
        if(digitalRead(row_pin[row])==LOW){
          keypressed =  (col<<4) | row;  
        }
      }
    }
    ScanLine <<= 1;    
  }
}

//取得按鈕陣列函式2
int gotkey(){
    KeyScan();
    switch (keypressed) {
        case 0x00: return 0; break;
        case 0x10: return 1; break;
        case 0x20: return 2; break;
        case 0x30: return 3; break;
        case 0x01: return 4; break;
        case 0x11: return 5; break;
        case 0x21: return 6; break;
        case 0x31: return 7; break;
        case 0x02: return 8; break;
        case 0x12: return 9; break;
        case 0x22: return 10; break;
        case 0x32: return 11; break;
        case 0x03: return 12; break;
        case 0x13: return 13; break;
        case 0x23: return 14; break;
        case 0x33: return 15; break;
        default:   return 16; break;
    }
}

//音樂功能函式
void play() {
  
  int notelength;
  if (a == 1 || a == 2) {

    notelength = beatlength * song1_intro_rhythmn[b];
    if (song1_intro_melody[b] > 0) {     
    tone(piezo, song1_verse1_melody[b], notelength);
    }
    b++;
    if (b >= sizeof(song1_intro_melody) / sizeof(int)) {
      a++;
      b = 0;
      c = 0;
    }
  }
  else if (a == 3 || a == 5) {

    notelength = beatlength * 2 * song1_verse1_rhythmn[b];
    if (song1_verse1_melody[b] > 0) {
      tone(piezo, song1_verse1_melody[b], notelength);
      c++;
    }
    b++;
    if (b >= sizeof(song1_verse1_melody) / sizeof(int)) {
      a++;
      b = 0;
      c = 0;
    }
  }
  else if (a == 4 || a == 6) {
    // chorus
    notelength = beatlength * song1_chorus_rhythmn[b];
    if (song1_chorus_melody[b] > 0) {
      tone(piezo, song1_chorus_melody[b], notelength);
      c++;
    }
    b++;
    if (b >= sizeof(song1_chorus_melody) / sizeof(int)) {
      Serial.println("");
      a++;
      b = 0;
      c = 0;
    }

  }
  delay(notelength);
  noTone(piezo);
  delay(notelength * beatseparationconstant);
  if (a == 7) { // loop back around to beginning of song
    a = 1;
  }
 
}


//鬧鐘功能函式
void playALARM() {

  int notelength;
  if (a == 1 || a == 2) {
    // intro
    notelength = beatlength * song1_intro_rhythmn[b];
    if (song1_intro_melody[b] > 0) {     
      tone(piezo, song1_intro_melody[b], notelength);
    }
    b++;
    if (b >= sizeof(song1_intro_melody) / sizeof(int)) {
      a++;
      b = 0;
      c = 0;
    }
  }
  else if (a == 3 || a == 5) {
    // verse
    notelength = beatlength * 2 * song1_verse1_rhythmn[b];
    if (song1_verse1_melody[b] > 0) {
      tone(piezo, song1_verse1_melody[b], notelength);
      c++;
    }
    b++;
    if (b >= sizeof(song1_verse1_melody) / sizeof(int)) {
      a++;
      b = 0;
      c = 0;
    }
  }
  else if (a == 4 || a == 6) {
    // chorus
    notelength = beatlength * song1_chorus_rhythmn[b];
    if (song1_chorus_melody[b] > 0) {
      tone(piezo, song1_chorus_melody[b], notelength);
      c++;
    }
    b++;
    if (b >= sizeof(song1_chorus_melody) / sizeof(int)) {
      Serial.println("");
      a++;
      b = 0;
      c = 0;
    }
  }
  delay(notelength);
  noTone(piezo);
  delay(notelength * beatseparationconstant);
 
}

//時間
typedef struct {
    int hour;
    int minute;
    int second;
} ttime;

typedef struct {
    int year;
    int month;
    int day;
} tdate;
//預設時間
ttime now={16,41,45},tmptime;
tdate today={22,12,29},tmpday;
unsigned char monthday(unsigned char year, unsigned char month)
{
  if(month==2 && year%4==0)   //潤年的2月有29天
        return(29);
  else
    return(dayofmonth[month-1]);//非閏年時的該月份天數
}

// 計時中斷函數
void timer_isr_routine() { 
  now.second++;        
  if (now.second==60) {   
    now.second=0;   
    now.minute++;   
    if (now.minute==60) { 
      now.minute=0; 
      now.hour++; 
      if (now.hour==24) { 
        now.hour=0;
        today.day++;
        if (today.day>monthday(today.year,today.month)) {
          today.day=1;    
          today.month++;                
          if(today.month==13) {
              today.month=1;
              today.year++;
          }
        }
      }
    }
  }
}

//時鐘顯示函式
void displaycnt(unsigned char num)
{
  if(num < 10){
    lcd.print('0');
    lcd.print(num);     
  }
  else
    lcd.print(num);    
}

//設定時鐘日期與時間時的顯示函式
int getdigit(unsigned char x,unsigned char y)
{ 
  char  keys;
  lcd.setCursor(x,y);
  lcd.print('_');
  while((keys=gotkey())>9);
  lcd.setCursor(x,y);
  lcd.print(keys,DEC);
  return(keys);
}

//設定時鐘日期函式
int getdate()
{
  char temp,days;
  lcd.setCursor(0,0);
  lcd.print("20");
  lcd.setCursor(4,0);
  lcd.print("/");
  lcd.setCursor(7,0);
  lcd.print("/");

  temp=getdigit(2,0);
  tmpday.year=temp*10+getdigit(3,0);

  do {
      while((temp=getdigit(5,0))>1);  //月的十位數不能大於1
      temp=temp*10+getdigit(6,0);
  } while (temp > 12 || temp==0);
  tmpday.month=temp;
  do {
      while((temp=getdigit(8,0))>3);  //日的十位數不能大於3
      temp=temp*10+getdigit(9,0);
      days=monthday(tmpday.year,tmpday.month);
  } while (temp > days || temp==0);//如果輸入的日期大於該月份的日期就重新輸入
    tmpday.day=temp;
    return 1;
}

//設定時鐘時間函式
int gettime()
{
  char temp;
  lcd.setCursor(2,1);
  lcd.print(':');
  lcd.setCursor(5,1);
  lcd.print(':');
  do {
    while((temp=getdigit(0,1))>2);//時的十位數不能大於2
    temp=temp*10+getdigit(1,1);
  } 
  while (temp > 23);
  tmptime.hour=temp;
  while((temp=getdigit(3,1))>5);
  tmptime.minute=temp*10+getdigit(4,1);
  return 1;
}

//設定計時時間函式
int getdigit9()
{ 
  char key;
  while((key=gotkey())>9){delay(10);};
  num9=num9+int(key)*1000;
  WriteNumberToSegment(0 ,num9/1000%10);
  while((key=gotkey())>9){delay(10);}
  num9=num9+int(key)*100;
  WriteNumberToSegment(1 ,num9/100%10);
  while((key=gotkey())>9){delay(10);};
  num9=num9+int(key)*10; 
  WriteNumberToSegment(2 ,num9/10%10);
  while((key=gotkey())>9){delay(10);};
  num9=num9+int(key);
  WriteNumberToSegment(3 ,num9%10);
  return 0;
  }
  
//設定計時鬧鐘函式
int Alarmclock()
{ char key;
  while((key=gotkey())>9){delay(10);};
    Alarm[0]=key;
    lcd.setCursor(12,0);
    lcd.print(Alarm[0]);
  while((key=gotkey())>9){delay(10);}
  Alarm[1]=key;
    lcd.setCursor(13,0);
    lcd.print(Alarm[1]);
  while((key=gotkey())>9){delay(10);};
  Alarm[2]=key;
    lcd.setCursor(14,0);
    lcd.print(Alarm[2]);
  while((key=gotkey())>9){delay(10);};
  Alarm[3]=key;
    lcd.setCursor(15,0);
    lcd.print(Alarm[3]);


   return 0;
 }
 
// 計數器顯示正數函式
void timer_isr_routine9() { 
  if(num9>9999){
    num9=0;
  }else{
    num9=num9+1;  
  }
}

//計數器顯示反數函式
void timer_isr_routine_sub9() { 
  if(num9<1){ 
    num9=9999;
  }else{ 
    num9=num9-1;
  }  
}

const byte SEGMENT_MAP[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0X7F,0X6F};
const byte SEGMENT_SELECT[] = {0xFE,0xFD,0xFB,0xF7};

//計數器顯示函式
void WriteNumberToSegment(byte Segment, byte Value) 
{
  digitalWrite(LATCH_DIO,LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_MAP[Value]);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[Segment]);
  digitalWrite(LATCH_DIO,HIGH);
}

//設定
 void setup() {
    FlexiTimer2::set(1000, timer_isr_routine); // 每1000ms產生中斷，進入中斷服務常式
    FlexiTimer2::start(); // 啟動開始計時
    lcd.begin(16, 2);
    pinMode(piezo, OUTPUT);
    Serial.begin(9600);
    flag = true;
    a = 4;
    b = 0;
    c = 0;
    for(int i = 0; i < NUM; i++){
      pinMode(leds[i], OUTPUT);
    }    
    pinMode(BT1, INPUT); // initialize digital pin BT1 as an input
    pinMode(BT2, INPUT); // initialize digital pin BT2 as an input
    pinMode(BT3, INPUT); // initialize digital pin BT3 as an input
    pinMode(BT4, INPUT); // initialize digital pin BT4 as an input
    for(int i=0;i<4;i++){
      pinMode(col_pin[i], OUTPUT);
      pinMode(row_pin[i], OUTPUT);
      digitalWrite(col_pin[i],HIGH);    
      digitalWrite(row_pin[i],HIGH);    
    }
    pinMode(LATCH_DIO,OUTPUT);
    pinMode(CLK_DIO,OUTPUT);
    pinMode(DATA_DIO,OUTPUT);
  }
boolean buttonUp1=true;
boolean LedState1=false;
boolean buttonUp2=true;
boolean LedState2=false;
boolean buttonUp3=true;
boolean LedState3=false;
boolean buttonUp4=true;
boolean LedState4=false;
boolean buttonUp5=true;
boolean LedState5=false;
long unsigned int nowtime=0;

//主函式
void loop() {
  int buttonState1 = 0; // declare integer variables
  int buttonState2 = 0; // declare integer variables
  int buttonState3 = 0; // declare integer variables
  int buttonState4 = 0;// declare integer variables
  int buttonState5 = 0;
  int delay_number = 100; 
  buttonState1 = digitalRead(BT1); 
  buttonState2 = digitalRead(BT2); 
  buttonState3 = digitalRead(BT3); 
  buttonState4 = digitalRead(BT4);
  key = gotkey();
  buttonState5 = digitalRead(0);
  int case1 = 0x0000;
  int case2 = 0xAAAA;
  int case3 = 0x5555;
  //讀取按鈕狀態
 //bt1
 //按鈕LED功能1
  if (buttonState1 == HIGH && buttonUp1 == true) {  
    LedState1=!LedState1;
      for(int i = 9; i <18 ; i++){
      
        digitalWrite(leds[i],LedState1 );
            }
    buttonUp1 = false;
  } 
  
  else if(buttonState1 != HIGH && buttonUp1 != true){    
      buttonUp1 = true;
  }
   //按鈕LED功能2

    if (buttonState2 == HIGH && buttonUp2 == true) {  
    LedState2=!LedState2;
      for(int i = 10; i <18  ; i=i+2){
      
        digitalWrite(leds[i],LedState2 );
            }
    buttonUp2 = false;
  } 
  
  else if(buttonState2 != HIGH && buttonUp2 != true){    
      buttonUp2 = true;
  }
   //按鈕LED功能3
    if (buttonState3 == HIGH && buttonUp3 == true) {  
    LedState3=!LedState3;
      for(int i = 9; i <18 ; i=i+2){
      
        digitalWrite(leds[i],LedState3 );
            }
    buttonUp3= false;
  } 
  
  else if(buttonState3 != HIGH && buttonUp3 != true){    
      buttonUp3 = true;
  }
   //按鈕音樂功能4
    if (digitalRead(BT4)==LOW){     
          {LedState4=!LedState4;
          play();
          }
    }
     if(LedState4== true){    
        play();
     }
      else if(LedState4==false)
      {noTone(piezo);}


 //時鐘計數和更改功能
    int temp = gotkey();
    if(temp == 15){
        if (getdate()) today=tmpday;
        if (gettime()) now=tmptime;
    }
    else{
        lcd.setCursor(0,0);
        lcd.print("20");
        lcd.print(today.year);
        lcd.print("/");    
        displaycnt(today.month);      
        lcd.print("/");
        displaycnt(today.day);        
        lcd.setCursor(0, 1);
        displaycnt(now.hour);          
        lcd.print(":"); 
        displaycnt(now.minute);
        lcd.print(":");
        displaycnt(now.second);         
    }

  //計時器正數反數與更改功能
        WriteNumberToSegment(0 ,num9/1000%10);
        WriteNumberToSegment(1 ,num9/100%10);
        WriteNumberToSegment(2 ,num9/10%10);
        WriteNumberToSegment(3 ,num9%10);
  
  
  int temp9 = gotkey();
  if(temp9==10 and dir==0){
    dir=1;
  }else if(temp9==10 and dir==1){
    dir=0;
  }else if(temp9==11){
    num9=0000;
    getdigit9();
    if(dir==0)
      {num9=num9-1;}
    else if(dir==1)
      {num9=num9+1;}
  }
  if( (millis()/1000) >= (nowtime+1) ){
    nowtime=millis()/1000;
    if(dir==1){
      timer_isr_routine_sub9();
    }else if(dir==0){
      timer_isr_routine9();
    }
  }
  //鬧鐘功能
  
  if(temp9==13)
    {Alarmclock();
     numap=0;
     }
  if((num9/1000%10==Alarm[0])&&(num9/100%10==Alarm[1])&&(num9/10%10==Alarm[2])&&(num9%10==Alarm[3])) 
  {   numa=0;
      
  } 
  if(temp9==12)
    {numa=1;numap=1;Alarm[0]=10;Alarm[1]=10;Alarm[2]=10;Alarm[3]=10;
      }
  if(numa==0)
     {playALARM();
      }
  if(numap==0)
  { lcd.setCursor(12,0);
    lcd.print(Alarm[0]);
    lcd.setCursor(13,0);
    lcd.print(Alarm[1]);
    lcd.setCursor(14,0);
    lcd.print(Alarm[2]);
  lcd.setCursor(15,0);
    lcd.print(Alarm[3]);
    }
  else if(numap==1)
  {lcd.setCursor(13,0);
    lcd.print("X");
    lcd.setCursor(14,0);
    lcd.print("X");
    lcd.setCursor(15,0);
    lcd.print("X");
  lcd.setCursor(12,0);
    lcd.print("X");
    }
  //時區功能
    if(temp9==14)
      {C=C+1;
       if(C>14)
          {C=-12;today.day=today.day-1;}
        now.hour=now.hour+1;
        if(now.hour>=24)
        {now.hour=0;
         today.day=today.day+1;
          }
       }
  
    lcd.setCursor(10,1);
    lcd.print("UTC");
    lcd.setCursor(13,1);
    lcd.print(C);
    if(C>=0&&C<=9)
    {lcd.setCursor(14,1);lcd.print(" ");lcd.setCursor(15,1);lcd.print(" ");}
    else if(C>=10)
    {lcd.setCursor(15,1);lcd.print(" ");}
    else if(C<0&&C>=-9)
    {lcd.setCursor(15,1);lcd.print(" ");}
}