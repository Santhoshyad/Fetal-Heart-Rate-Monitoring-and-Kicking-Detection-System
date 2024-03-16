#include <SoftwareSerial.h>
SoftwareSerial SIM7670Serial(5,4); // RX, TX
// Constants
const int baseHeartRate = 120;           // Base heart rate (beats per minute)
const int variationRange = 15;          // Range of random variations (+/-)
const int arraySize = 5;  // Size of the arrays
int array1[arraySize];  // First array of heartbeat values
int array2[arraySize] = {98,92,95,98,92};  // Second array of heartbeat values

void setup() {
  Serial.begin(9600);                   // Initialize serial communication
  randomSeed(analogRead(A0));           // Seed the random number generator
  Serial.begin(9600);
  SIM7670Serial.begin(115200);
  sendATCommand("AT", "OK", 1000); // check communication
  sendATCommand("AT+CMGF=1", "OK", 1000); // set SMS format to text
}

void loop() {
  int heartRate = baseHeartRate + random(-variationRange,variationRange );
  Serial.println(heartRate);
 

    for (int i = 0; i < arraySize; i++) {
    array1[i] = heartRate;
  // Delay between sensor readings
}

  // Calculate the average of the first array
  float average1 = calculateAverage(array1, arraySize);
  // Calculate the average of the second array
  float average2 = calculateAverage(array2, arraySize);
  //Serial.println(average1,average2);
  int emergency=detection1 (average1 ,average2);
  if (emergency==0||emergency==1){
    delay1000000();
  }
  delay(100); 
  
  }
  
  int detection1 (float averag1 ,float averag2)  {                      // Delay between heart rate updates
  if (averag1 < averag2){
     sendSMS("+919449093779", "FETAL HYPOXIA DETECTED!");
     return 1;
  }

  else if (averag1 < 90){
    sendSMS("+919449093779", "LOW HEART FETAL RATE !");
    return 0;
  }

  }

void sendSMS(String number, String message) {
  String cmd = "AT+CMGS=\"" + number + "\"\r\n";
  SIM7670Serial.print(cmd);
  delay(10);
  SIM7670Serial.println(message);
  delay(10);
  SIM7670Serial.write(0x1A); // send ctrl-z
  delay(10);
}

void sendATCommand(String cmd, String expectedResponse, int timeout) {
  SIM7670Serial.println(cmd);
  String response = "";
  long timeStart = millis();
  while (millis() - timeStart < timeout) {
    while (SIM7670Serial.available() > 0) {
      char c = SIM7670Serial.read();
      response += c;
    }
    if (response.indexOf(expectedResponse) != -1) {
      break;
    }
  }
  Serial.println(response);
}

void delay1000000(){
  delay(100);
}

float calculateAverage(int values[], int size) {
  int sum = 0;
  for (int i = 0; i < size; i++) {
    sum += values[i];
  }
  return static_cast<float>(sum) / size;
}