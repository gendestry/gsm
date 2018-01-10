#include <SoftwareSerial.h>
#include <String.h>

SoftwareSerial gsm(10, 11);

String allowedNums[] = {
  "040404040"
};

int allowedNumsLength = sizeof(allowedNums) / sizeof(String);

void setup(){
  Serial.begin(9600);
  gsm.begin(4800);
  delay(5000); // give time to log on to network.
  
  gsm.print("AT+CLIP=1\r"); // turn on caller ID notification

  while(!gsm.available());
  gsm.readString();

  Serial.println("Initialisation fertig");
}

void loop() {
  if(gsm.available() > 0) {
    String cellnum = parseNumber(gsm.readString());
    Serial.println("Parsed: " + cellnum);
    
    if(cellnum != "ERR") {
      for(int i = 0; i < allowedNumsLength; i++) {
        if(cellnum == allowedNums[i]) {
          // your phone number is ok
          gsm.println("ATH"); // hangup the call
          
          while(!gsm.available());
          gsm.readString(); // clear buffer
        }
      }
    }
  } 
}

String parseNumber(String input) {
  if(input.charAt(17) == '"'){ // working for xxxyyyzzz / 9 numbers long numbers
    return input.substring(18,27);
  }
  else {
    return "ERR";
  }
}

