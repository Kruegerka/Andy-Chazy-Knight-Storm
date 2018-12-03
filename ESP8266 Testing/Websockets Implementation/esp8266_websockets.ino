#include <SoftwareSerial.h>
SoftwareSerial ESP8266(2,3);

boolean FAIL_8266 = false;

void setup() {
  
  Serial.begin(9600); 
  ESP8266.begin(115200);  // change this value to your wifi module (esp8266) baud rate
  
  do{
  ESP8266.println("AT");
    delay(1000);
    if(ESP8266.find((char*)"OK"))
    {
      Serial.println("Module Connected: Running Setup");
      delay(1000);
      clearESP8266SerialBuffer();

      //configure ESP as access point
      sendESP8266Cmdln("AT+CWMODE=2",1000);

      //Get and display my IP
      sendESP8266Cmdln("AT+CIFSR", 1000);  

      //Set multi connections
      sendESP8266Cmdln("AT+CIPMUX=1", 1000);
      
      //Setup web server on port 80
      sendESP8266Cmdln("AT+CIPSERVER=1,80",1000);

      Serial.println("Server setup finish");

      FAIL_8266 = false;

    }
    else{
      Serial.println("Setup Failed: No response");
      delay(500);
      FAIL_8266 = true;
    }
  }while(FAIL_8266);

  ESP8266.setTimeout(100); 

}

void loop() {
  if(ESP8266.available()){
    String msg = ESP8266.readString();
    Serial.println("MSG: "+msg);
  }
  if (Serial.available()){  
        String s = Serial.readString();
        sendStringResponse(connectionId , s + "\r\n"); 
    }
}


void sendESP8266Cmdln(String cmd, int waitTime)
{
  ESP8266.println(cmd);
  delay(waitTime);
  clearESP8266SerialBuffer();
}

void clearESP8266SerialBuffer()
{
  Serial.println("=  Clearing ESP_8266 Buffer  =");
  while (ESP8266.available() > 0) {
    char a = ESP8266.read();
  }
}

void sendStringResponse(int connectionId, String content)
{     
     sendCIPData(connectionId,content);
}