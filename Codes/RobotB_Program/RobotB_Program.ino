#include "configuration.h"

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
  
// Processed Input Signal
  int left_w = 0;
  int right_w = 0;
  
// Motor's Current
float currentValue1 = 0.0;
float currentValue2 = 0.0;
float currentValue3 = 0.0;
float currentValue4 = 0.0;
int currentLimit = 5;

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

    // Read input signal from receiver. PulseIn signal from Receiver vary between 1000 - 2000.
    // Substract 1500 as the offset to set new signal range from (1000, 2000) to (-500, 500) 
    // Also set Deadband limit to the input Signal

    input1 = pulseIn(CH1, HIGH) - 1500; //Channel 1 -400, 400
    input2 = pulseIn(CH2, HIGH) - 1500; //Channel 2 -400, 400
    input3 = pulseIn(CH3, HIGH) - 1500; //Channel 3 -400, 400
    input4 = pulseIn(CH4, HIGH) - 1500; //Channel 4 -400, 400
    input5 = pulseIn(CH5, HIGH) - 1500; //Channel 5 -510, 530
    input6 = pulseIn(CH6, HIGH) - 1500; //Channel 6 -510, 530
  
    input1 = Deadband(input1, 30); //Channel 1
    input2 = Deadband(input2, 30); //Channel 2
    input3 = Deadband(input3, 50); //Channel 3
    input4 = Deadband(input4, 30); //Channel 4
    input5 = Deadband(input5, 30); //Channel 5
    input6 = Deadband(input6, 30); //Channel 6
    
    // Read Motor's Current From Motor Driver
    // The resolution of Arduino analogRead is 5/1024 Volts/Unit. (10-Bit, Signal vary from 0 to 1023 units)
    // The resolution of Current Sensor from POLOLU VNH5019 is 0.14 Volt/Amp.
    // Convert analogRead signal(Volt) to Current(Amp) by multiply (5/1024)/0.14 = 0.035 Amp/Unit.
    currentValue1 = analogRead(CS1) * 0.035; // Motor Driver 1
    currentValue2 = analogRead(CS2) * 0.035; // Motor Driver 2
    currentValue3 = analogRead(CS3) * 0.035; // Motor Driver 3
    currentValue4 = analogRead(CS4) * 0.035; // Motor Driver 4

    //Moving - motor1, motor3
    out1 = -input1 - input2;
    out3 = input1 - input2;
    
    if (out1 > 0) SetMotorDirection (Motor1, false);
    else SetMotorDirection (Motor1, true);
    if (out3 > 0) SetMotorDirection (Motor3, true);
    else SetMotorDirection (Motor3, false);
    
    out1 = out1 > 0 ? out1 : -out1;
    out3 = out3 > 0 ? out3 : -out3;
    out1 = currentValue1 > currentLimit ? 0 : out1;
    out3 = currentValue3 > currentLimit ? 0 : out3;
    out1 = map (out1, 0, 800, 0, 255);
    out3 = map (out3, 0, 800, 0, 255);
    out1 = map (out1, 0, 150, 0, 255);
    out3 = map (out3, 0, 150, 0, 255);
    analogWrite (PWM1, out1);
    analogWrite (PWM3, out3);

    //Gripper - motor2
    out2 = input4;

    if (out2 < 0) SetMotorDirection (Motor2, true);
    else SetMotorDirection (Motor2, false);

    out2 = out2 > 0 ? out2 : -out2;
    out2 = currentValue2 > currentLimit ? 0 : out2;
    out2 = map(out2, 0, 400, 0, 255);
    out2 = map(out2, 0, 150, 0, 255);
    analogWrite (PWM2, out2);
    
//    // Print
//    Serial.print("M 1 = ");
//    Serial.print(input1);
//    Serial.print("\t M 2 = ");
//    Serial.print(input2);
//    Serial.print("\t M 3 = ");
//    Serial.print(input3);
//    Serial.print("\t M 4 = ");
//    Serial.print(input4);
//    Serial.print("\t M 5 = ");
//    Serial.print(input5);
//    Serial.print("\t M 6= ");
//    Serial.print(input6);
//    Serial.print("\t LoopTime = ");
//    Serial.println(loopTime);
    Serial.print("\t out1 = ");
    Serial.println(out1);
    Serial.print("\t out2 = ");
    Serial.println(out2);
   
  
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


