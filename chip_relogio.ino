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
    String retornoData = ""; 

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
    
    retornoData = jsonData(year, mon, mday, hour, min, sec);
    dataAgora[0] = hour;
    dataAgora[1] = min;
    //Serial.println(retornoData); //debug 

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
