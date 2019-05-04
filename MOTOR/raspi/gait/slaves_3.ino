#include <Wire.h>

#define SLAVE_ADDRESS 0x04
//#define SLAVE_ADDRESS 0x05
//#define SLAVE_ADDRESS 0x06
int number[10];
int state = 0;
static int i;

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600); // start serial for output
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Ready!");
  pinMode(LED_BUILTIN, OUTPUT);
  i=0;
}

void loop() {
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
}

// callback for received data
void receiveData(int byteCount){
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println(byteCount);
  while(Wire.available()) {
//      Serial.print(i);
//      Serial.print("=>");
      number[i] = Wire.read();
//      Serial.print("data received: ");
      Serial.println(number[i]);  
      i++;
      if (i==byteCount) {
        i=0;
      }
  }
}

// callback for sending data
void sendData(){
  Wire.write(number[1]);
}


