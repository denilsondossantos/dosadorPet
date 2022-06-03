 
void configBalancaPote() {
  Serial.println(escala.read());   // Aguarda até o dispositivo estar pronto
  escala.set_scale(913474.57);     // Substituir o valor encontrado para escala
  escala.tare(20);                // O peso é chamado de Tare.
}
 
float getPesoPote() {
  float p = 0;
  p = escala.get_units(20);
    if(p<0){
      return 0;
    }
  else{
   // Serial.println(String(p,3).toFloat());  
    return String(p,3).toFloat() * 1000;   
  }  
}

//-------------------------------------------------------------------
void configBalancaMaior() {
  Serial.println(escala1.read());
  escala1.set_scale(22.560);      // Substituir o valor encontrado para escala
  //escala1.tare(10);               // O peso é chamado de Tare.
  escala1.tare(20);               // O peso é chamado de Tare.
}
 
float getPesoMaior() {
 float peso = 0;
// char x[9]= {0};
 
 peso = escala1.get_units(20);
  if(peso < 0 ){
      return 0;
    }
  else{
   return String(peso,3).toFloat();
  }
}
