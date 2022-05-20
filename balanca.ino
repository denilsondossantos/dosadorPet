 
void configBalancaPote() {
  Serial.println(escala.read());   // Aguada até o dispositivo estar pronto
  escala.set_scale(913474.57);     // Substituir o valor encontrado para escala
  escala.tare(20);                // O peso é chamado de Tare.
}
 
float getPesoPote() {
  float p = 0;
  p = escala.get_units(20);
  //Serial.print("A última leitura do peso lido foi: ");
  //Serial.print(pesoPote);
  if(p<0){
    return 0;
    }
  else  
  return p;
}
