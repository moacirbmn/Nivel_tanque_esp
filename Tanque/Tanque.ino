#include <ESP8266WiFi.h>
WiFiServer server(80); //Initialize the server on Port 80

int nivel = 0;
const int pingPin = 15; // Ultrasom
const int led1 = 13; //vermelho
const int led2 = 5; //Verde
const int led3 = 4; //Azul
long Alt;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint
  WiFi.softAP("Tanque_IoT", "12345678"); // Provide the (SSID, password); .
  server.begin(); // Start the HTTP Server

  // Looking under the hood
  Serial.begin(115200); //Start communication between the ESP8266-12E and the monitor window
  IPAddress HTTPS_ServerIP = WiFi.softAPIP(); // Obtain the IP of the Server
  Serial.print("Server IP is: "); // Print the IP to the monitor window
  Serial.println(HTTPS_ServerIP);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);

}

// the loop routine runs over and over again forever:
void loop() {

  //WIFI -------------------------------------------------------------------------
  WIFI();

  //estado do led --------------------------------------------------


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
  // delay(100);

  //calculo volume-------------------------------------------------------------------------
  float Tcm3_ret = 0;
  int Larg_ret = 10; //em cm
  int Comp_ret = 10; //em cm
  int Altura_max_tanque = 60;
  float Litros = 0;
  Tcm3_ret = Larg_ret * Comp_ret * (Altura_max_tanque - Alt);
  if (Tcm3_ret < 0)
    Tcm3_ret = 0;
  Litros = Tcm3_ret / 1000;
  /* Serial.print(Tcm3_ret);
    Serial.print("cm3");
    Serial.print(" - ");
    Serial.print(Litros);
    Serial.println("L");
    delay(300);*/

  if (Litros <= 4)
    nivel = 3;
  if (Litros > 4)
    nivel = 2;
  if (Litros == 0)
    nivel = 1;



  //-----------------------------------------------------------------

}

void Ultrasom() {
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


void WIFI() {
  while ( WiFiClient client = server.available()) {
    if (!client) {
      return;
    }
    //Looking under the hood
    Serial.println("Somebody has connected :)");

    //Read what the browser has sent into a String class and print the request to the monitor
    String request = client.readString();
    //Looking under the hood
    Serial.println(request);
    // Handle the Request

    if (request.indexOf("/OFF") != -1) {
      digitalWrite(led3, HIGH);
    }
    else if (request.indexOf("/ON") != -1) {
      digitalWrite(led3, LOW);
    }
    String s = "HTTP/1.1 200 OK\r\n";
    s += "Content-Type: text/html\r\n\r\n";
    s += "<!DOCTYPE HTML>\r\n<html>\r\n";
    s += "<br><input type=\"button\" name=\"b1\" value=\"Ligar LED ON\" onclick=\"location.href='/ON\">";
    s += "<br><br><br>";
    s += "<br><input type=\"button\" name=\"b1\" value=\"Desliga LED OFF\" onclick=\"location.href='/OFF\">";
    s += "</html>\n";

    client.flush(); //clear previous info in the stream
    client.print(s); // Send the response to the client
    delay(1);
    Serial.println("Client disonnected"); //Looking under the hood

  }
}

