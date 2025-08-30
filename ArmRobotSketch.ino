#include "IR_remote.h"
#include "keymap.h"

IRremote ir(3);

#define MAX_SEQUENCE 50

int seq = 0;
int seq_Array[MAX_SEQUENCE];
bool obstacleMode = false; 

unsigned long int total_fwdT[10];
unsigned long int total_bwdT[10];
unsigned long int total_leftT[10];
unsigned long int total_rightT[10];
unsigned long int total_stpT[10];

float checkdistance() {
    digitalWrite(12, LOW);
    delayMicroseconds(2);
    digitalWrite(12, HIGH);
    delayMicroseconds(10);
    digitalWrite(12, LOW);
    float distance = pulseIn(13, HIGH) / 58.00;
    delay(10);
    return distance;
}

void RecordMovement(int command) {
    if (seq < MAX_SEQUENCE) {
        seq_Array[seq++] = command;
    }
}

void Repeat() {
    for (int i = 0; i < seq; i++) {
        switch (seq_Array[i]) {
            case IR_KEYCODE_UP:
                Move_Forward(100);
                delay(300);
                Stop();
                break;
            case IR_KEYCODE_DOWN:
                Move_Backward(100);
                delay(300);
                Stop();
                break;
            case IR_KEYCODE_LEFT:
                Rotate_Left(70);
                delay(300);
                Stop();
                break;
            case IR_KEYCODE_RIGHT:
                Rotate_Right(70);
                delay(300);
                Stop();
                break;
            case IR_KEYCODE_OK:
                Stop();
                break;
        }
    }
}

void SavePath() {
  
}

void ClearPath() {
    seq = 0;
}

void ObstacleAvoidance() {
    float Avoidance_distance = checkdistance();

    if (Avoidance_distance <= 25) {
        if (Avoidance_distance <= 15) {
            Stop();
            delay(100);
            Move_Backward(100);
            delay(600);
        } else {
            Stop();
            delay(100);
            Rotate_Left(100);
            delay(600);
        }
    } else {
        Move_Forward(70);
    }
}

void setup() {
    pinMode(12, OUTPUT);
    pinMode(13, INPUT);
    pinMode(2, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(6, OUTPUT);
}

void loop() {
    int key = ir.getIrKey(ir.getCode(), 1);

    // Checking obstacle avoidance
    if (obstacleMode) {
        ObstacleAvoidance();
        if (key == IR_KEYCODE_OK) { 
            obstacleMode = false;
            Stop();
        }
        return;
    }

    // IR keys operation
    if (key == IR_KEYCODE_UP) {
        Move_Forward(100);
        RecordMovement(IR_KEYCODE_UP);
        delay(300);
        Stop();
    } else if (key == IR_KEYCODE_DOWN) {
        Move_Backward(100);
        RecordMovement(IR_KEYCODE_DOWN);
        delay(300);
        Stop();
    } else if (key == IR_KEYCODE_LEFT) {
        Rotate_Left(70);
        RecordMovement(IR_KEYCODE_LEFT);
        delay(300);
        Stop();
    } else if (key == IR_KEYCODE_RIGHT) {
        Rotate_Right(70);
        RecordMovement(IR_KEYCODE_RIGHT);
        delay(300);
        Stop();
    } else if (key == IR_KEYCODE_OK) {
        Stop();
        RecordMovement(IR_KEYCODE_OK);
    } else if (key == IR_KEYCODE_1) {
        Repeat();
    } else if (key == IR_KEYCODE_2) { //Memorize the path
        SavePath();
    } else if (key == IR_KEYCODE_3) { // Clears the path
        ClearPath();
    } else if (key == IR_KEYCODE_5) { // Obstacle avoidance mode
        obstacleMode = true;
    }
}

void Stop() {
    digitalWrite(2, LOW);
    analogWrite(5, 0);
    digitalWrite(4, HIGH);
    analogWrite(6, 0);
}

void Move_Backward(int speed) {
    digitalWrite(2, LOW);
    analogWrite(5, speed);
    digitalWrite(4, HIGH);
    analogWrite(6, speed);
}

void Rotate_Left(int speed) {
    digitalWrite(2, LOW);
    analogWrite(5, speed);
    digitalWrite(4, LOW);
    analogWrite(6, speed);
}

void Rotate_Right(int speed) {
    digitalWrite(2, HIGH);
    analogWrite(5, speed);
    digitalWrite(4, HIGH);
    analogWrite(6, speed);
}

void Move_Forward(int speed) {
    digitalWrite(2, HIGH);
    analogWrite(5, speed);
    digitalWrite(4, LOW);
    analogWrite(6, speed);
}
