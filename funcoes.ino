/*
#define motor 22


*/




/*
agenda(int hora, int minuto){ 
  for(int i = 0; i<= 3; i++){
    if(hora == h[i] && minuto == m[i]){
      acionaMotor(pesoGramas);
      }
  }      
}
*/


acionaMotor(void * parameter){
  while(true){  
    printaData(); //atualiza data da variavel global

      for(int i = 0; i <= 2; i++){
        if(horaAgora[0] == agenda[i].hora && horaAgora[1] == agenda[i].minuto){    
          while(100 <= 50){
            digitalWrite(motor, HIGH);
          }
          digitalWrite(motor, LOW);
        }
      }      
  xTaskDelay(10000);
  }   
}
