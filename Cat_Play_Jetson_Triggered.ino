#include <Servo.h>

Servo pan;
Servo tilt;
int pan_position = 80;
int tilt_position = 80;
int start_tilt_position = 80;
int start_pan_position = 80;
int end_tilt_position = 80;
int end_pan_position = 80;
int min_pan = 60;
int max_pan = 120;
int min_tilt = 60;
int max_tilt = 120;
int laser = 2;

#define wig 5
#define wig_pause 40

bool laser_active = false;
bool play_now = false;


void setup() {
  Serial.begin(9600);
  pinMode(laser, OUTPUT);
  pan.attach(9);
  tilt.attach(10);
  pan.write(pan_position);
  tilt.write(tilt_position);
  delay(300);
}

void loop() {
  get_last_command();
  if (play_now) {
    digitalWrite(laser, HIGH);
    play_with_cat();
  } else {
    digitalWrite(laser, LOW);
  }
}

void get_last_command() {
  if (Serial.available() > 0) {
    char letter_in = (char)Serial.read();
    if (letter_in == '1') {
      play_now = true;
    } else if (letter_in == '0') {
      play_now = false;
    } else {
      Serial.println(letter_in);
    }
  }
}

void ptu_test() {
  digitalWrite(laser, laser_active);
  start_tilt_position = 50;
  end_tilt_position = 110;
  tilt_position = start_tilt_position;
  tilt.write(tilt_position);
  delay(200);
  for (int step=0; step<60; step++) {
    tilt_position = map(step, 0, 60, start_tilt_position, end_tilt_position);
    tilt.write(tilt_position);
    delay(15);
  }
  tilt_position = end_tilt_position;
  delay(200);
  tilt_position = 80;
  tilt.write(tilt_position);
  delay(200);

  start_pan_position = 50;
  end_pan_position = 110;
  pan_position = start_pan_position;
  pan.write(pan_position);
  delay(200);
  for (int step=0; step<60; step++) {
    pan_position = map(step, 0, 60, start_pan_position, end_pan_position);
    pan.write(pan_position);
    delay(15);
  }
  pan_position = end_pan_position;
  delay(200);
  pan_position = 80;
  pan.write(pan_position);
  delay(200);

  delay(1000);
  laser_active = !laser_active;
}

void play_with_cat() {
  delay(random(500,3000));
  // Go to a new position, sometimes quickly, usually slowly.
  if (random(0,4)) slow_move();
  else quick_move();
  // Sometimes wiggle there.
  if (random(0,2)) wiggle();
}

void slow_move() {
  int end_pan_position = random(min_pan,max_pan);
  int end_tilt_position = random(min_tilt,max_tilt);
  int steps = random(20,40);
  for (int i=0; i<steps; i++) {
    pan.write(map(i, 0, steps, pan_position, end_pan_position));
    tilt.write(map(i, 0, steps, tilt_position, end_tilt_position));
    delay(40);
  }
  pan_position = end_pan_position;
  tilt_position = end_tilt_position;
}

void quick_move() {
  pan_position = random(60, 120);
  tilt_position = random(60, 120);
  int steps = random(10,15);
  for (int i=0; i<steps; i++) {
    pan.write(map(i, 0, steps, pan_position, end_pan_position));
    tilt.write(map(i, 0, steps, tilt_position, end_tilt_position));
    delay(40);
  }
  pan_position = end_pan_position;
  tilt_position = end_tilt_position;}

void wiggle() {
  int randomom_wiggle = random(0,4);
  int wiggle_count = random(3,7);
  switch(randomom_wiggle) {
    case 0:
      for (int i = 0; i<wiggle_count; i++) horizontal_wiggle();
      break;
    case 1:
      for (int i = 0; i<wiggle_count; i++) vertical_wiggle();
      break;
    case 2:
      for (int i = 0; i<wiggle_count; i++) diagonal_wiggle_1();
      break;
    case 3:
      for (int i = 0; i<wiggle_count; i++) diagonal_wiggle_2();
      break;
    default:
      for (int i = 0; i<wiggle_count; i++) horizontal_wiggle();    
  }
}

void horizontal_wiggle() {
  pan_position+=wig;
  pan.write(pan_position);
  delay(wig_pause);
  pan_position-=wig;
  pan.write(pan_position);
  delay(wig_pause);
}

void vertical_wiggle() {
  tilt_position+=wig;
  tilt.write(tilt_position);
  delay(wig_pause);
  tilt_position-=wig;
  tilt.write(tilt_position);
  delay(wig_pause);
}

void diagonal_wiggle_1() {
  pan_position+=wig;
  tilt_position+=wig;
  pan.write(pan_position);
  tilt.write(pan_position);
  delay(wig_pause);
  pan_position-=wig;
  tilt_position-=wig;
  pan.write(pan_position);
  tilt.write(tilt_position);
  delay(wig_pause);
}

void diagonal_wiggle_2() {
  pan_position+=wig;
  tilt_position-=wig;
  pan.write(pan_position);
  tilt.write(pan_position);
  delay(wig_pause);
  pan_position-=wig;
  tilt_position+=wig;
  pan.write(pan_position);
  tilt.write(tilt_position);
  delay(wig_pause);
}


