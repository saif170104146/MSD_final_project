//#include <NewPing.h>
#include <Servo.h>
#include<LiquidCrystal.h>
#include "DHT.h"

#define DHTPIN 51
#define DHTTYPE 22
DHT dht(DHTPIN, DHTTYPE);

// LCD module connections (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(22, 23, 24, 25, 26, 27);

Servo myservo;

int LeftF = 12,LeftR = 13;
int RightF = 11,RightR = 10;
int start ,cleaningMotor=8;
long frontDistance,leftDistance,rightDistance;

int ldr=A0,intensity=0;
int buzzer=53,gasSensor=52;
const int echoPin = 2; // Echo Pin of Ultrasonic Sensor
const int pingPin = 3; // Trigger Pin of Ultrasonic Sensor

const int Button = 50,buttonLdr=49; // Trigger Pin of Ultrasonic Sensor
int buttonState=0,buttonStateLdr=0;
char temperature[] = "Temp = 00.0 C";

void setup() {
  Serial.begin(9600);
  pinMode(LeftF, OUTPUT);
  pinMode(LeftR, OUTPUT);
  pinMode(RightF, OUTPUT);
  pinMode(RightR, OUTPUT); 
  pinMode(pingPin, OUTPUT); // initialising pin 3 as output
  pinMode(echoPin, INPUT); // initialising pin 2 as input
  pinMode(Button, INPUT);
  pinMode(buttonLdr, INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(gasSensor,OUTPUT);
  pinMode(cleaningMotor,OUTPUT);
  myservo.attach(9); 
  lcd.begin(16,2);
  dht.begin();
}
void run()     // 
{
    Serial.println("Going Forward..");
    digitalWrite(LeftF, HIGH); 
    digitalWrite(RightF, HIGH);
    digitalWrite(LeftR, LOW); 
    digitalWrite(RightR, LOW); 
}

void brake(int time)  //
{
  Serial.println("braked");
  digitalWrite(LeftF, LOW); 
  digitalWrite(RightF, LOW);
  digitalWrite(LeftR, LOW); 
  digitalWrite(RightR, LOW);
  delay(time * 100);//  
}

void left(int time)         //()
//void left()         //()
{
  Serial.println("Going Left..");
  digitalWrite(LeftF, LOW); 
  digitalWrite(RightF, HIGH);
  digitalWrite(LeftR, LOW); 
  digitalWrite(RightR, LOW);
  delay(time * 100);  //  
}
void right(int time)         //()
//void left()         //()
{
  Serial.println("Going Right..");
  digitalWrite(LeftF, HIGH); 
  digitalWrite(RightF, LOW);
  digitalWrite(LeftR, LOW); 
  digitalWrite(RightR, LOW);
  delay(time * 100);  //  
}
void back(int time)         //()
//void left()         //()
{
  Serial.println("Going Back..");
  digitalWrite(LeftF, LOW); 
  digitalWrite(RightF, LOW);
  digitalWrite(LeftR, HIGH); 
  digitalWrite(RightR, HIGH);
  delay(time * 100);  //  
}
void spin_right(int time)         //()
{
  Serial.println("Going spining right..");
  digitalWrite(LeftF, HIGH); 
  digitalWrite(RightF, LOW);
  digitalWrite(LeftR, LOW); 
  digitalWrite(RightR, HIGH);
  delay(time * 100);  //  
} 

long Distance()   //  
{
  digitalWrite(pingPin, LOW);   // 2s
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);  // 10s10s
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);    // 
  long Fdistance = pulseIn(echoPin, HIGH);  // ()
  Fdistance = microsecondsToInches(Fdistance);
  return Fdistance;
}
void setServoAngle(int angle)/*PWM0.5MS2.5MS 1.5MS 20MS*/ 
{
  myservo.write(angle);
  delay(1000);
}
void FrontDetection()
{
  setServoAngle(90);
  frontDistance=Distance();
}
void LeftDetection()
{
  setServoAngle(20);
  leftDistance=Distance();
}
void RightDetection()
{
  setServoAngle(160);
  rightDistance=Distance();
}
void buzzerSound(int value)
{
  tone(buzzer,value);
}
void checkGas(){
  if(digitalRead(gasSensor)==HIGH)
    {
      buzzerSound(500);
    }
  else
  {
    noTone(buzzer);
  }
}
void printTemperature(){
  //Read temperature in degree Celsius
  int Temp = dht.readTemperature() * 10;
  
  // Check if any reads failed and exit early (to try again)
  if (isnan(Temp)) {
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Error");
    return;
  }
 
  if(Temp < 0){
    temperature[6] = '-';
    Temp = abs(Temp);
  }
  else
    temperature[6]   = ' ';
    //Serial.println(Temp);

  temperature[7]   = (Temp / 100) % 10  + 48;
  temperature[8]   = (Temp / 10)  % 10  + 48;
  temperature[10]  =  Temp % 10 + 48;
  temperature[11]  = 223;        // Degree symbol ( °)
  
  lcd.setCursor(0, 0);
  lcd.print(temperature);
  
}
void loop() {
  //setServoAngle(90);
  //tone(buzzer,1000);
  buttonState = digitalRead(Button);
  buttonStateLdr = digitalRead(buttonLdr);
  checkGas();
  if(buttonStateLdr== HIGH)
  {
    intensity=analogRead(ldr);
  }
  else{
    intensity=200;
  }
  
 // Serial.println("intensity :"intensity);
 // Serial.println(intensity);
  //Serial.println(Temp);
  printTemperature();
  if(intensity>100 && buttonState==HIGH)
  {
    start=1;
  }
  else
  {
    start=0;
    digitalWrite(cleaningMotor, LOW); 
    brake(1);
  }
  if(start==1)
  {
    digitalWrite(cleaningMotor, HIGH); 
    FrontDetection();
    Serial.println(frontDistance);
    if(frontDistance>50)
    {
      run();
    }
    else
    {
      brake(2);
      delay(1000);
      back(2);
      delay(1000);
      brake(2);
      delay(1000);
      LeftDetection();
      Serial.println(leftDistance);
      delay(3000);
     // Distance_display(Left_Distance);
      RightDetection();
      Serial.println(rightDistance);
      delay(3000);
     // Distance_display(Right_Distance);
      if((leftDistance < 50 ) &&( rightDistance < 50 ))
        spin_right(0.7);
      else if(leftDistance > rightDistance)
      {      
        left(4);//
        brake(1);//
      }
      else//
      {
        right(4);//
        brake(1);//
      }
    }
  }
   delay(500);
}

long microsecondsToInches(long microseconds) // method to covert microsec to inches 
{
 return microseconds / 74 / 2;
}
