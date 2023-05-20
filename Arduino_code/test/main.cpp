#include <Arduino.h>                                  //Povemo prgramu da naj to kompila za arduiono če kompilamo v arduino IDE jo je potrebno izbrisati
#include <Wire.h>                                     //Uvozimo knjižnico, ki vsebuje navodila za komunikacijo preko I2C-ja
#include <Adafruit_VL6180X.h>                         //Uvozimo knjižnico, ki vsebuje navodila za dešifracijo podatkov pridobljenih iz senzorja
#include <Stepper.h>                                  //Uvozimo knjižnico, ki vsebuje funkcijo za pogovor z koračnim motorjem
#include "SPI.h"

int numberofstep = 48*64;                       //Definirmo število korakov, ki jih lahko koračni motor naredi                                  
Stepper vertikalniMotor(numberofstep,5,7,6,4);  //Definiramo motor ter pine na katere je priključen
Stepper krozniMotor(numberofstep,9,11,10,8);    //Definiramo drug motor ter pine na katere je definiran    

Adafruit_VL6180X vl = Adafruit_VL6180X();      //Odpremo knjižnico za dešifriranje podatkov iz merilnika razdalj

int y = 0;    //Nastavimo pozicijo krožnega motorja
int z = 1;    //Nastavimo pozicijo vertikalnega motorja

void setup() {              //Začetne nastavitve, ki se zaženejo samo enkrat
  Serial.begin(115200);     //Aktiviramo serijsko povezo


  while (!Serial) {            //Počakamo in pogledamo če je komunikacija z računalnikom vzpostavljena
    delay(1);                  //Počakamo stotinko
  }

  Serial.println("Adafruit VL6180x test!");        //Preko serijskega porta povemo, da bomo testirali senzor
  if (! vl.begin()) {                              //Tvorimo if stavek z rezultatom o odvisnosti senzorja za razdalje
    Serial.println("Failed to find sensor");       //Če nismo uspeli najti senzorja za razdalje in nam to pove
    while (1);                                     // Ko funkcija drži jo preskočimo
  }
  Serial.println("Sensor found!");     //Če smo prišli do tega dela kode nam pove da smo našli senzor in da lahko nadaljujemo
  Serial.println("dodatna vrstica");   //To je pomožna vrstica za usklajevanje z Python programu
  
  krozniMotor.setSpeed(9);            //Nastavitev hitrosti krožnega motorja
  vertikalniMotor.setSpeed(9);        //Nastavitev hitrosti verikalnega motorja

}


void loop() {                        //Začetek glavne zanke

for(z=0;z<2048;z++){                //For zanka ki premakne potor iz zčetne pozicije do najvišje pozicije na vertikalni osi
  vertikalniMotor.step(1);          //Funkcija, ki premakne motor za prej nastavljeno eno stopnjo
  for(y=0;y<2048;y++){              //For zanka, ki zavrti kororačni motor za cel krog
      krozniMotor.step(1);          //Funkcija katera zavrti krožni motor za prej določeno enoto; the motor will run during 600 step (you can change the step)
      z = z;                        //Z spremenljivki dodelimo novo vrednos
      Serial.print("visina : "), Serial.print(z);     //Izpišemom višino preko serijskega izhoda
      Serial.print(" ,vrtljaj : "),Serial.print(y);   //Izpišemo vrtljaj preko serijskega izhoda
      

    /* konec motorčka za y os
    */
      uint8_t range = vl.readRange();             //Preberemo razdaljo iz senzorja, če lahko, če dobi nič izpusti
      uint8_t status = vl.readRangeStatus();      //Prebememo status senzorja (Če je znotraj kateri error)

      if (status == VL6180X_ERROR_NONE) {                   //Pogledamo če senzor je senzor dobil napake
        Serial.print(", Range: "); Serial.println(range);   //Na serijska vrate so izpisani podatki meritev
      }

      // Če se zgodi napaka, jo izpiše na uporabniku prijazen način!
      
      if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {     //če je bil kakšen error bomo pregledal če je ta specifična
        Serial.println("System error");                                                    //Če je bila ta napaka nam pove da je nekaj narobe s programom
      }
      else if (status == VL6180X_ERROR_ECEFAIL) {                                        //Pogleda če je ta specifična napaka zaznana
        Serial.println("ECE failure");                                                   //nam pove da senzor ne dela
      } 
      else if (status == VL6180X_ERROR_NOCONVERGE) {                                     //Pregleda če se pojavi ta specifična napaka
        Serial.println("No convergence");                                                //Nam pove da ne najde programa za dešifriranje podatkov
      }
      else if (status == VL6180X_ERROR_RANGEIGNORE) {                                    //Pregleda če se pojavi ta specifična napaka
        Serial.println("Ignoring range");                                                //Pove da ni nč pred senzorjem oz. da je preveč oddaljeno
      }
      else if (status == VL6180X_ERROR_SNR) {                                            //Pregleda če se pojavi ta specifična napaka
        Serial.println("Signal/Noise error");                                            //Pove da je v podatkih preveč šuma in da ne more razbrati le teh
      }
      else if (status == VL6180X_ERROR_RAWUFLOW) {                                       //Pregleda če se pojavi ta specifična napaka
        Serial.println("Raw reading underflow");                                         //Laser se ne odbije nazaj v senzor
      }
      else if (status == VL6180X_ERROR_RAWOFLOW) {                                       //Pregleda če se pojavi ta specifična napaka
        Serial.println("Raw reading overflow");                                          //Preveč laserja se odbije nazaj
      }
      else if (status == VL6180X_ERROR_RANGEUFLOW) {                                     //Pregleda če se pojavi ta specifična napaka
        Serial.println("Range reading underflow");                                       //Podatek ki ga je hotel laser pretvoriti v številko je premejhen da bi ga poslal
      }
      else if (status == VL6180X_ERROR_RANGEOFLOW) {                                     //Pregleda če se pojavi ta specifična napaka
        Serial.println("Range reading overflow");                                        //Podatke, ki ga je hotel program poslati kot številko je prevelik da bi ga lahko poslal (Npr laser rabi predolgo časa da se odbije nazaj(Amplituda laserja je zanesljiva ampak čas je prevelik))
      }
    delay(20);        //Počakamo 20 stotink
    }
  }
}
