#include "HX711.h"
#define DT_PIN 16
#define SCK_PIN 17
HX711 scale;

float kg = 0;
float pesoLido = 0;
float taraPeso = 0;

void setup()
{
    scale.begin(DT_PIN, SCK_PIN);
    delay(1000);
    Serial.begin(9600);
    Serial.println("Serial iniciada:");
    taraPeso = calibra();
}

void loop()
{   
peso();
}


float calibra(){
  float maiorValor = 0;
  float  vetor [16];
  float tara = 0;

  Serial.println("Calibrando balança...");
  maiorValor  = scale.read_average();
    for(int x = 0; x <= 15; x++){
      vetor[x] = scale.read_average();
      if(maiorValor < scale.read_average()){
        maiorValor = scale.read_average();
        }
    }

    
tara  = maiorValor + 300;
return tara;

}

void peso(){
    pesoLido  = scale.get_units()/1000;
    if(scale.read_average() < taraPeso){
    kg = 0;
    Serial.print("KG: ");
    Serial.println(kg);
    
    }
    else if (pesoLido < 1000 && pesoLido > 10) {
      Serial.print("Gramas: ");
      Serial.println(pesoLido);
      delay(1000);
      }

      else{
      float pesoLido = scale.get_units() / 1000;
      Serial.println(pesoLido);
      delay(1000);
      }
    }
