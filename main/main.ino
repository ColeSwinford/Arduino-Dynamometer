#define PI 3.1415926535897932384626433832795

//vars
int hallSensor =  2;
float rev = 0;
unsigned int rpm;
unsigned long prevTime = 0;
unsigned long int time;
float angularVel0 = 0;
float angularVel1;
float angularAccel;
float torque;
float curveHP;
float hp;

////// testing for momentInertia constant
momentInertia;
float torque = 1234;
////// testing for momentInertia constant

//consts
// momentInertia = 1234;

void interruptRevCounter()  //interrupt service routine
{
  rev ++;
}

void setup()
{
  Serial.begin(115200); //Baud Rate
  attachInterrupt(digitalPinToInterrupt(hallSensor), interruptRevCounter, FALLING); //attach interrupt
}

// torque and hp calculations based on data from engine curves
float getTorque(int rpm)
{
    // torque = y = 16.6 + 4.25E-03x + -1.43E-06x^2
    float torque = 16.6 + (0.00425)*rpm + (-0.00000143)*(pow(rpm, 2));
    return torque;
}
float getHP(int rpm)
{
    // hp = y = -5.99 + 9.7E-03x + -1.52E-06x^2
    float hp = -5.99 + (0.0097)*rpm + (-0.00000152)*(pow(rpm, 2));
    return hp;
}

void loop()
{
  delay(1000);
  detachInterrupt(digitalPinToInterrupt(hallSensor)); //detach interrupt
  time = micros() - prevTime;  //get time elapsed since last check
  rpm = ( rev / time ) * 60000000;  //calculate rpm
  angularVel1 = (rpm / 60) * (2 * PI);  //calculate angularVel1 in rad/s
  angularAccel = (angularVel1 - angularVel0)/(time - prevTime);  //calculate angularAccel
  angularVel0 = angularVel1;  //save angularVel0
  prevTime = micros();  //save current time
  // torque = angularAccel * momentInertia;  //calculate torque

  ////// testing for momentInertia constant
  torque = getTorque(rpm);
  curveHP = getHP(rpm);
  momentInertia = torque / angularAccel;
  ////// testing for momentInertia constant
  
  hp = (rpm * torque) / 63025  //calculate hp
  rev = 0;

  serialDisplay();

  attachInterrupt(digitalPinToInterrupt(hallSensor), interruptRevCounter, FALLING);
}

// rpm,time (seconds)
void serialDisplay()
{
  Serial.print(rpm);
  Serial.print(",");
  Serial.print(torque);
  Serial.print(",");
  Serial.print(curveHP);
  Serial.print(",");
  Serial.print(hp);
  Serial.print(",");

  ////// testing for momentInertia constant
  Serial.print(momentInertia);
  Serial.print(",");
  ////// testing for momentInertia constant
  
  Serial.println(micros()/1000000);
}
