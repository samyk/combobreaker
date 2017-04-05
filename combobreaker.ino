/*

 C-C-C-C-Combo Breaker
 by samy kamkar
 http://samy.pl/combobreaker
 
 A motorized, battery powered, 3D printed, 
 Arduino-based combination lock cracking device.
 
 current hardware: 
 Stepper Motor (dial): 28STH32-0674B (3.8V 0.67A stepper)
 Rotary Encoder: HKT22-3531 optical rotary encoder (1200steps)
 Servo Motor (shackle): Batan S1213 (with analog feedback)
 Stepper Driver: Allegro A3967 or Allegro A4988 (EasyDriver, 750mA, 1/8th step)
 Microcontroller: Arduino Nano ATmega328P @ 5V
 
 Stepper/Arduino Power: 3S LiPo battery (12V power supply also works)
 Servo Power: L7805 Voltage Regulator
 
 */

// Servo requirements: at least 6.5kg/cm of torque

// TODO: CW could be autodetected by the rotary encoder
// Depending on your stepper wiring, you may need to change CW to true or false
#define CW true // clockwise
#define CCW !CW // counter-clockwise

// Servo details (lifts the shackle)
#include <Servo.h>
#include <Encoder.h>

#define SERVO_DEFAULT 30
#define SERVO_MIN 0
#define SERVO_MAX 200
#define SERVO_PIN 9
#define SERVO_FEEDBACK_PIN A0
#define FB_DIFF 10
#define FB_TOLERANCE 3.5
#define FB_DELAY 500

// Encoder details (pins)
#define ENCODER1 2 // 5
#define ENCODER2 3 // 6
#define ENCODER_STEPS 1200 // 1200 per rotation

// Stepper driver details (controls the dial)
#define DIR_PIN  5 // 2
#define STEP_PIN 6 // 3


#define STEPS 200
#define MICROSTEPS 8 // was 8 on easydriver

// fastest (us) we can get stepper moving without skipping
// #define MIN_DELAY (1600 / MICROSTEPS) // was 200
#define MIN_DELAY 500

// Number of digits on a Master combo lock
#define DIGITS 40

// C's % is remainder instead of modulo
#define mod(a, b) ((a % b + b) % b)

// For debug
#define BAUDRATE 9600

// this means we can rotate *consistently* 3.125 times per second (187.5rpm):
// 1sec in us / min_delay / (steps * microsteps)
// 1_000_000  / 200       / (200   * 8) = 3.125
int current_digit = 0;

Servo shackle;
Encoder myEnc(ENCODER1, ENCODER2);
//int servo_pos = 0;
long oldPosition;
int mini, maxi, minFeedback, maxFeedback;

void setup() 
{
  Serial.begin(BAUDRATE);

  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(SERVO_FEEDBACK_PIN, INPUT);
  shackle.attach(SERVO_PIN); 
  oldPosition = myEnc.read();

  calibrateServo();
}

void loop()
{
  brute(-1, -1, -1);

  for (int i = 0; i < ceil(DIGITS/3); i++)
  {
    // turn to new position
    spinto(0, i * 3, CCW);

    // hold shackle high to find left and right
    shackleHigh();
    spinto(0, (i-1)*3, CW);
    //    delay(100);

    Serial.print("spin low ");
    Serial.print(oldPosition);
    Serial.print(" ");
    spinto(0, (i+1)*3, CCW);
    //    delay(100);
    Serial.print(oldPosition);
    Serial.println("");

    //    delay(100);
    spinto(0, i * 3, CCW);

    shackleMid();
    delay(2000);
  }

  //  spinto(1, 0, CW);
  //  brute(16, -1, 16);
  //  pin(0, 0, 0);
  //  openShackle();
}

// pulls the shackle up
// NOTE: if you do this, you should immediately
// detect if the servo actually moved up, and if not,
// return back to shackleMid(), otherwise you'll be
// putting stress on the motor and 3d printed case
void shackleHigh()
{
  shackle.write(maxi + (FB_DIFF*(FB_TOLERANCE-1)));
}

// puts the shackle in middle position
void shackleMid()
{
  shackle.write(maxi);
}

// attempts to open the shackle *safely*
boolean openShackle()
{
  int fb;
  boolean ret = false;

  shackle.write(maxi + (FB_DIFF*FB_TOLERANCE*1.5));
  delay(300);

  fb = analogRead(SERVO_FEEDBACK_PIN);
  if (fb - maxFeedback > FB_DIFF*FB_TOLERANCE)
  {
    Serial.println("OPENED!");
    ret = true;
  }
  else
  {
    ret = false;
    Serial.println("not opened :(");
  }

  shackleMid();
  return ret;
}

// detect the middle and high positions of the servo
void calibrateServo()
{
  int fb;
  int i = SERVO_DEFAULT;

  shackle.write(i);
  delay(500);
  fb = analogRead(SERVO_FEEDBACK_PIN);

  Serial.println(fb);
  for (i -= FB_DIFF; i > SERVO_MIN; i -= FB_DIFF)
  {
    Serial.print("i=");
    Serial.print(i);
    shackle.write(i);
    mini = i;
    delay(FB_DELAY);
    minFeedback = analogRead(SERVO_FEEDBACK_PIN);
    Serial.print(" fb=");
    Serial.println(minFeedback);

    // if difference is less than 10, we can't move!
    if (fb - minFeedback < FB_DIFF)
    {
      mini = i + (FB_DIFF*FB_TOLERANCE);
      Serial.println("crap!");
      break;
    }
    fb = minFeedback;

  }


  for (i+=FB_DIFF*2; i <= SERVO_MAX; i += FB_DIFF)
  {
    Serial.print("i=");
    Serial.print(i);
    shackle.write(i);
    maxi = i;
    delay(FB_DELAY);
    maxFeedback = analogRead(SERVO_FEEDBACK_PIN);
    Serial.print(" fb=");
    Serial.println(maxFeedback);

    // if difference is less than 10, we can't move!
    if (maxFeedback - fb < FB_DIFF)
    {
      maxi = i - (FB_DIFF*FB_TOLERANCE);
      Serial.print("crap!! changing to ");
      Serial.println(maxi);
      shackleMid();
      break;
    }
    fb = maxFeedback;

  }

  Serial.print("mini=");
  Serial.print(mini);
  Serial.print(" maxi=");
  Serial.print(maxi);
  Serial.println("");
}

// brute force pin numbers based off of the
// mathematical congruencies of the lock
void brute(int pin1, int pin2, int pin3)
{
  int si = pin1, sj = pin2, sk = pin3;

  if (pin1 == -1)
    si = pin2 >= 0 ?
    (pin2 + 2) % 4 :
    pin3 >= 0 ?
    pin3 % 4 : 0;

  if (pin2 == -1)
    sj = pin1 >= 0 ?
    (pin1 + 2) % 4 :
    pin3 >= 0 ?
    (pin3 + 2) % 4 : 0;

  if (pin3 == -1)
    sk = pin1 >= 0 ?
    pin1 % 4 :
    pin2 >= 0 ?
    (pin2 + 2) % 4 : 0;

  Serial.println(si);
  Serial.println(sj);
  Serial.println(sk);
  Serial.println("go");

  for     (int i = si; (pin1 == i || pin1 == -1) && i < DIGITS; i += 4)
    for   (int j = sj; (pin2 == j || pin2 == -1) && j < DIGITS; j += 4)
      for (int k = sk; (pin3 == k || pin3 == -1) && k < DIGITS; k += 4)
      {
        Serial.print("testing: ");
        Serial.print(i);
        Serial.print(" ");
        Serial.print(j);
        Serial.print(" ");
        Serial.print(k);
        Serial.println();

        pin(i, j, k);
        if (openShackle())
          return;

        delay(100);
      }

}

// enter a full combination through the dial
void pin(int pin1, int pin2, int pin3)
{
  // clockwise 3 times, then to first number  
  spinto(3, pin1, CW);

  // counterclockwise once, then to second
  spinto(1, pin2, CCW);

  // clockwise to third number
  spinto(0, pin3, CW);
}

// spin to a specific digit,
// possibly with some additional rotations
void spinto(int spins, int digit, boolean cw)
{
  digitalWrite(DIR_PIN, cw ? HIGH : LOW);

  // spin some amount all the way?
  for (int i = 0; i < spins; i++)
  {
    step(STEPS);
//    delay(1000);
  }

  // track where we are for next time (get set later)
  int tmp = digit;
  Serial.print("going to ");
  Serial.print(digit);
  Serial.print(" cur=");
  Serial.print(current_digit);

  if (cw == CW)
    digit = (DIGITS - digit) + current_digit;
  else
    digit = digit - current_digit;

  digit = mod(digit, DIGITS);

  // track where we are for next time
  current_digit = tmp;

  Serial.print(" moving ");
  Serial.println(digit);
  step(digit * (STEPS / DIGITS));
//  delay(1000);
}

/*
void rotate(int digits, boolean cw)
 {
 rotate(0, digits, cw);
 }
 
 void rotate(int spins, int digits, boolean cw)
 {
 digitalWrite(DIR_PIN, cw ? HIGH : LOW);
 
 // spin some amount all the way?
 for (int i = 0; i < spins; i++)
 {
 step(STEPS);
// delay(1000);
 }
 
 
 step(digits * (STEPS / DIGITS));
// delay(1000);
 }
 */

// step to location
// TODO: return encoder feedback
void step(int steps)
{
  long newPosition;
  for (int i = 0; i < steps * MICROSTEPS; i++)
  {
    digitalWrite(STEP_PIN, LOW);
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(500);
  }

  // get encoder position
  newPosition = myEnc.read();
  if (newPosition != oldPosition)
  {
    //      oldPosition = newPosition;
    oldPosition = mod(newPosition, ENCODER_STEPS);//XXX
    Serial.print("newpos=");
    Serial.print(newPosition);
    Serial.print(" actual=");
    Serial.print((int)(oldPosition / DIGITS));
    Serial.print(" pos=");
    Serial.println(oldPosition);
  }
}

