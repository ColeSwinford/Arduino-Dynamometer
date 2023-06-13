int hallSensor =  2;
float rev = 0 ;
unsigned int rpm ;
unsigned long prevTime = 0 ;
unsigned long int time;

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
  rev = 0;

  serialDisplay();

  attachInterrupt(digitalPinToInterrupt(hallSensor), interruptRevCounter, FALLING);
}

// rpm,time (seconds)
void serialDisplay()
{
  Serial.print(rpm);
  Serial.print(",");
  Serial.println(micros()/1000000);
}