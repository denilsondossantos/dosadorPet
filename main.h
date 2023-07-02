#include <Arduino.h>


#define SDA     23 //define pinos I2C do chip relógio
#define SCL     19
#define DT_PIN  16 //define pinos I2C da balança do pote
#define SCK_PIN 17
#define DT      26 //define pinos I2C da balança maior
#define SCK     27
#define motor   13



typedef struct { 
  String nome;
  String raca;
  int idade;
  double peso;
  String tipoRacao;
  double pesoDispenser;
  double pesoPote;
  boolean comFome;
  int tempoComer;
}infoPet;