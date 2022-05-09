//---------------------- BIBLIOTECAS ---------------------------
#include <ArduinoJson.h>       //ok
#include <WiFi.h>              //nativo
#include <ESPAsyncWebServer.h> //baixar
#include <SPIFFS.h>            //ok
#include <ErriezDS1307.h>      //ok
#include <Wire.h>              //nativo
#include <HX711.h>             //ok
#include <FS.h>                //ok   


typedef struct {
    //parametros
    int hora;
    int minuto;
    double peso;

}Agenda;

//------------------------ OBJETOS -------------------------- 
HX711 escala;    
Agenda agenda[2];

//---------------------------------------------------------------

#define SDA 23 //define pinos I2C do chip relógio
#define SCL 19

#define DT_PIN 16 //define pinos I2C da balança do pote
#define SCK_PIN 17

#define motor 12

//----------------- credenciais wifi ----------------------------
const char *ssid = "dosador_pet";
const char *password = "dosadorpet";
//-----------------------------------------------------------------
//Configurações chip relogio
ErriezDS1307 rtc;
#define DATE_STRING_SHORT           3

//---------------- variaveis de teste------------------------------

//String dados = "";
char dados[256] = {0};
String dadosPet = "";
String infoData = "";
float pesoPote = 0;
//------------------------------------------------
AsyncWebServer server(80);




void setup()
{ 

  //inicializa serial
  Serial.begin(19200);

  Serial.println("------------------");
  Serial.println("PROGRAMA INICIADO");
  Serial.println("------------------");

  pinMode(motor, OUTPUT);   //define motor como uma saída 

  //inicia balança  
  //escala.begin (DT_PIN, SCK_PIN);
  //configBalancaPote();  //Seta configurações da baçança do pote.
  //Serial.print(getPesoPote());
   
  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("Um erro ocorreu durante a montagem da SPIFFS");
    return;
  }

  //Seta configurações do chip relógio
  configRelogio();
  //configTime(12,10,0,30,4,2022);

 

  readFile(SPIFFS, "/default.txt");  //debug
  Serial.println("");

  //inicia  acess point
  WiFi.softAP(ssid, password);
  IPAddress meuIP = WiFi.softAPIP();
  Serial.print("IP do equipamento: ");
  Serial.println(meuIP);

  //criar rota para servir imagens
  // Route for root / web page//pasta raiz quer dizer o ip da página
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/plain", "servidor rodando"); //devolve pagina contendo as informações do gato });
  }); 
//
//  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request){
//  request->send(200, "text/plain", json("rex", "pastor alemao", 5, 20.0, "DogShow", 22, 2.3)); //devolve json contendo todas as informações });
//  });

  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/default.txt", "text/plain" );
  });

  server.on("/gato", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/gato.png", "image/png"); //devolve imagem do pet
  });

  server.on("/image-get", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/cat.png", "image/png"); //devolve imagem do pet
  });

  server.on("/rain", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/rain.png", "image/png"); //devolve imagem do pet
  });

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
  printaData();
  request->send(200, "text/plain", printaData()); //devolve data e hora salvas no equipamento
  });

  //debug
  server.on("/peso", HTTP_GET, [](AsyncWebServerRequest *request){
  String peso = String(getPesoPote(),3);
  request->send(200, "text/plain", peso); //devolve data e hora salvas no equipamento
  });

//    //testeJson
//  server.on("/json-teste", HTTP_GET, [](AsyncWebServerRequest *request){
//  String jsont = readFile(SPIFFS, "/default.txt");
//  request->send(200, "text/plain", "teste json"); //devolve data e hora salvas no equipamento
//  });


  /*Seta hora e data do equipamento*/
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
        //Serial.write(data[i]);  //debug
        dados[i]= data[i];                         
      }
      for(int i = 0; i <= len ; i++){
        dadosPet = dadosPet + dados[i];
      }

      //Serial.print(dadosPet);  //debug 
      writeFile(SPIFFS, "/default.txt", dados);
      readFile(SPIFFS, "/default.txt");
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
}




