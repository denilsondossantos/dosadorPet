//----------------------------



//22/04/2022
/*
json precisa conter as seguintes informações
lista de agendamentos 
peso total
peso da balança
hora do equipamento
*/


String json(String nome, String raca, int idade, double peso, String tipoRacao, double pesoDispenser, float pesoPote, boolean comFome, int tempoComer){
  
  DynamicJsonDocument doc(1024);  //tamanho do documento json

  const int CAPACITY = JSON_OBJECT_SIZE(20);
  StaticJsonDocument<CAPACITY> docAgenda;
  JsonArray arr = docAgenda.to<JsonArray>();

    for(int i = 0; i <= tamanhoAgenda; i++){
    docAgenda[i]["hora"] =   agenda[i].hora;
    docAgenda[i]["minuto"] = agenda[i].minuto;
    docAgenda[i]["peso"] =   agenda[i].peso;
    }
 
  String texto;

  doc["nome"]            = nome;
  doc["raca"]            = raca;
  doc["idade"]           = idade;
  doc["peso"]            = peso;
  doc["tipoRacao"]       = tipoRacao;
  doc["pesoDispenser"]   = pesoDispenser;
  doc["pesoPote"]        = pesoPote;
  doc["comFome"]         = comFome;
  doc["tempoComer"]      = tempoComer;
  doc["agendas"]         = arr;  


  serializeJson(doc, texto);
  return texto;
}


String jsonData (int ano, int mes, int dia, int hora, int minuto, int segundo){
  DynamicJsonDocument doc(1024);
  String texto;

  doc["ano"]           = ano;
  doc["mes"]           = mes;
  doc["dia"]           = dia;
  doc["hora"]          = hora;
  doc["minuto"]        = minuto;
  doc["segundo"]       = segundo;

  serializeJson(doc, texto);
  return texto;
}

/*
Pega a reposta do post e atualiza a hora e data
*/
void jsonDataD(String json){ 
     
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);
  
  int ano     = doc["ano"];
  int mes     = doc["mes"];
  int dia     = doc["dia"];
  int hora    = doc["hora"];
  int minuto  = doc["minuto"];
  int segundo = doc["segundo"];

  //Chama a função config time para alterar data e hora.
  configTime(hora, minuto, segundo, dia, mes, ano);

  
}

void jsonD(String json){ 
     
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);
  
  String nome           = doc["nome"];
  String raca           = doc["raca"];
  int idade             = doc["idade"];
  double peso           = doc["peso"];
  String tipoRacao      = doc["tipoRacao"];
  double pesoDispenser  = doc["pesoDispenser"];
  double pesoPote       = doc["pesoPote"];
  boolean comFome       = doc["comFome"];
  int tempoComer        = doc["tempoComer"];


 tamanhoAgenda = doc["agendas"].size() -1;
//Serial.println(tamanhoAgenda); //debug 

  Serial.println("Atualizando agenda...");
  //funcional
  for(int i = 0; i <= tamanhoAgenda; i ++){
    agenda[i].hora = doc["agendas"][i]["hora"];
    //Serial.println(agenda[i].hora);
    agenda[i].minuto = doc["agendas"][i]["minuto"];
    //Serial.println(agenda[i].minuto);
    agenda[i].peso = doc["agendas"][i]["peso"];
    //Serial.println(agenda[i].peso);
    }

   Serial.println("Agenda atualizada");

}
