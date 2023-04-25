//#include <LiquidCrystal_I2C.h> // LCD
//#include // Motors
#include <IRremote.h> // IR Recv.

//LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD Initialization and Setup on 0x27 default 
// We may want to switch this to a 7 segment display, much cheaper

// IR Receiver Pins and Vars
const int irRecvPin = 13; // IR Recieve Pin
int cmd;

// Motor Pins and Vars
const int motorLPin = 9; // Left Motor Pin (IN1)
const int motorLRev = 5; // Left Motor Reverse Pin (IN2)
const int motorRPin = 10; // Right Motor Pin (IN3)
const int motorRRev = 6; // Right Motor Reverse Pin (IN4)

const int speed = 255;

void setup() {
    Serial.begin(9600); // Serial Readout is 9600
    pinMode(motorRRev,OUTPUT);
    pinMode(motorLRev,OUTPUT);
    pinMode(motorRPin,OUTPUT);
    pinMode(motorLPin,OUTPUT);

    //lcd.init(); // LCD Initialization 
    //lcd.backlight(); // Turning on Backlight
    //lcd.clear(); // Clearing LCD

    IrReceiver.begin(irRecvPin); // Receiver enables

    digitalWrite(motorLRev, LOW); // Ensuring Motor Movement is Forward
    digitalWrite(motorRRev, LOW);

    //lcd.print('Strt Cmplt'); // May switch to 7 segment
}

void loop() {
    //Serial.println("Pre-IR");
    if (IrReceiver.decode()) { // On IR Receive, decode
        IrReceiver.resume(); // resume reception
        Serial.print(IrReceiver.decodedIRData.command); // Print Reception
        cmd = IrReceiver.decodedIRData.command; // Set as CMD
        if (cmd == 201) { // Check CMD for Mode Button
        Serial.print("Received");
            while (true) {  // Infinite loop to be broken if Power is pressed
                if (IrReceiver.decode()){ // IR Receiver for shutdown
                    IrReceiver.resume();
                    Serial.print(IrReceiver.decodedIRData.command);
                    cmd = IrReceiver.decodedIRData.command;
                    if (cmd == 114/*Stop Button*/) {
                        stop();
                    }
                    else if (cmd == 198/*Right Button*/) {
                        turnRight();
                    }
                    else if (cmd == 199/*Left Button*/) {
                        turnLeft();
                    }
                    else if (cmd == 246/*Forward Button*/) {
                        forward();
                    }
                    else if (cmd == 247/*Reverse Button*/) {
                        reverse();
                    }
                    if (cmd == 224) {
                        stop();
                        break;
                    }
                }
            }
        }
    }
}


void forward() {
    stop();
    analogWrite(motorLPin, speed);
    analogWrite(motorRPin, speed);
    return;
}

void turnLeft() {
    digitalWrite(motorLPin, LOW);
    analogWrite(motorRPin, speed);
    return;
}

void turnRight() {
    digitalWrite(motorRPin, LOW);
    analogWrite(motorLPin, speed);
    return;
}

void reverse() {
    stop();
    analogWrite(motorRRev, speed);
    analogWrite(motorLRev, speed);
    return;
}

void stop(){
    Serial.print('Stopped');
    digitalWrite(motorLPin, LOW);
    digitalWrite(motorRPin, LOW);
    digitalWrite(motorLRev, LOW);
    digitalWrite(motorRRev, LOW);
    return;
}

