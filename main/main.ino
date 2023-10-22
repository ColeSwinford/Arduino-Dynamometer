#define PI 3.1415926535897932384626433832795

//vars
int hallSensor =  2;
double rev = 0;
unsigned int rpm;
unsigned long prevTime = 0;
unsigned long int time;
double angularVel0 = 0;
double angularVel1;
double angularAccel;
double torque;
double curveHP;
double hp;

////// testing for momentInertia constant
double momentInertia;
////// testing for momentInertia constant

//Kohler CH440 Torque and HP curve relative to RPM. {RPM, Torque, HP}
double RPM_v_Torque_v_HP[][3]{
  {0, 0, 0},
  {2400, 18.5, 8.5},
  {2600, 18.1, 9},
  {2800, 17.4, 9.3},
  {3000, 16.6, 9.5},
  {3200, 15.4, 9.4},
  {3400, 14.5, 9.4},
  {3600, 13.5, 9.3}
};

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
double getTorque(int rpm)
{
  // torque = y = 16.6 + 4.25E-03x + -1.43E-06x^2
  // double torque = 16.6 + (0.00425)*rpm + (-0.00000143)*(pow(rpm, 2));
  //y=
  double y = rpm;
  double y0;
  int xLow;
  int xUp;
  float yLow;
  float yUp;
  if(rpm <= 2400)
  {
    xLow = RPM_v_Torque_v_HP[0][0];
    yLow = RPM_v_Torque_v_HP[0][1];
    xUp = RPM_v_Torque_v_HP[1][0];
    yUp = RPM_v_Torque_v_HP[1][1];
  }
  if(rpm <= 2600 && rpm > 2400)
  {
    xLow = RPM_v_Torque_v_HP[1][0];
    yLow = RPM_v_Torque_v_HP[1][1];
    xUp = RPM_v_Torque_v_HP[2][0];
    yUp = RPM_v_Torque_v_HP[2][1];
  }
  if(rpm <= 2800 && rpm > 2600)
  {
    xLow = RPM_v_Torque_v_HP[2][0];
    yLow = RPM_v_Torque_v_HP[2][1];
    xUp = RPM_v_Torque_v_HP[3][0];
    yUp = RPM_v_Torque_v_HP[3][1];
  }
  if(rpm <= 3000 && rpm > 2800)
  {
    xLow = RPM_v_Torque_v_HP[3][0];
    yLow = RPM_v_Torque_v_HP[3][1];
    xUp = RPM_v_Torque_v_HP[4][0];
    yUp = RPM_v_Torque_v_HP[4][1];
  }
  if(rpm <= 3200 && rpm > 3000)
  {
    xLow = RPM_v_Torque_v_HP[4][0];
    yLow = RPM_v_Torque_v_HP[4][1];
    xUp = RPM_v_Torque_v_HP[5][0];
    yUp = RPM_v_Torque_v_HP[5][1];
  }
  if(rpm <= 3400 && rpm > 3200)
  {
    xLow = RPM_v_Torque_v_HP[5][0];
    yLow = RPM_v_Torque_v_HP[5][1];
    xUp = RPM_v_Torque_v_HP[6][0];
    yUp = RPM_v_Torque_v_HP[6][1];
  }
  if(rpm <= 3600 && rpm > 3400)
  {
    xLow = RPM_v_Torque_v_HP[6][0];
    yLow = RPM_v_Torque_v_HP[6][1];
    xUp = RPM_v_Torque_v_HP[7][0];
    yUp = RPM_v_Torque_v_HP[7][1];
  }
  if(rpm > 3600)
  {
    torque = 12345678910;
  }

  // y = yLow + slope(x-xLow), where slope = (yUp-YLow)/(xUp-xLow)
  double torque = yLow + (((yUp-yLow)/(xUp-xLow))*(rpm-xLow));
  return torque;
};
double getHP(int rpm)
{
  // hp = y = -5.99 + 9.7E-03x + -1.52E-06x^2
  double hp = -5.99 + (0.0097)*rpm + (-0.00000152)*(pow(rpm, 2));
  return hp;
}
double getMomentInertia(double torque, double angularAccel)
{
  momentInertia = torque / angularAccel;
  return momentInertia;
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
  
  ////// testing for momentInertia constant
  momentInertia = getMomentInertia(torque, angularAccel);
  
  hp = (rpm * torque) / 63025;  //calculate hp
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
