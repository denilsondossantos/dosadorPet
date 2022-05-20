
/*
Função que completa a ração no pote do pet 
*/
void acionaMotor(void * parameter){
  while(true){
    printaData(); //atualiza data da variavel global
      //    Serial.println("Entrei na task motor");  
      //    Serial.print("hora:");  
      //    Serial.print(dataAgora[0]);  
      //    Serial.print(" "); 
      //    Serial.print("minuto:");   
      //    Serial.print(dataAgora[1]);
      //    Serial.println("\n");

    //Roda o for até achar o primeiro item "Lixo" da agenda ou até terminar a agenda.
    for(int i = 0; agenda[i].hora!=-1 && i<AGENDA_INTERNAL_SIZE; i++){
        if(dataAgora[0] == agenda[i].hora && dataAgora[1] == agenda[i].minuto){ 
          Serial.println("ENTREI NA FUNÇÃO DO MOTOR");   
          if(getPesoPote() >= agenda[i].peso/1000.0){
            Serial.println("Pote cheio");  
          }else{
                do
                  {
                   digitalWrite(motor, HIGH);
                   vTaskDelay(100);
              }while (getPesoPote()<= agenda[i].peso/1000.0);
            }            
            digitalWrite(motor, LOW);
          }
    }
    vTaskDelay(10000);
  }   
}      
  
