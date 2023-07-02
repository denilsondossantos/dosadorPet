//---------------------- BIBLIOTECAS ---------------------------
#include <ArduinoJson.h>       //ok
#include <ESPAsyncWebServer.h> //baixar
#include <SPIFFS.h>            //ok
#include <ErriezDS1307.h>      //ok
#include <Wire.h>              //nativo
#include <HX711.h>             //ok
#include <FS.h>                //ok   

#include "main.h"
#include "wifi.h"

typedef struct {
    int hora;
    int minuto;
    double peso;
}Agenda;


//------------------------ OBJETOS -------------------------- 
HX711 escala;    
HX711 escala1; 

Agenda agenda[10];
infoPet infopet = {0};

//-----------------------------------------------------------------
//Configurações chip relogio
ErriezDS1307 rtc;
#define DATE_STRING_SHORT           3

//---------------- variaveis de teste / globais ------------------------------

//String dados = "";
char dados[256] = {0};
String dadosPet = "";
String infoData = "";
String infoApp = "";
double pesoPote = 0.0;
int tamanhoAgenda = 0;
int dataAgora[2] = {0};
//boolean block =  false; //bloqueia requições a rota get enquanto o motor estiver funcionando


//------------------------------------------------
AsyncWebServer server(80);




void setup()
{ 
  pinMode(motor, OUTPUT);   //define motor como uma saída 

  Serial.begin(19200);
  configRelogio();
  //configTime(12,10,0,30,4,2022);

  //inicia balança  
  escala.begin (DT_PIN, SCK_PIN);
  escala1.begin (DT, SCK);
  
  configBalancaPote();  //Seta configurações da baçança do pote.
  configBalancaMaior();
   
  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("Um erro ocorreu durante a montagem da SPIFFS");
    return;
  }
  readFile(SPIFFS, "/default.txt");  //debug
  Serial.println("");

  initWifi();

  Serial.println("------------------");
  Serial.println("PROGRAMA INICIADO");
  Serial.println("------------------");

  infoApp = readFileString(SPIFFS, "/default.txt");
  jsonD(infoApp);  //le e atualiza informações da agenda
  infoApp = "";


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/plain", "servidor rodando");
  }); 

  //route for get general informations
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request){
  infoApp = readFileString(SPIFFS, "/default.txt");
  jsonD(infoApp);  //le e atualiza informações da agenda
  infoApp = "";  

  request->send(200, "text/plain", 
  json(infopet.nome, 
  infopet.raca, 
  infopet.idade, 
  infopet.peso, 
  infopet.tipoRacao, 
  getPesoMaior(), 
  getPesoPote(), 
  infopet.comFome, 
  infopet.tempoComer));

 });     

  server.on("/gato", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/gato.png", "image/png"); //devolve imagem do pet
  });

  server.on("/image-get", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/cat.png", "image/png"); //devolve imagem do pet
  });

  server.on("/teste", HTTP_GET, [](AsyncWebServerRequest *request){
  Serial.println(tamanhoAgenda);
  request->send(200, "text/plain", json("cachorro", "pitbull", 5, 100, "dogshow", 100, getPesoPote(), false, 15));
  });

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
  printaData();
  request->send(200, "text/plain", printaData()); //devolve data e hora salvas no equipamento
  });

  //debug
  server.on("/peso", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/plain", String(getPesoPote(),3));
  });

  //route for update hour and data in device
  server.on(
    "/data", HTTP_POST,
    [](AsyncWebServerRequest *request) {},
    NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
      for (size_t i = 0; i < len; i++)
      {
        dados[i]= data[i];                         
      }
      for(int i = 0; i <= len ; i++){
        infoData = infoData + dados[i];
      }

      Serial.print(infoData);
      jsonDataD(infoData);
      
      // depois que guardar na spiffs tem que zerar a variavel
      memset(dados,0,256);
      infoData = "";
      dadosPet = "";
      Serial.println();
     request->send(200);
  });

  server.on(
    "/post", HTTP_POST,
    [](AsyncWebServerRequest *request) {},
    NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
      for (size_t i = 0; i < len; i++)
      {
        dados[i]= data[i];                         
      }
      for(int i = 0; i <= len ; i++){
        dadosPet = dadosPet + dados[i];
      }
      writeFile(SPIFFS, "/default.txt", dados);
      readFile(SPIFFS, "/default.txt");

      infoApp = readFileString(SPIFFS, "/default.txt");
      jsonD(infoApp);  //le e atualiza informações da agenda
      infoApp = "";
      dadosPet = "";
      memset(dados,0,256);
      Serial.println();
      request->send(200);
  });

server.on(
    "/image-post", HTTP_POST,
    [](AsyncWebServerRequest *request) {},
    NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
      if(index==0) writeFileByte(SPIFFS, "/cat.png", (char *)data, len); //Escreve por cima o começo do arquivo
      else appendFileByte(SPIFFS, "/cat.png", (char *)data, len); //Vai adicionando o resto dos dados no fim do arquivo.
    
      if(index+len==total) request->send(200);
  });

 server.begin();
}


void loop()
{
  bool liberaComida = false;
  int racao = -1;
  printaData();
  for(int i = 0; i < 3; i++){
    if(dataAgora[0] == agenda[i].hora && dataAgora[1] == agenda[i].minuto){
      liberaComida = true;
      racao = agenda[i].peso;
      }        
    }
    if(getPesoPote() < racao && liberaComida && getPesoMaior() > 20){
      liberaComida = true;
      digitalWrite(motor, HIGH);
    }
    else{
      digitalWrite(motor, LOW);
      racao = -1;
      liberaComida = false;
    }
}
