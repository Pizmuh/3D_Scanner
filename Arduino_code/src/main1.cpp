Koda za Arduino
#include <Arduino.h>
//SD
#include <SPI.h>
#include <SD.h>
File myFile;

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 7, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



//laser
#include <Wire.h>
#include "Adafruit_VL6180X.h"

Adafruit_VL6180X vl = Adafruit_VL6180X();


// Define stepper motor connections and steps per revolution:
#define dirPin1 A2
#define stepPin1 A3
#define dirPin2 6
#define stepPin2 9


//zaznavannje napake
int motor_upocasnitev = 500;
int ponovitve = 500;
int visina_z_osi = 0.5; // 2 
int vrtljaj_z_osi = 1600 * visina_z_osi; //3200 
bool napaka = false;
float razdalja = 0; //uvedba spremenjlivke zaradi unkcije
float x = 0;
float y = 0;
float z = 0;

void setup() {
    // Declare pins as output:
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);

   lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");


  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  while (!Serial) {;}
  //Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
  Serial.println("initialization failed!");
  while (1);
  }
  //Serial.println("initialization done.");

  while (!Serial) {
      delay(1);
    }
  Serial.println("Adafruit VL6180x test!");
  if (! vl.begin()) {
    Serial.println("Failed to find sensor");
    while (1);
  }
  Serial.println("Sensor found!");
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("This is a test file :)");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void errors(float status){
  napaka = true;
  if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
  Serial.println("System error");
  }
  else if (status == VL6180X_ERROR_ECEFAIL) {
    Serial.println("ECE failure");
  }
  else if (status == VL6180X_ERROR_NOCONVERGE) {
    Serial.println("No convergence");
  }
  else if (status == VL6180X_ERROR_RANGEIGNORE) {
    Serial.println("Ignoring range");
  }
  else if (status == VL6180X_ERROR_SNR) {
    Serial.println("Signal/Noise error");
  }
  else if (status == VL6180X_ERROR_RAWUFLOW) {
    Serial.println("Raw reading underflow");
  }
  else if (status == VL6180X_ERROR_RAWOFLOW) {
    Serial.println("Raw reading overflow");
  }
  else if (status == VL6180X_ERROR_RANGEUFLOW) {
    Serial.println("Range reading underflow");
  }
  else if (status == VL6180X_ERROR_RANGEOFLOW) {
    Serial.println("Range reading overflow");    
  }
  delay(50);
}
void kordinate(int i, int j, float range){
  
 
    int razdalja = 100-range;
      float kot = 3.1415926535897932384626433832795*2/3200 * j; //najmanjši kot ki ga lahko motor naredi s korakom in št korakov
    x = cos(kot) * razdalja; //kotna funkcija za izračun x kordinate
    y = sin(kot) * razdalja; //kotna funkcija za izračun y kordinate
    z = i * visina_z_osi;  // druga številka je za koliko se z os vzdigne ko se zavrti za en obrat
  

}
float laser(float naprava){
 /*  float lux = vl.readLux(naprava);

  Serial.print("Lux: "); Serial.println(lux);
   */
  uint8_t range = vl.readRange();
  uint8_t status = vl.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    //Serial.print("Range: "); Serial.println(range);
   // return range; //lux;
   razdalja = range;
  }else{// Some error occurred, print it out!
    errors(status);}
  
  return razdalja;
}
void sd(float x, float y, float z){
   myFile = SD.open("test.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
  Serial.print("Writing to test.txt...");
  myFile.print(x);
  myFile.print(";");
  myFile.print(y);
  myFile.print(";");
  myFile.println(z);

  // close the file:
  myFile.close();
  Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop() {

  digitalWrite(dirPin2, LOW); // Set the spinning direction clockwise:
  digitalWrite(dirPin1, LOW); // Smer vrtenja


  // Set the spinning direction counterclockwise:
  for ( int i = 0; i < ponovitve; i++){ // Kolikokrat se bo z os zavrtela za 360° 



    for (int j = 0; j < 3200; j++){ //vrtenje podlage za 2pi
    
      digitalWrite(stepPin2, HIGH);
      delayMicroseconds(motor_upocasnitev);
      digitalWrite(stepPin2, LOW);
      delayMicroseconds(motor_upocasnitev);
        
      if (!napaka){

        float range = laser(VL6180X_ALS_GAIN_5);
        //Serial.println(range);
        kordinate(i, j, range); 
        Serial.println(x);
        Serial.println(y);
        Serial.println(z);
        sd(x,y,z);
      }else{
        napaka = false; //Če je napaka jo ignorera in ne zapiše nič
      } 
    }
        for (int k = 0; k < vrtljaj_z_osi; k++){ //En vrtljaj na osi oz, laho se spremeni na kaj druzga 3200
      digitalWrite(stepPin1, HIGH);
      delayMicroseconds(motor_upocasnitev);
      
      digitalWrite(stepPin1, LOW);
      delayMicroseconds(motor_upocasnitev);
    }

  }
  
  digitalWrite(dirPin1, HIGH);
  // Set the spinning direction counterclockwise:
  for ( int i = 0; i < ponovitve; i++){ // Kolikokrat se bo z os zavrtela za 360°  in postavitev senzorja v začetno pozicijo
    
    for (int k = 0; k <3200; k++){ //En vrtljaj na osi oz, laho se spremeni na kaj druzga
      digitalWrite(stepPin1, HIGH);
      delayMicroseconds(motor_upocasnitev);
      
      digitalWrite(stepPin1, LOW);
      delayMicroseconds(motor_upocasnitev);
    }
  }
  
  while (1) //Po končanem skenu se prograam ustavi
  {
    /* code */
  }
  
}
