/** SERVO INIT **/
#include<Servo.h>

Servo Myservo1;
Servo Myservo2;
int pos=0;

/** BLUETOOTH INIT **/
// Basic Bluetooth LED Switch (For Zs-40 Version)
// Connect the Hc-06 module and turn on/off one LED
// On Android device, type "o" to turn LED on, "f" to turn off
//
// The HC-06 defaults to AT mode when first powered on.
// The default baud rate is 9600
// The Hc-06 requires all AT commands to be in uppercase. NL+CR should be added to the command string

#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 7); // RX | TX
// Connect the HC-06 TX to the Arduino RX on pin 2.
// Connect the HC-06 RX to the Arduino TX on pin 3 through a voltage divider.
// Connect a LED in serial with ~220 Ohm resistor to Pin 12 and ground.

char recievedChar;

void setup()
{
  // Servo init 
  pinMode(4,INPUT);
  pinMode(8,INPUT);
  Myservo1.attach(5);
  Myservo2.attach(9);

  // Bluetooth serial connection init
  Serial.begin(9600);
  Serial.println("Enter AT commands:");

  // HC-06 default serial speed is 9600
  BTserial.begin(9600);

  // Put motors in the right position
  Myservo1.write(39); //D string
  Myservo2.write(130);//A string
}

#define LEDPin 13

char lastChar = '\0';

void dataprint(char *parameterName, char dataNum)
{
  Serial.print(parameterName);
  Serial.print(",");
  Serial.print(dataNum);
}

void loop()
{
  // Keep reading from HC-06 and send to Arduino Serial Monitor
  if (BTserial.available()) { 
      recievedChar = BTserial.read();
      Serial.write(recievedChar);

      if (recievedChar == 'a' || recievedChar == 'd' || recievedChar == '1' || recievedChar == '2') {
        dataprint("GOT CHAR", recievedChar);
        lastChar = recievedChar;
      }

      if(lastChar == 'd'){ // d string
        Myservo1.write(62); //D string
        digitalWrite(LEDPin, HIGH);
        BTserial.write("LED is On \r\n");
      }
      else if(lastChar == 'a'){ // a string
        Myservo2.write(110); //A string
        digitalWrite(LEDPin, LOW);
        BTserial.write("LED is Off \r\n");
      }
      else if (lastChar == '1') {
        Myservo1.write(39); //D string
      } else if (lastChar == '2') {
        Myservo2.write(130);//A string
      }
      else  {
        Serial.write("Wrong Command");
      }
  }
  
  // Keep reading from Arduino Serial Monitor and send to HC-06
  if (Serial.available())
  {
    BTserial.write(Serial.read());
  }

  

/*
  if (digitalRead(4)==HIGH) {
    Myservo1.write(62); //D string
  } else if (digitalRead(4)==LOW) {
    Myservo1.write(39); //D string
  }
  
  if(digitalRead(8)==HIGH){
    Myservo2.write(110); //A string
    Serial.println("foo");
  }
  else if (digitalRead(8)==LOW) {
    Myservo2.write(130);//A string
    Serial.println("bar");
  }
  */
}
