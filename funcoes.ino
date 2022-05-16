
/*
Função que completa a ração no pote do pet 
*/
void acionaMotor(void * parameter){
  while(true){
    Serial.println("Entrei na task motor");  
    printaData(); //atualiza data da variavel global
    Serial.print("hora:");  
    Serial.print(dataAgora[0]);  
    Serial.print(" "); 
    Serial.print("minuto:");   
    Serial.print(dataAgora[1]);
    Serial.println("\n");

      for(int i = 0; i <= tamanhoAgenda; i++){
        if(dataAgora[0] == agenda[i].hora && dataAgora[1] == agenda[i].minuto){ 
          Serial.println("ENTREI NA FUNÇÃO DO MOTOR");   
          if(getPesoPote() >= 0.100){
            Serial.println("Pote cheio");  
          }else{
                do
                  {
                   digitalWrite(motor, HIGH);
                   vTaskDelay(100);
              }while (getPesoPote()<= 0.100);
            }            
            digitalWrite(motor, LOW);
          }
        }
        vTaskDelay(10000);
      }      
  }   
