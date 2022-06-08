/*
Função que completa a ração no pote do pet 
*/
//void acionaMotor(void * parameter){
//  while(true){
//    printaData(); //atualiza data da variavel global
//
//    for(int i = 0; i <= 3; i++)
//    {
//        if(dataAgora[0] == agenda[i].hora && dataAgora[1] == agenda[i].minuto)
//        { 
//          block = true;   //debug 
//          vTaskDelay(100);
//          Serial.println("ENTREI NA FUNÇÃO DO MOTOR");
//          if(getPesoPote() >= agenda[i].peso){
//            Serial.println("Pote cheio");  
//          }else{
//                do
//                  {
//                   digitalWrite(motor, HIGH);
//                   vTaskDelay(100);
//                  }while (getPesoPote()<= agenda[i].peso);
//                }            
//            digitalWrite(motor, LOW);
//         }
//         block = false;
//    }
//    //block = false;
//    vTaskDelay(10000);
//  }   
//}    
