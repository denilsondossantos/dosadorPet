// Create RTC object
ErriezDS1307 rtc;

#define DATE_STRING_SHORT           3

// Month names in flash
const char monthNames_P[] PROGMEM = "JanFebMarAprMayJunJulAugSepOctNovDec";

// Day of the week names in flash
const char dayNames_P[] PROGMEM= "SunMonTueWedThuFriSat";


void configRelogio()
{
    // Initialize serial port
    delay(500);
    Serial.begin(115200);
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

//set data and time 
//    // Set date/time: 12:34:56 31 December 2020 Sunday
//    if (!rtc.setDateTime(10, 40, 0,  10, 4, 2022, 0)) {
//        Serial.println(F("Set date/time failed"));
//    }

    // Set square wave out pin
    // SquareWaveDisable, SquareWave1Hz, SquareWave4096Hz, SquareWave8192Hz, SquareWave32768Hz
    rtc.setSquareWave(SquareWaveDisable);
}

void rodaRelogio()
{
    char name[DATE_STRING_SHORT + 1];
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t mday;
    uint8_t mon;
    uint16_t year;
    uint8_t wday;

    // Read date/time
    if (!rtc.getDateTime(&hour, &min, &sec, &mday, &mon, &year, &wday)) {
        Serial.println(F("Read date/time failed"));
        return;
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


    //minhavariavel de testes
    if(hour == 11 && min  == 3 && sec == 0){
      Serial.println("toca alarme");
      }

    // Wait a second
    delay(1000);
}
