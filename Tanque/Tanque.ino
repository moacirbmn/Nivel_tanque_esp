int nivel = 0;
const int pingPin = 6; // Ultrasom
int led1 = 13; //vermelho
int led2 = 12; //Verde
int led3 = 11; //Azul
long Alt;

void setup() {
  Serial.begin(9600);  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);     
  pinMode(led3, OUTPUT);     

}

// the loop routine runs over and over again forever:
void loop() {

  //estado do led --------------------------------------------------
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);

  if (nivel == 1)  
    digitalWrite(led1, LOW);
  else
    digitalWrite(led1, HIGH);

  if (nivel == 2)  
    digitalWrite(led2, LOW);
  else
    digitalWrite(led2, HIGH);

  if (nivel == 3)  
    digitalWrite(led3, LOW);
  else
    digitalWrite(led3, HIGH);

  //-----------------------------------------------------------------
  //Sensor ultrasom--------------------------------------------------
  Ultrasom();
//  Serial.print(Alt);
//  Serial.print("cm");
//  Serial.println();
//  delay(100); 

  //calculo volume-------------------------------------------------------------------------
  float Tcm3_ret = 0;
  int Larg_ret = 10; //em cm
  int Comp_ret = 10; //em cm
  float Litros = 0;
  Tcm3_ret = Larg_ret * Comp_ret * Alt;
  Litros = Tcm3_ret/1000;
  Serial.print(Tcm3_ret);
  Serial.print("cm3");
  Serial.print(" - ");
  Serial.print(Litros);
  Serial.println("L");
  delay(1000);




  //-----------------------------------------------------------------



  //WIFI -------------------------------------------------------------------------









}

void Ultrasom(){
  long duration;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  Alt = microsecondsToCentimeters(duration);

}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}








