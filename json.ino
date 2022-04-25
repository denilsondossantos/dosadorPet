
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

/*
nome
raca
idade
peso
tipoRacao
pesoDispenser
pesoPote
agendas //verificar com wagner
*/

 
