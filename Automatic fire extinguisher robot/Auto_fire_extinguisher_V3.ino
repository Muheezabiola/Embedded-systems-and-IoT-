int in1=2, in2=3, in3=4, in4=5, en1=9, en2=10, pump=7, ir1, ir2, ir3, speed=255, speed2=100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);
  pinMode(pump, OUTPUT);
  pumping(0);
}

void loop() {
  ir1=analogRead(A1); ir2=analogRead(A2); ir3=analogRead(A3);
  //put your main code here, to run repeatedly:
  if (ir1<= 100 && ir2 >= 400 && ir3 >= 400) {
    tilt_right();
    delay(500);
    stop();
    pumping(1);
    delay(1000);
    pumping(0);
  }
  else if (ir2<= 100 && ir1 >= 400 && ir3 >= 400) {
    forward();
    delay(500);
    stop();
    pumping(1);
    delay(1000);
    pumping(0);
  }
  else if (ir3<= 100 && ir2 >= 400 && ir1 >= 400) {
    tilt_left();
    delay(500);
    stop();
    pumping(1);
    delay(1000);
    pumping(0);
  }
  else {
    stop();
    pumping(0);
  }


}


void left_turn() {
  Serial.println("Turning left");
  analogWrite(en1, speed);
  analogWrite(en2, speed);
  digitalWrite(in1, 1);
  digitalWrite(in2, 0);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
 }

  void right_turn() {
  Serial.println("Turning right");
  analogWrite(en1, speed);
  analogWrite(en2, speed);
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 0);
  digitalWrite(in4, 1);
 }

 void forward() {
  Serial.println("Moving forward");
  analogWrite(en1, speed);
  analogWrite(en2, speed);
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
 }

  void backward() {
  Serial.println("Moving backward");
  analogWrite(en1, speed);
  analogWrite(en2, speed);
  digitalWrite(in1, 1);
  digitalWrite(in2, 0);
  digitalWrite(in3, 0);
  digitalWrite(in4, 1);
 }

 void stop() {
  Serial.println("Car stopped");
  analogWrite(en1, 0);
  analogWrite(en2, 0);
  digitalWrite(in1, 0);
  digitalWrite(in2, 0);
  digitalWrite(in3, 0);
  digitalWrite(in4, 0);
 }

void tilt_right() {
  Serial.println("Tilting right");
  analogWrite(en1, speed2);
  analogWrite(en2, speed);
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
 }

 void tilt_left() {
  Serial.println("Tilting left");
  analogWrite(en1, speed);
  analogWrite(en2, speed2);
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
 }

void navigate(){
forward();
delay(1000);
tilt_right();
delay(1000);
tilt_left();
delay(1000);
backward();
delay(1000);
right_turn();
delay(500);
forward();
delay(1000);
left_turn();
delay(500);
}

void pumping(int x) {
  digitalWrite(pump,x);
}
// Serial.print("first: "); Serial.print(analogRead(A1));
// Serial.print("  second: "); Serial.print(analogRead(A2));
// Serial.print("  third: "); Serial.println(analogRead(A3));
// delay(200);