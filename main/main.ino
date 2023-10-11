//vars
int hallSensor =  2;
float rev = 0 ;
unsigned int rpm ;
unsigned long prevTime = 0 ;
unsigned long int time;
float torque ;
float hp ;

//constants
alpha = 1234;
momentInteria = 1234;

void interruptRevCounter()  //interrupt service routine
{
  rev ++;
}

void setup()
{
  Serial.begin(115200); //Baud Rate
  attachInterrupt(digitalPinToInterrupt(hallSensor), interruptRevCounter, FALLING); //attach interrupt
}

void loop()
{
  delay(1000);
  detachInterrupt(digitalPinToInterrupt(hallSensor)); //detach interrupt
  time = micros() - prevTime;  //get time elapsed since last check
  rpm = ( rev / time ) * 60000000;  //calculate rpm
  prevTime = micros();  //save current time
  torque = alpha * momentInteria;  //calculate torque
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
  Serial.print(hp);
  Serial.print(",");
  Serial.println(micros()/1000000);
}
