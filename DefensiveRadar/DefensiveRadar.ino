/**
 * @file    DefensiveRadar.ino
 * @author  Stéphane DELEBASSE - Nans DUMORTIER
 * @version V1.0.0
 * @date    2018/05/23
 * @brief   Description: this file is a program that allows a robot to scan its environment and fire at targets
 *
 * Function List:
 * 1. checkDistance : function that checks what's in front of the robot, if a target is in range, rescan the area and fire at it
 * 2. fire : function that fire balls contained in the tank
 * 
 * motorX.moveTo, motorX.getCurrentPosition, motorX.runTurns are functions provided by Makeblock
 * See : https://github.com/Makeblock-official/Me_EncodeDriver 
 * ultraSensor.distanceCm is another function provided by Makeblock
 *
**/

#include "MeOrion.h"
#include <Wire.h>
#include <SoftwareSerial.h>

MeUltrasonicSensor ultraSensor(PORT_6); /* Ultrasonic module can ONLY be connected to port 3, 4, 6, 7, 8 of base shield. */
MeEncoderMotor motor1(0x09, SLOT1);   //  motor at slot2
MeEncoderMotor motor2(0x09, SLOT2);   //  motor at slot2

void setup()
{
  Serial.begin(9600);
  // motor1 is responsible for the rotation that allows to scan an area
  motor1.begin(); 
  // motor2 is responsible for firing
  motor2.begin();
}

void loop()
{
  Serial.println("Moving left");
  float desiredAngle = 50;
  float desiredSpeed = 10;
  motor1.moveTo(desiredAngle, desiredSpeed);
  for(int i = 0; i < 20; i++){
    checkDistance(); // function that takes at least 100ms to run
  }


  Serial.println("Moving right");
  desiredAngle = -desiredAngle;
  motor1.moveTo(desiredAngle, desiredSpeed);
  for(int j = 0; j < 20; j++){
    checkDistance(); // function that takes at least 100ms to run
  }
}

void checkDistance()
// function that checks what's in front of the robot
// if a target is in range, rescan the area and fire at it
{ 
  float currentPos = motor1.getCurrentPosition();
  float distanceCm;
  distanceCm = ultraSensor.distanceCm();

  if(distanceCm > 5.0 && distanceCm <= 25.0){
    // rescan the area
    // memorize the position where the object is
    // (= where the distance between the robot and the
    // closest obstacle is the smallest)
    // move to that position
    // fire
    Serial.println("Target detected");
    motor1.moveTo(currentPos - 10, 15);
    Serial.println("Moving for scanning");
    float desiredAngle = currentPos + 10;
    Serial.println("Beginning scan");
    float desiredSpeed = 7;
    float distance2 = 30;
    float reading;
    float position2 = currentPos;
    motor1.moveTo(desiredAngle, desiredSpeed);
    for(int k = 0; k < 10; k++){
      reading = ultraSensor.distanceCm();
      if(distance2 > reading){
        distance2 = reading;
        position2 = motor1.getCurrentPosition();
      }
      delay(100);
    }
    Serial.println("Moving towards the target");
    Serial.println(position2);
    motor1.moveTo(position2, 15);
    delay(1000);
    Serial.println("Target's position reached");
    Serial.println("FIRE !");
    fire();
  }
 
  if (distanceCm <= 5.0){
    motor1.moveTo(currentPos, 10);
    delay(100);
    // fire almost instantly if the target is too close
    fire(); 
  }
  delay(100);
}

void fire()
{
  motor2.runTurns(0.5,200);
  delay(1000);
  Serial.println("Projectile launched");
}



