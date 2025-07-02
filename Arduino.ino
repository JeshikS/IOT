#include <LiquidCrystal.h>
#include <Servo.h>

Servo motor1;
Servo motor2;
int pos = 0;
int pir = A4;
LiquidCrystal lcd(2,4,7,8,12,13);
const int trigpin = 10;
const int echopin = 9;
int gas_sensor = A3;
const int temp_sensor = A5;
const int photo_resistor = A2;
String ssid     = "PGH";  // SSID to connect to WIFI
String password = ""; // 
String host     = "https://api.thingspeak.com";
const int httpPort   = 80;

String uri = "/update?api_key=5LE14CU529N142U2";

int bulb1 = 5;
int bulb2 = 6;
int fan1 = 11;
int fan2 = 3;
byte automatic[] = {
  B00000,
  B00000,
  B01110,
  B10001,
  B10101,
  B10001,
  B01110,
  B00000
};

int button1 = 3;
int button2 = 5;
int button3 = 6;
int button4 = 11;
double distancecm;
double duration;
double distanceinch;
float temp_reading;
float temp;
float temp_final;
int i;

int setupESP8266(void) {
  // Start our ESP8266 Serial Communication
  Serial.begin(115200);   // Serial connection over USB to computer
  Serial.println("AT");   // Serial connection on Tx / Rx port to ESP8266
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 1;
    
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 2;
  
  // Open TCP connection to the host:
  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
  delay(50);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 3;
  
  return 0;
}

void setup()
{
  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);
    motor1.attach(A0);
    motor2.attach(A1);
    motor1.write(pos);
    motor2.write(pos);
  lcd.createChar(0,automatic);
  lcd.begin(16,2);
  lcd.setCursor(4,0);
  lcd.print("Complete");
  lcd.setCursor(3,1);
  lcd.print("Automation ");
  lcd.write(byte(0));
  delay(1000);
  lcd.clear();
  Serial.begin(9600);
  
  setupESP8266();
  
}
void loop()
{
  
  digitalWrite(trigpin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);
  duration = pulseIn(echopin,HIGH);
   distancecm = (duration*0.034)/2;
  distanceinch = (duration*0.0133)/2;
 // Serial.print("Distance in cm =");
 // Serial.println(distancecm);
  
  int analog_sensor = analogRead(gas_sensor);
  temp_reading = analogRead(temp_sensor);
  temp = (temp_reading/1023)*5000;
  temp_final = (temp - 500)/10;
 // Serial.print("Temperature : ");
 // Serial.println(temp_final);
 // Serial.print("Sensor : ");
  Serial.println(analog_sensor);
  

  int photo = analogRead(photo_resistor);
 // Serial.println(photo);
  
 // analogWrite(A4,255);
 // analogWrite(fan2,64);
  
  
  if(photo <= 425 & distancecm <= 305 & analog_sensor < 180)
  {
    lcd.clear();
    analogWrite(bulb1,255);
    analogWrite(bulb2,255);
   lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2");
  lcd.setCursor(0,1);
  lcd.print("100");
  lcd.setCursor(4,1);
  lcd.print("100");
    delay(400);
  }
  else if(photo > 425 & photo <=517 & distancecm <= 305 & analog_sensor < 180)
  {
    lcd.clear();
    analogWrite(bulb1,150);
    analogWrite(bulb2,150);
    lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2");
  lcd.setCursor(0,1);
  lcd.print("75");
  lcd.setCursor(4,1);
  lcd.print("75");
    delay(400);
  }
   else if(photo > 517 & photo <=574 & distancecm <= 305 & analog_sensor < 180)
  {
    lcd.clear();
    analogWrite(bulb1,110);
    analogWrite(bulb2,110);
  lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2");  
  lcd.setCursor(0,1);
  lcd.print("50");
  lcd.setCursor(4,1);
  lcd.print("50");
    delay(400);
  }
   else if(photo > 574 & photo <=630 & distancecm <= 305 & analog_sensor < 180)
  {
    lcd.clear();
    analogWrite(bulb1,64);
    analogWrite(bulb2,64);
  lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2");  
  lcd.setCursor(0,1);
  lcd.print("25");
  lcd.setCursor(4,1);
  lcd.print("25");
    delay(400);
  }
   else if(photo > 630 & distancecm <= 305 & analog_sensor < 180)
  {
    lcd.clear();
    analogWrite(bulb1,0);
    analogWrite(bulb2,0);
  lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2");  
  lcd.setCursor(0,1);
  lcd.print("OFF");
  lcd.setCursor(4,1);
  lcd.print("OFF");
    delay(400);
  }
  else if(distancecm > 305  & analog_sensor < 180)
  {
    lcd.clear();
    analogWrite(bulb1,0);
    analogWrite(bulb2,0);
     analogWrite(fan1,0);
    analogWrite(fan2,0);
  lcd.setCursor(3,0);
  lcd.print("System off");
  
  lcd.setCursor(1,1);
  lcd.print("Nobody in Room");
 // lcd.print("OFF ");
    delay(400);
  }
  
  else if(analog_sensor >= 180)
  {
    lcd.clear();
     analogWrite(bulb1,255);
    analogWrite(bulb2,255);
     analogWrite(fan1,255);
    analogWrite(fan2,255);
    motoropen();
  
     lcd.setCursor(3,0);
  lcd.print("DANGER");
  
  lcd.setCursor(0,1);
  lcd.print("EXIT FROM ROOM");
 // lcd.print("OFF ");
    delay(400);
  }
  if(temp_final >= 20 & temp_final < 30 & distancecm <= 305 & analog_sensor < 180)
  {
    lcd.clear();
    analogWrite(fan1,64);
    analogWrite(fan2,64);
  lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2"); 
  lcd.setCursor(8,1);
  lcd.print("25");
  lcd.setCursor(12,1);
  lcd.print("25");
    delay(400);
  }
  else if(temp_final >= 30 & temp_final < 45  & distancecm <= 305 & analog_sensor < 180)
  {
     lcd.clear();
    analogWrite(fan1,128);
    analogWrite(fan2,128);
  lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2"); 
  lcd.setCursor(8,1);
  lcd.print("50");
  lcd.setCursor(12,1);
  lcd.print("50");
    delay(400);
  }
  else if(temp_final >= 45 & temp_final < 60  & distancecm <= 305 & analog_sensor < 180)
  {
     lcd.clear();
    analogWrite(fan1,150);
    analogWrite(fan2,150);
  lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2"); 
  lcd.setCursor(8,1);
  lcd.print("75");
  lcd.setCursor(12,1);
  lcd.print("75");
    delay(400);
  }
   else if( temp_final < 0  & distancecm <= 305 & analog_sensor < 180)
  {
     lcd.clear();
    analogWrite(fan1,0);
    analogWrite(fan2,0);
  lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2"); 
  lcd.setCursor(8,1);
  lcd.print("OFF");
  lcd.setCursor(12,1);
  lcd.print("OFF");
    delay(400);
  }
   else if(temp_final >= 60 & distancecm <= 305 & analog_sensor < 180)
  {
     lcd.clear();
    analogWrite(fan1,255);
    analogWrite(fan2,255);
  lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2"); 
  lcd.setCursor(8,1);
  lcd.print("100");
  lcd.setCursor(12,1);
  lcd.print("100");
    delay(400);
  }
 
 
 /* if(analog_sensor > 180)
  {
     lcd.clear();
    analogWrite(fan1,255);
    analogWrite(fan2,255);
    analogWrite(bulb1,255);
    analogWrite(bulb2,255);
    lcd.setCursor(0,0);
    lcd.print("Danger....");
    motoropen();
   // delay(400);
  }*/
   int pirvalue = digitalRead(pir);
  if(pirvalue == HIGH)
  {
     lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Somebody Enter");
    lcd.setCursor(0,1);
    lcd.print("Gates - OPENING");
    motoropen();
    delay(400);
  }
  else if(analog_sensor < 180)
  {
   motorclose();
  }
  
  /* if (!client.connect(host, httpPort)) {
   	Serial.println("Connection failed");
   } */
  	
   /*String postData = "api_key=" + String(writeAPIKey) + "&field1=" + String(distanceinch) +
     				  "&field2=" + String(analog_sensor) + "&field3=" + String(temp_final) +
     				   "&field4=" + String(photo) + "&field5=" + String(pirvalue);*/
   
   /*client.print(String("POST ") + "/update" + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n" +
               "Content-Type: application/x-www-form-urlencoded\r\n" +
               "Content-Length: " + postData.length() + "\r\n\r\n" +
               postData);*/
  
    // Construct our HTTP call

   String httpPacket= "GET "+ host + uri +"&field1=" + String(distanceinch) +
     				  "&field2=" + String(analog_sensor) + "&field3=" + String(temp_final) +
     				   "&field4=" + String(photo) + "&field5=" + String(pirvalue);
  //  + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n"

    int length = httpPacket.length();

    // Serial.println("TempSensorvalue: "+ String(tempSensor1)+"\n\n");
    // Serial.println("LightSensorvalue: "+ String(lightSensor2)+"\n\n");

    Serial.print("AT+CIPSEND=");
    Serial.println(length);
    delay(10); // Wait a little for the ESP to respond if (!Serial.find(">")) return -1;

    // Send our http request
    Serial.print(httpPacket);
    delay(10); // Wait a little for the ESP to respond
    if (!Serial.find("SEND OK\r\n")) return;
  
  }

void motoropen()
{
 for(;pos <= 90;pos+=10)
 {
  motor1.write(pos);
   motor2.write(pos);
   delay(80);
 }
}
void motorclose()
{
 for(;pos>=0;pos-=10)
 {
  motor1.write(pos);
   motor2.write(pos);
   delay(80);
 }
}
