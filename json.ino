
//----------------------------



//22/04/2022
/*
json precisa conter as seguintes informações
lista de agendamentos 
peso total
peso da balança
hora do equipamento
*/


String json (String nome, String raca, int idade, double peso, String tipoRacao, double pesoDispenser, double pesoPote){
  DynamicJsonDocument doc(1024);
  String texto;

  doc["nome"]            = nome;
  doc["raca"]            = raca;
  doc["idade"]           = idade;
  doc["peso"]            = peso;
  doc["tipoRacao"]       = tipoRacao;
  doc["pesoDispenser"]   = pesoDispenser;
  doc["pesoPote"]        = pesoPote;


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
