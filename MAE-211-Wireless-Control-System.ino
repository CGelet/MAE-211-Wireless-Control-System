/* MAE-211 Project */

#include <IRremote.h> // IR Recv. Librariy

// IR Receiver Pins and Vars
const int irRecvPin = 13; // IR Recieve Pin
int cmd; /* Intializing cmd variable which will be used to remember the button pressed on the remote
and will keep this in memory until another button is pressed to keep completing that action until
another button is pressed */

// Motor Pins and Vars
const int motorLPin = 9; // Left Motor Pin (IN1)
const int motorLRev = 5; // Left Motor Reverse Pin (IN2)
const int motorRPin = 10; // Right Motor Pin (IN3)
const int motorRRev = 6; // Right Motor Reverse Pin (IN4)

const int speed = 255; // set speed value

void setup() {
    Serial.begin(9600); // Serial Readout is 9600
    pinMode(motorRRev,OUTPUT);// Pinmode output for motor variables
    pinMode(motorLRev,OUTPUT);
    pinMode(motorRPin,OUTPUT);
    pinMode(motorLPin,OUTPUT);

    IrReceiver.begin(irRecvPin); // Receiver enabled for reception of IR signals

    digitalWrite(motorLRev, LOW); // Ensuring Motor Movement is Forward
    digitalWrite(motorRRev, LOW);

}

void loop() {
    Serial.println("Pre-IR"); // Serial Indication that the program is running and is ready for remote start
    if (IrReceiver.decode()) { // On receiving an IR signal, decode it to a number
        IrReceiver.resume(); // resume reception that way it can receive another IR signal
        Serial.print(IrReceiver.decodedIRData.command); // Print the reception code from the last IR signal
        cmd = IrReceiver.decodedIRData.command; // Set the IR signal as the variable cmd
        if (cmd == 201) { // Check CMD for Mode Button on the remote used to start the entire system
        Serial.print("Received"); // Notification that the system has begun and received the correct code
            while (true) {  // Infinite loop to be broken if Power is pressed
                if (IrReceiver.decode()){ // IR Receiver for shutdown
                    IrReceiver.resume(); // resume reception that way it can receive another IR signal
                    Serial.print(IrReceiver.decodedIRData.command);// Print the reception code from the last IR signal
                    cmd = IrReceiver.decodedIRData.command; // Set the IR signal as the variable cmd.
                    if (cmd == 114/*Stop Button*/) { // Check for the Central button established as the stop button 
                        stop();
                    }
                    else if (cmd == 198/*Right Button*/) { // Check for the right button established as the turn right button
                        turnRight(); // turn right function
                    }
                    else if (cmd == 199/*Left Button*/) { // Check for the left button established as the turn left button
                        turnLeft(); // turn left function
                    }
                    else if (cmd == 246/*Forward Button*/) { // Check for the forward button established as the forward button
                        forward(); // Forward driving function
                    }
                    else if (cmd == 247/*Reverse Button*/) { // Check for the reverse button established as the reverse button
                        reverse(); // reverse driving function
                    }
                    if (cmd == 224) {// Check CMD for Mode Button on the remote used to break the entire system
                        stop(); // Running the stop function to stop the robots movement through setting power output
                        break;// break the loop and send it back to "Pre-IR"
                    }
                }
            }
        }
    }
}


void forward() {// Forward driving function
    stop(); // Quickly stop the robot to ensure nothing brakes or reverses
    analogWrite(motorLPin, speed); // Writes both motors to high and at the same speed
    analogWrite(motorRPin, speed); // Writes both motors to high and at the same speed
    return; // returns to the orginial loop 
}

void turnLeft() {// turn left function
    digitalWrite(motorLPin, LOW); // Setting L Motor to off
    analogWrite(motorRPin, speed); // Setting L Motor to speed const
    return; // returns to the orginial loop 
}

void turnRight() { //  turn right function
    digitalWrite(motorRPin, LOW); // Setting R Motor to off
    analogWrite(motorLPin, speed); // Setting L Motor to speed const
    return; // returns to the orginial loop 
}

void reverse() { // Reverse function
    stop(); // Quickly stop the robot to ensure nothing brakes or goes forward
    analogWrite(motorRRev, speed); // Set the reverse pins to max speed
    analogWrite(motorLRev, speed);
    return; // returns to the orginial loop 
}

void stop(){ // Stop Function
    Serial.print('Stopped'); // Stating stop in Serial Monitor
    digitalWrite(motorLPin, LOW); // Setting ALL motor pins to low to stop everything.
    digitalWrite(motorRPin, LOW);
    digitalWrite(motorLRev, LOW);
    digitalWrite(motorRRev, LOW);
    return; // returns to the orginial loop 
}

// End of Code