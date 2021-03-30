#include <SoftwareSerial.h>
#define RX 5// CHANGE ME
#define TX 6
String AP = "SURIYA";
String PASS = "shivvish2019"; // CHANGE ME
String API = "TG65B0L1AZVEDMQX";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";
int countTrueCommand;
int countTimeCommand;
boolean found = false;
int val ;
SoftwareSerial esp8266(RX, TX);
const int trigpin = 9;
const int echopin = 10;
const int pr = A0;
long duration;
int distance;
int ledpin = 11;
int btnpin = 2;
int k = 0;
int value;
int buzzerpin = 13;

void setup() {
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(ledpin, OUTPUT);
  pinMode(buzzerpin, OUTPUT);
  pinMode(btnpin, INPUT);
  pinMode(pr, INPUT);
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK");
}


void loop() {
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  duration = pulseIn(echopin, HIGH);
  distance = duration * 0.034 / 2;
  
  value = analogRead(pr);
  
  if ((distance != 10) && (value > 25)) {
    val = 1;
    digitalWrite(ledpin, HIGH);
    digitalWrite(buzzerpin, HIGH);
    delay(100);
    digitalWrite(ledpin, LOW);
    digitalWrite(buzzerpin, LOW);
  }
  else if ((distance != 10) || (value > 25)) {
    val = 1;
    digitalWrite(ledpin, HIGH);
    digitalWrite(buzzerpin, HIGH);
    delay(100);
    digitalWrite(ledpin, LOW);
    digitalWrite(buzzerpin, LOW);
  }
  else
  {
    val = 0;
    digitalWrite(ledpin, LOW);
    digitalWrite(buzzerpin, LOW);
  }
  
  String getData = "GET /update?api_key=" + API + "&" + field + "=" + String(val);
  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 4, ">");
  esp8266.println(getData); countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 5, "OK");
  Serial.print("VALUE: ");
  Serial.println(val);
  
  if (value>25)
   {
    Serial.print("Light is on.   ");
   }
  else 
  {
    Serial.print("light is off.  ");
  }
  Serial.print("distance: ");
  Serial.println(distance);

}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while (countTimeCommand < (maxTime * 1))
  {
    esp8266.println(command);//at+cipsend
    if (esp8266.find(readReplay)) //ok
    {
      found = true;
      break;
    }

    countTimeCommand++;
  }

  if (found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }

  if (found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }

  found = false;
}
