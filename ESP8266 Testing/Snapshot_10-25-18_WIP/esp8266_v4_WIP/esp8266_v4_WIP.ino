#include <SoftwareSerial.h>
#include <WiFi.h>

/*
 * These variables were written by jeffparedes.
 */
SoftwareSerial wifiSerial(2, 3);
static bool testSerial;
bool DEBUG = true;   //show more logs
int responseTime = 10; //communication timeout

/*
 * These variables were added by boyerct.
 */
const int SW1 = 8;
const int SW2 = 9;
const int SW3 = 10;
const int LED1 = 13;
const int SECOND_DELAY = 15625;
const int SEC1 = 1;
const int SEC5 = 5;
const int SEC10 = 10;
static byte sw_val;
static byte prev_sw_val;
static bool sw1_lvl;
static bool sw2_lvl;
static bool sw3_lvl;
volatile static bool ledon;
volatile static int secondsCnt;
volatile static int seconds;

//WiFiServer server(80);

//Most of this code was written by jeffparedes. 
void setup() {
  //noInterrupts();
  /*
   * Boyerct added the pin descriptions and variable declarations.
   */
  prev_sw_val = 0;
  sw_val = 0;
  secondsCnt = 0;
  seconds = 0;
  //ledon = false;
  testSerial = false;
  pinMode(SW1,INPUT);
  pinMode(SW2,INPUT);
  pinMode(SW3,INPUT);
  pinMode(LED1,OUTPUT);
  //digitalWrite(LED1,LOW);

  /**
   * The Timer1 setup code for blinking LED sequences by boyerct.
   */
  //Setup timer 2 and start it
  TCCR1A = 0; //Clear the A control register
  TCCR1B = 0; //Clear the B control register
  TCNT1 = 0;  //Initialize counter start value to 0

  OCR1A = SECOND_DELAY;           //This is the number of cycles with 1024 prescaler for 1 second delay
  TCCR1B |= (1 << WGM12);  //Set to "Clear Timer on Compare" mode for autonomous restart on interrupt.
  TIMSK1 |= (1 << OCIE1A); //Set interrupt to trigger on a comparison match.
  //TCCR1B |= (1 << CS12) | (1 << CS10); // set prescaler to 1024 and start the timer; comment out for real run

  //interrupts();
  
  /**
   * jeffparedes' code continues from here, except the "AT" command with no parameters.
   */
  if(!testSerial){
    Serial.begin(115200);
    while (!Serial) {}; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Started");

  // set the data rate for the SoftwareSerial port to ESP8266
  if(!testSerial){
    wifiSerial.begin(115200);
  }
  sendToWifi("AT+CWMODE=2",responseTime,DEBUG); // configure as access point
  sendToWifi("AT+CIFSR",responseTime,DEBUG); // get ip address
  sendToWifi("AT+CIPMUX=1",responseTime,DEBUG); // configure for multiple connections
  sendToWifi("AT+CIPSERVER=1,80",responseTime,DEBUG); // turn on server on port 80
 
  sendToUno("Wifi connection is running!",responseTime,DEBUG);

}

//Most of this code was written by jeffparedes; we only added more "find" statements for communication.
void loop() {
  if(Serial.available()){
    String message = readSerialMessage();
    if(find(message,"debugEsp8266:")){
      String result = sendToWifi(message.substring(13,message.length()),responseTime,DEBUG);
      if(find(result,"OK")){
        sendData("\nOK");
      } else {
        sendData("\nEr");
      }
    }
  }
  if(wifiSerial.available()){

    String message = readWifiSerialMessage();
    
    if(find(message,"esp8266:")){
       String result = sendToWifi(message.substring(8,message.length()),responseTime,DEBUG);
      if(find(result,"OK"))
        sendData("\n"+result);
      else
        sendData("\nErrRead");               //At command ERROR CODE for Failed Executing statement
    } else
    if(find(message,"HELLO")){  //receives HELLO from wifi
        sendData("\\nHI!");    //arduino says HI
    } else if(find(message,"/on")){
        TCCR1B = 0;
        //secondsCnt = 0; //Reset seconds count.
        digitalWrite(LED1,HIGH);
        sendData("Turned it on.");
    } else if(find(message,"/off")){
        TCCR1B = 0;
        //secondsCnt = 0; //Reset seconds count.
        digitalWrite(LED1,LOW);
        sendData("Turned it off.");
    } else if(find(message,"/1sec")){
      startTimer(SEC1);
      sendData("Blink mode set to 1 second.");
    } else if(find(message,"/5sec")){
      startTimer(SEC5);
      sendData("Blink mode set to 5 seconds.");
    } /*else if(find(message,"/10sec")){
      startTimer(SEC10);
      sendData("Blink mode set to 10 seconds.");
    } */else if(find(message,"/refresh")){
      sw_val = (digitalRead(SW3) << 2) + (digitalRead(SW2) << 1) + digitalRead(SW1);
      sendUpdate("Switch: "+String(sw_val));
    } /*else if(find(message,"/refreshed")){
      //Serial.println("Refresh confirm");
      sendData("OK");
    }*/
    else{
      sendData("\nNoMatch.");                 //Command ERROR CODE for UNABLE TO READ
    }
  }
}

/*
* Name: sendData
* Author: jeffparedes
* Description: Function used to send string to tcp client using cipsend
* Params: 
* Returns: void
*/
void sendData(String str){
  String len="";
  len+=str.length();
  sendToWifi("AT+CIPSEND=0,"+len,responseTime,DEBUG);
  delay(100);
  sendToWifi(str,responseTime,DEBUG);
  delay(100);
  sendToWifi("AT+CIPCLOSE=5",responseTime,DEBUG);
}

/*
* Name: sendUpdate
* Author: boyerct
* Description: Function used to send updated switch value to tcp client
* Params: String message to send
* Returns: void
*/
void sendUpdate(String str){
  String len="";
  len+=str.length();
  //sendToWifi("AT+CIPSTART=0,TCP,192.168.4.2,80",responseTime,DEBUG);
  //delay(100);
  sendToWifi("AT+CIPSEND=0,"+len,responseTime,DEBUG);
  delay(100);
  sendToWifi(str,responseTime,DEBUG);
  delay(100);
  sendToWifi("AT+CIPCLOSE=5",responseTime,DEBUG);
}

/*
* Name: find
* Author: jeffparedes
* Description: Function used to match two string
* Params: 
* Returns: true if match else false
*/
boolean find(String string, String value){
  if(string.indexOf(value)>=0)
    return true;
  return false;
}

/*
* Name: readSerialMessage
* Author: jeffparedes
* Description: Function used to read data from Arduino Serial.
* Params: 
* Returns: The response from the Arduino (if there is a reponse)
*/
String  readSerialMessage(){
  char value[100]; 
  int index_count =0;
  while(Serial.available()>0){
    value[index_count]=Serial.read();
    index_count++;
    value[index_count] = '\0'; // Null terminate the string
  }
  String str(value);
  str.trim();
  return str;
}

/*
* Name: readWifiSerialMessage
* Author: jeffparedes
* Description: Function used to read data from ESP8266 Serial.
* Params: 
* Returns: The response from the esp8266 (if there is a reponse)
*/
String  readWifiSerialMessage(){
  char value[100]; 
  int index_count =0;
  while(wifiSerial.available()>0){
    value[index_count]=wifiSerial.read();
    index_count++;
    value[index_count] = '\0'; // Null terminate the string
  }
  String str(value);
  str.trim();
  return str;
}

/*
* Name: sendToWifi
* Author: jeffparedes
* Description: Function used to send data to ESP8266.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a response)
*/
String sendToWifi(String command, const int timeout, boolean debug){
  String response = "";
  wifiSerial.println(command); // send the read character to the esp8266
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(wifiSerial.available())
    {
    // The esp has data so display its output to the serial window 
    char c = wifiSerial.read(); // read the next character.
    response+=c;
    }  
  }
  if(debug)
  {
    Serial.println(response);
  }
  return response;
}

/*
* Name: sendToUNO
* Author: jeffparedes
* Description: Function used to send data to UNO.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the UNO (if there is a response)
*/
String sendToUno(String command, const int timeout, boolean debug){
  String response = "";
  Serial.println(command); // send the read character to the esp8266
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(Serial.available())
    {
      // The esp has data so display its output to the serial window 
      char c = Serial.read(); // read the next character.
      response+=c;
    }  
  }
  if(debug)
  {
    Serial.println(response);
  }
  return response;
}

/**
 * Name: startTimer
 * Author: boyerct
 * Description: This function will stop and reset an existing timer configuration, and then start the timer with the new delay period.
 * Params: delayTime - the delay period for the timer
 * Returns: void
 */
void startTimer(int delayTime){
  //noInterrupts();
  TCCR1B = 0;
  TCCR1B |= (1 << WGM12); //Stop the timer.
  TCNT1 = 0; //Reset the timer count.
  //interrupts();
  secondsCnt = 0; //Reset seconds count.
  ledon = false;
  digitalWrite(LED1,LOW);
  switch(delayTime){
    case SEC1:
      seconds = 1;
      TCCR1B |= (1 << CS12) | (1 << CS10); //Starts timer with 1024 prescaler
      break;
    case SEC5:
      seconds = 5;
      TCCR1B |= (1 << CS12) | (1 << CS10); //Starts timer with 1024 prescaler
      break;
    case SEC10:
      seconds = 10;
      TCCR1B |= (1 << CS12) | (1 << CS10); //Starts timer with 1024 prescaler
      break;
    default:
      Serial.println("Invalid delay time");
      break;
  }
}

ISR(TIMER1_COMPA_vect){
  //check if count is equal to number of seconds; if so, then toggle the LED
  secondsCnt++;
  if(secondsCnt == seconds){
    ledon = !ledon;
    secondsCnt = 0;
    if(ledon){
      digitalWrite(LED1, HIGH);
    } else {
      digitalWrite(LED1, LOW);
    }
  }
}
