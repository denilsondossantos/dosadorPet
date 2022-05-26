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
  String texto;
  DynamicJsonDocument doc(1024);  //tamanho do documento json

  DynamicJsonDocument docAgenda(255);
  JsonArray arr = docAgenda.to<JsonArray>();

  Serial.println("tamanho da agenda dentro da serializacao:");
  Serial.print(tamanhoAgenda);


    //condição para dizer que a variavel tamanhoAgenda não pode ser maior que 3.
    //obs.: alguma coisa esta acontecendo no código pra varivel se alterar
  if(tamanhoAgenda > 3){
    tamanhoAgenda = 3;
  } 

  Serial.println("Printa tamanhoagenda depois da condição:");
  Serial.print(tamanhoAgenda);

    for(int i = 0; i < AGENDA_INTERNAL_SIZE; i++){
    if(agenda[i].hora == -1){
      break; //se o objeto agenda for igual a 0, sai do for e não faz mais nada.
    }
    docAgenda[i]["hora"] =   agenda[i].hora;
    docAgenda[i]["minuto"] = agenda[i].minuto;
    docAgenda[i]["peso"] =   agenda[i].peso;
    }

  doc["nome"]            = nome;
  doc["raca"]            = raca;
  doc["idade"]           = idade;
  doc["peso"]            = peso;
  doc["tipoRacao"]       = tipoRacao;
  doc["pesoDispenser"]   = pesoDispenser;
  doc["pesoPote"]        = pesoPote;
  doc["comFome"]         = comFome;
  doc["tempoComer"]      = tempoComer;
  doc["agendas"]         = docAgenda;  


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
  
//  String nome           = ["nome"];
//  String raca           = doc["raca"];
//  int idade             = doc["idade"];
//  double peso           = doc["peso"];
//  String tipoRacao      = doc["tipoRacao"];
//  double pesoDispenser  = doc["pesoDispenser"];
//  double pesoPote       = doc["pesoPote"];
//  boolean comFome       = doc["comFome"];
//  int tempoComer        = doc["tempoComer"];


 infopet.nome           = doc["nome"].as<String>();
 infopet.raca           = doc["raca"].as<String>();
 infopet.idade          = doc["idade"];
 infopet.peso           = doc["peso"];
 infopet.tipoRacao      = doc["tipoRacao"].as<String>();
 infopet.pesoDispenser  = doc["pesoDispenser"];
 infopet.pesoPote       = doc["pesoPote"];
 infopet.comFome        = doc["comFome"];
 infopet.tempoComer     = doc["tempoComer"];

 int tamanhoAgenda = doc["agendas"].size();

 Serial.println("tamanho da agenda dentro de JsonD: ");
 Serial.print(tamanhoAgenda); //debug 
 Serial.println(" ");
 Serial.println("Atualizando agenda...");
  
  //Limpa TODA agenda antes de aplicar a nova.
  for(int i = 0; i < AGENDA_INTERNAL_SIZE; i++){
    agenda[i].hora = -1;
    agenda[i].minuto = -1;
    agenda[i].peso = -1;
  }
  //Aplica a nova agenda até onde couber na lista interna.
  for(int i = 0; i < tamanhoAgenda && i < AGENDA_INTERNAL_SIZE; i++){

    agenda[i].hora = doc["agendas"][i]["hora"];
    Serial.println(agenda[i].hora);
    agenda[i].minuto = doc["agendas"][i]["minuto"];
    Serial.println(agenda[i].minuto);
    agenda[i].peso = doc["agendas"][i]["peso"];
    Serial.println(agenda[i].peso);
  }

   Serial.println("Agenda atualizada");

}
