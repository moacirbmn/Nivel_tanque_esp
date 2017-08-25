// Programa: Web Server com modulo ESP8266
// Alteracoes e adaptacoes: FILIPEFLOP

#include <SoftwareSerial.h>

//RX pino 2, TX pino 3
SoftwareSerial esp8266(2, 3);

#define DEBUG true

void setup()
{
  Serial.begin(9600);
  esp8266.begin(19200);

  sendData("AT+RSTrn", 2000, DEBUG); // rst
  // Conecta a rede wireless
  sendData("AT+CWJAP="SSID","SENHA"rn", 2000, DEBUG);
  delay(3000);
  sendData("AT+CWMODE=1rn", 1000, DEBUG);
  // Mostra o endereco IP
  sendData("AT+CIFSRrn", 1000, DEBUG);
  // Configura para multiplas conexoes
  sendData("AT+CIPMUX=1rn", 1000, DEBUG);
  // Inicia o web server na porta 80
  sendData("AT+CIPSERVER=1,80rn", 1000, DEBUG);
}

void loop()
{
  // Verifica se o ESP8266 esta enviando dados
  if (esp8266.available())
  {
    if (esp8266.find("+IPD,"))
    {
      delay(300);
      int connectionId = esp8266.read() - 48;

      String webpage = "<head><meta http-equiv=""refresh"" content=""3"">";
      webpage += "</head><h1><u>ESP8266 - Web Server</u></h1><h2>Porta";
      webpage += "Digital 8: ";
      int a = digitalRead(8);
      webpage += a;
      webpage += "<h2>Porta Digital 9: ";
      int b = digitalRead(9);
      webpage += b;
      webpage += "</h2>";

      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "rn";

      sendData(cipSend, 1000, DEBUG);
      sendData(webpage, 1000, DEBUG);

      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; // append connection id
      closeCommand += "rn";

      sendData(closeCommand, 3000, DEBUG);
    }
  }
}

String sendData(String command, const int timeout, boolean debug)
{
  // Envio dos comandos AT para o modulo
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
