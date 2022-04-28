// Create RTC object


// Month names in flash
const char monthNames_P[] PROGMEM = "JanFebMarAprMayJunJulAugSepOctNovDec";

// Day of the week names in flash
const char dayNames_P[] PROGMEM= "SunMonTueWedThuFriSat";


void configRelogio()
{     
    while (!Serial) {
        ;
    }
    Serial.println(F("\nErriez DS1307 read example"));

    // Initialize I2C
    Wire.begin(SDA, SCL);
    Wire.setClock(100000);

    // Initialize RTC
    while (!rtc.begin()) {
        Serial.println(F("RTC not found"));
        delay(3000);
    }



    // Set square wave out pin
    // SquareWaveDisable, SquareWave1Hz, SquareWave4096Hz, SquareWave8192Hz, SquareWave32768Hz
    rtc.setSquareWave(SquareWaveDisable);
}

String printaData()
{   
    String retornoData = ""; //variavel de teste 

    char name[DATE_STRING_SHORT + 1];
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t mday;
    uint8_t mon;
    uint16_t year;
    uint8_t wday;

    // Read date/time
    while (!rtc.getDateTime(&hour, &min, &sec, &mday, &mon, &year, &wday)) {
        Serial.println(F("Read date/time failed"));
        //return;
        break;
    }

    // Print day week
    strncpy_P(name, &(dayNames_P[wday * DATE_STRING_SHORT]), DATE_STRING_SHORT);
    name[DATE_STRING_SHORT] = '\0';
    Serial.print(name);
    Serial.print(F(" "));

    // Print month
    strncpy_P(name, &(monthNames_P[(mon - 1) * DATE_STRING_SHORT]), DATE_STRING_SHORT);
    name[DATE_STRING_SHORT] = '\0';
    Serial.print(name);
    Serial.print(F(" "));

    // Print day month
    Serial.print(mday);
    Serial.print(F(" "));

    // Print time
    Serial.print(hour);
    Serial.print(F(":"));
    if (min < 10) {
        Serial.print(F("0"));
    }
    Serial.print(min);
    Serial.print(F(":"));
    if (sec < 10) {
        Serial.print(F("0"));
    }
    Serial.print(sec);
    Serial.print(F(" "));

    // Print year
    Serial.println(year);



    
    retornoData = jsonData(year, mon, mday, hour, min, sec);
    Serial.println(retornoData); //debug 

return retornoData;
}


void configTime(int hora, int minuto, int segundo, int dia, int mes, int ano){
  //set data and time 
     //Set date/time: 12:34:56 31 December 2020 Sunday
     //(!rtc.setDateTime(13, 36, 0,  10, 4, 2022, 0))
    if (!rtc.setDateTime(hora, minuto, segundo,  dia, mes, ano, 0)) {
        Serial.println(F("Set date/time failed"));
    }  
  }
