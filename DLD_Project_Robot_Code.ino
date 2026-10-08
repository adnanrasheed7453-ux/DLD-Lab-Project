#include <AFMotor.h>
#include <SoftwareSerial.h>

// HC-05
SoftwareSerial BT(2, 3);   // RX, TX

// Ultrasonic
#define TRIG_PIN A0
#define ECHO_PIN A1

#define buzzer A2

// Motors
AF_DCMotor leftMotor(2, MOTOR12_1KHZ);
AF_DCMotor rightMotor(4, MOTOR12_1KHZ);

long duration;
int distance;
int cmd;

void setup()
{
  Serial.begin(9600);
  BT.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(buzzer , OUTPUT);

  leftMotor.setSpeed(180);
  rightMotor.setSpeed(180);

  stopRobot();
}

void loop()
{
  // Bluetooth Command
  if (BT.available() > 0)
  {
    cmd = BT.read();

    Serial.print("Command: ");
    Serial.println(cmd);

    switch (cmd)
    {
      case 1:
        forward();
        break;

      case 2:
        backward();
        break;

      case 3:
        left();
        break;

      case 4:
        right();
        break;

        case 6:
        right();
        delay(800);
        stopRobot();
        break;

         case 7:
        left();
        delay(800);
        stopRobot();
        break;

      case 5:
        stopRobot();
        break;
    }
  }

  // Ultrasonic Distance
  distance = readDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Safety Stop
  if (distance > 0 && distance < 20)
  {
    digitalWrite(buzzer,HIGH);
    stopRobot();
  }else{digitalWrite(buzzer,LOW);}

  delay(30);
}

int readDistance()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0)
    return 0;

  return duration * 0.034 / 2;
}

void forward()
{
  leftMotor.run(FORWARD);
  rightMotor.run(FORWARD);
}

void backward()
{
  leftMotor.run(BACKWARD);
  rightMotor.run(BACKWARD);
}

void right() 
{
  leftMotor.run(BACKWARD);
  rightMotor.run(FORWARD);
}

void left()
{
  leftMotor.run(FORWARD);
  rightMotor.run(BACKWARD);
}

void stopRobot()
{
  leftMotor.run(RELEASE);
  rightMotor.run(RELEASE);
}
