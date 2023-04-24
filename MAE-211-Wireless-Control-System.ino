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
const int motorLRev = 10; // Left Motor Reverse Pin (IN2)
const int motorRPin = 11; // Right Motor Pin (IN3)
const int motorRRev = 12; // Right Motor Reverse Pin (IN4)

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
    Serial.println("Pre-IR");
    if (IrReceiver.decode()) { // On IR Receive, decode
        IrReceiver.resume(); // resume reception
        Serial.print(IrReceiver.decodedIRData.command); // Print Reception
        cmd = IrReceiver.decodedIRData.command; // Set as CMD
        if (cmd == 70) { // Check CMD for Mode Button
        Serial.print("Received");
            while (true) {  // Infinite loop to be broken if Power is pressed
                if (IrReceiver.decode()){ // IR Receiver for shutdown
                    IrReceiver.resume();
                    Serial.print(IrReceiver.decodedIRData.command);
                    cmd = IrReceiver.decodedIRData.command;
                    if (cmd == /*Stop Button*/) {
                        stop();
                    }
                    else if (cmd == /*Right Button*/) {
                        turnRight();
                    }
                    else if (cmd == /*Left Button*/) {
                        turnLeft();
                    }
                    else if (cmd == /*Forward Button*/) {
                        forward();
                    }
                    else if (cmd == /*Reverse Button*/) {
                        reverse();
                    }
                    if (cmd == 69) {
                        stop();
                        break;
                    }
                }
            }
        }
    }
}


void forward() {
    digitalWrite(motorLPin, HIGH);
    digitalWrite(motorRPin, HIGH);
    return;
}

void turnRight() {
    digitalWrite(motorLPin, LOW);
    digitalWrite(motorRPin, HIGH);
    return;
}

void turnLeft() {
    digitalWrite(motorRPin, LOW);
    digitalWrite(motorLPin, HIGH);
    return;
}

void reverse() {
    stop();
    digitalWrite(motorRRev, HIGH);
    digitalWrite(motorLRev, HIGH);
    delay(1000);
    shift();
    delay(1000);
    digitalWrite(motorRRev, LOW);
    digitalWrite(motorLRev, LOW);
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

