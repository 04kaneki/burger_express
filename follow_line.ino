#include <NIBOburger.h>
#include <PID_v1.h>

enum {
  CENTER = 1,
  CENTER_RIGHT = 2,
  CENTER_LEFT = 3,
  RIGHT = 4,
  LEFT = 5,
  RIGHT_OUTSIDE = 6,
  LEFT_OUTSIDE = 7,
  LOST = 8,
};

uint8_t POSITION = LOST;
uint8_t POSITION_OLD = LOST;

float l_max, c_max, r_max;
float l, c, r;   
double l_minus_r;
int16_t pwm_speed = 800;        
int16_t pwm_left = pwm_speed;  
int16_t pwm_right = pwm_speed;

char key = ' ';
double output_left, output_right, setpoint = 0.0;
double kp = 2.0 , kd = 0.5, ki = 0;

PID PID_RIGHT(&l_minus_r, &output_right, &setpoint, kp, ki, kd, REVERSE);
PID PID_LEFT(&l_minus_r, &output_left, &setpoint, kp, ki, kd, DIRECT);


void setup() {
 
  // put your setup code here, to run once:
  NIBOburger.begin();
  SurfaceSensor.begin();
  Engine.begin();

  PID_RIGHT.SetMode(AUTOMATIC);
  PID_RIGHT.SetSampleTime(30);
 
  PID_LEFT.SetMode(AUTOMATIC);
  PID_LEFT.SetSampleTime(30);

  calibrate_floor();
  Engine.setPWM(pwm_left,pwm_right);
}


void loop() {

  // check hardware
  NIBOburger.waitAnalogUpdate();
  NIBOburger.checkVoltage();

  // follow line
  get_position();
  follow_line();
  position_leds();
}

// calibrates the floor
void calibrate_floor() {
  
  POSITION = LOST;
  NIBOburger.setLeds(1,1,1,1);

  key = ' ';
  while (key != 'A') {
    key = NIBOburger.getKeyChar();
  }
  NIBOburger.setLeds(0,0,0,0);

  c_max = 100.0 / SurfaceSensor.getC(0);
  l_max = 100.0 / SurfaceSensor.getL(0);
  r_max = 100.0 / SurfaceSensor.getR(0);

  while (key != 'B') {
    key = NIBOburger.getKeyChar();
  }

  while (key != 'C') {
    get_position();
    position_leds();
    key = NIBOburger.getKeyChar();
  }
  key = ' ';
}

// determine the current pos
void get_position() {
  
  float c = c_max * SurfaceSensor.getC(0);
  float l = l_max * SurfaceSensor.getL(0);
  float r = r_max * SurfaceSensor.getR(0);
  l_minus_r = l - r;

  if (c < 40.) {
    if (abs(l_minus_r) < 20.) {
      POSITION = CENTER;
    } else if (abs(l_minus_r) == (l_minus_r)) {
      POSITION = CENTER_RIGHT;
    } else {
      POSITION = CENTER_LEFT;
    }
  } else {
    if (r < 80.) {
      POSITION = RIGHT;
    } else if (l < 80.) {
      POSITION = LEFT;
    } else if (POSITION == RIGHT) {
      POSITION = RIGHT_OUTSIDE;
    } else if (POSITION == LEFT) {
      POSITION = LEFT_OUTSIDE;
    }
  }

}

// follows the line acording to current pos
void follow_line() {

  switch(POSITION) {
    case LEFT_OUTSIDE:
    pwm_left = -pwm_speed/2;
    pwm_right = pwm_speed/2;
    break;
    case RIGHT_OUTSIDE:
    pwm_right = -pwm_speed/2; 
    pwm_left = pwm_speed/2;
    break;

    default:
    while (!PID_RIGHT.Compute());
    while (!PID_LEFT.Compute());

    pwm_left = pwm_speed - (output_left * pwm_speed / 255.);
    pwm_right = pwm_speed - (output_right * pwm_speed / 255.);

    pwm_left = constrain(pwm_left, 0, 1024);
    pwm_right = constrain(pwm_right, 0, 1024);
  }

  Engine.setPWM(pwm_left,pwm_right);

}

// changes the LED acording to the current pos
void position_leds() {
  if (POSITION_OLD == POSITION) return;
  switch (POSITION) {
    case CENTER:
      NIBOburger.setLeds(0,1,1,0);
      break;
    case CENTER_RIGHT:
      NIBOburger.setLeds(0,0,1,0);
      break;
    case CENTER_LEFT:
      NIBOburger.setLeds(0,1,0,0);
      break;
    case RIGHT:
      NIBOburger.setLeds(0,0,1,1);
      break;
    case LEFT:
      NIBOburger.setLeds(1,1,0,0);
      break;
    case RIGHT_OUTSIDE:
      NIBOburger.setLeds(0,0,0,1);
      break;
    case LEFT_OUTSIDE:
      NIBOburger.setLeds(1,0,0,0);
 
  }
  POSITION_OLD = POSITION;
}
