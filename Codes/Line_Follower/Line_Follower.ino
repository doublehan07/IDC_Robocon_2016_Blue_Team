#include "configuration.h"
#include "myConf.h"
#include <Servo.h>

enum Motor
{
  Motor1,
  Motor2,
  Motor3,
  Motor4,
};

//=============== Function Name Declaration ===============
  double Deadband(double value,double limit);
  void SetMotorDirection(Motor mt, bool direc);
  void SetServoAngle (unsigned int angle, unsigned int delay_time);
  
//=============== Parameters Declaration ==================
// Time
  unsigned long previousLoopTime = 0;
  unsigned long loopTime = 0;
  
// Input Signal from Receiver 
  int input1 = 0;
  int input2 = 0;
  int input3 = 0;
  int input4 = 0;
  int input5 = 0;
  int input6 = 0;
  
// Output Signal to Motor Driver  
  int out1 = 0;
  int out2 = 0;
  int out3 = 0;
  int out4 = 0;
  
// Motor's Current
float currentValue1 = 0.0;
float currentValue2 = 0.0;
float currentValue3 = 0.0;
float currentValue4 = 0.0;
int currentLimit = 5;

// Type Servo
  Servo myServo1;

//===================== setup() ========================
void setup () 
{
//===== Set Digital Pin Mode =====    
// Set pinmode to read command signal from Receiver.  
  pinMode(CH1, INPUT);   //channel 1
  pinMode(CH2, INPUT);   //channel 2
  pinMode(CH3, INPUT);   //channel 3
  pinMode(CH4, INPUT);   //channel 4
  pinMode(CH5, INPUT);   //channel 5
  pinMode(CH6, INPUT);   //channel 6  
  
// Set pinmode to read command signal from Test Switch.
  pinMode(buttonPin, INPUT);
  
// Set pinmode to write command signal to Motor Driver.
  pinMode(INA1, OUTPUT);
  pinMode(INB1, OUTPUT);
  pinMode(PWM1, OUTPUT);
  
  pinMode(INA2, OUTPUT);
  pinMode(INB2, OUTPUT);
  pinMode(PWM2, OUTPUT);

  pinMode(INA3, OUTPUT);
  pinMode(INB3, OUTPUT);
  pinMode(PWM3, OUTPUT);

  pinMode(INA4, OUTPUT);
  pinMode(INB4, OUTPUT);
  pinMode(PWM4, OUTPUT);
  
// Set pinmode to write command signal to LED.
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);

// Set pinmode to get command signal from infrared sensor
  pinMode (IF1, INPUT);
  pinMode (IF2, INPUT);
  pinMode (IF3, INPUT);
  pinMode (IF4, INPUT);

// Assign Servo variable to a servo pin
  myServo1.attach(servo1);

//===== Initialize Command =====
  // Initialize Motor Driver.
  digitalWrite(INA1, LOW);
  digitalWrite(INB1, LOW);
  analogWrite(PWM1, 0);

  digitalWrite(INA2, LOW);
  digitalWrite(INB2, LOW);
  analogWrite(PWM2, 0);
  
  digitalWrite(INA3, LOW);
  digitalWrite(INB3, LOW);
  analogWrite(PWM3, 0);
  
  digitalWrite(INA4, LOW);
  digitalWrite(INB4,LOW); 
  analogWrite(PWM4,0); 

  // Initialize Servo Motor, Set servo to Mid-point.
//  myServo1.write(90);
  
  // Open Serial port, Set baud rate for serial data transmission.
  Serial.begin(115200); // USB:Rx0,Tx0

  // Returns time(us)
  previousLoopTime = micros();

} // End SetUp


//======================= loop() ==========================
void loop () 
{
  loopTime = micros() - previousLoopTime;
  
  // if loop time is more than 10000 microseconds, do the next loop.
  // Limit Maximum feedback loop at 100Hz.
  if (loopTime >= 10000) 
  {
    // Set new loop time
    previousLoopTime = micros();

    // IF1, IF2 left - former, right - former
    ,






    /
    
    // IF3, IF4 left - latter, right - latter

  } // End if
} // End loop


//=============== Function Declaration ===============

  //===== Set Dead Band =====
  // If the input signal from receiver is in the band limit, set input signal to 0.0.
  double Deadband (double value, double limit)
  {
    double temp = 0.0;
    if (value >= limit) temp = value-limit;
    else if (value <= -limit) temp = value+limit;
    else temp = 0.0;
    return temp;
  }

  void SetMotorDirection(Motor mt, bool direc)
  {
    if(direc) //positive
    {
      switch(mt)
      {
        case Motor1:
            digitalWrite(INA1, LOW);
            digitalWrite(INB1, HIGH);
            break;
        case Motor2:
            digitalWrite(INA2, LOW);
            digitalWrite(INB2, HIGH);
            break;
        case Motor3:
            digitalWrite(INA3, LOW);
            digitalWrite(INB3, HIGH);
            break;
        case Motor4:
            digitalWrite(INA4, LOW);
            digitalWrite(INB4, HIGH);
            break;
      }
    }
    else //negtive
    {
      switch(mt)
      {
        case Motor1:
            digitalWrite(INA1, HIGH);
            digitalWrite(INB1, LOW);
            break;
        case Motor2:
            digitalWrite(INA2, HIGH);
            digitalWrite(INB2, LOW);
            break;
        case Motor3:
            digitalWrite(INA3, HIGH);
            digitalWrite(INB3, LOW);
            break;
        case Motor4:
            digitalWrite(INA4, HIGH);
            digitalWrite(INB4, LOW);
            break;
      }
    }
  }

    void SetServoAngle (unsigned int angle, unsigned int delay_time)
    {
      angle = angle > 180 ? 180 : angle;
      for (unsigned int i = 0; i < angle; i++)
      {
        myServo1.write(i);
        delay(delay_time);
      }
    }


