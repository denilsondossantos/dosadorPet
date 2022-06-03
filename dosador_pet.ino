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


typedef struct { 
  String nome = "";
  String raca = "";
  int idade = 0;
  double peso = 0.0;
  String tipoRacao = "";
  double pesoDispenser = 0.0;
  double pesoPote = 0.0;
  boolean comFome = false;
  int tempoComer = 0;
  }infoPet;

//------------------------ OBJETOS -------------------------- 
HX711 escala;    
HX711 escala1; 

Agenda agenda[10];
infoPet infopet;

//---------------------------------------------------------------

#define SDA 23 //define pinos I2C do chip relógio
#define SCL 19

#define DT_PIN 16 //define pinos I2C da balança do pote
#define SCK_PIN 17

#define DT 26 //define pinos I2C da balança maior
#define SCK 27

#define motor 13

//----------------- credenciais wifi ----------------------------
const char *ssid = "dosador_pet";
const char *password = "dosadorpet";
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
int dataAgora[1] = {0};


//------------------------------------------------
AsyncWebServer server(80);




void setup()
{ 
  pinMode(motor, OUTPUT);   //define motor como uma saída 

  //inicializa serial
  Serial.begin(19200);

  //Seta configurações do chip relógio
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

  //inicia  acess point
  WiFi.softAP(ssid, password);
  IPAddress meuIP = WiFi.softAPIP();
  Serial.print("IP do equipamento: ");
  Serial.println(meuIP);

  Serial.println("------------------");
  Serial.println("PROGRAMA INICIADO");
  Serial.println("------------------");

  infoApp = readFileString(SPIFFS, "/default.txt");
  jsonD(infoApp);  //le e atualiza informações da agenda
  infoApp = "";

    xTaskCreate(
                    acionaMotor,          /* Task function. */
                    "acionaMotor",        /* String with name of task. */
                    10000,                /* Stack size in bytes. */
                    NULL,                 /* Parameter passed as input of the task */
                    1,                   /* Priority of the task. */
                    NULL); 
                      
  //criar rota para servir imagens
  // Route for root / web page//pasta raiz quer dizer o ip da página
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/plain", "servidor rodando"); //devolve pagina contendo as informações do gato });
  }); 

  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request){

  //Gambiarra para atualizar objeto agenda : e te certeza que os valores da agenda foram atualizados.   
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
  infopet.tempoComer)); //devolve json contendo todas as informações });
  });


  /*Toda vez que o app se conecta ele le e atualiza as informaçãoes da agenda*/
//  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request){
//  infoApp = readFileString(SPIFFS, "/default.txt");
//  jsonD(infoApp);  //le e atualiza informações da agenda
//  infoApp = "";
//  request->send(SPIFFS, "/default.txt", "text/plain" );
//  });

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
  request->send(200, "text/plain", String(getPesoPote(),3)); //devolve data e hora salvas no equipamento
  });




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
}
