
/*
Scanning...
0x57 - 0x68 - 'DS3231 real-time clock' OR 'MPU-9250 Nine axis sensor module'
0x77 - 'BMP180 barometric pressure sensor' OR 'BMP280 barometric pressure sensor'
done
*/
/*-----------------------------------------ОБЩЕЕ-------------------------------------------------------*/
//Библиотеки общие
//I2C экран часы датчик BMP

#include <Wire.h> 

//Запрос системного времени для функций
unsigned long Period;
void CurrentTime() { Period = millis(); }
bool BthSw = true;
bool WFSw = true;
/*------------------------------------------ЧАСЫ--------------------------------------------------------*/

//Библиотека часов
#include <iarduino_RTC.h>
iarduino_RTC Time(RTC_DS3231);

//интервал запроса времени
unsigned long PeriodClocks;
unsigned long IntervalClocks = 7000;

//Переменные запроса
int YY, W, MM, DD, hh, mm, ss;

//установка времени
bool SetupTime = false;

//Запуск
void ClockSetup() {
	Time.begin();
}

//Установка
int SetYY, SetW, SetMM, SetDD, Sethh, Setmm, Setss;

void SetupTimeClock(int SetYY, int SetW, int SetMM, int SetDD, int Sethh, int Setmm, int Setss) {
	//день недели указывается в виде числа от 0-воскресенье до 6-суббота
	if (SetupTime) {
		Time.settime(Setss, Setmm, Sethh, SetDD, SetMM, SetYY, SetW); 
		SetupTime = false;
	}
}

//Цикл чтения
void ReadClock() {
	if (PeriodClocks < Period) {
		Time.gettime();
		YY = Time.year;
		MM = Time.month;
		DD = Time.day;
		hh = Time.Hours;
		mm = Time.minutes;
		ss = Time.seconds;
		W = Time.weekday;
		PeriodClocks = Period + IntervalClocks;
	}
}

/*---------------------------------------------BMP------------------------------------------------------*/

//Библиотека 
#include <Adafruit_BMP085.h>
Adafruit_BMP085 BMP;

//интервал запроса
unsigned long PeriodBMP;
unsigned long IntervalBMP = 17000;

//Переменные запроса
float temperature;
float pressure;
float altitude;
int BMPTemper;
int BMPPress;
int BMPAlt;

//Рабочие переменные
bool BMPON;

//Запуск
void BMPSetup() {
	if (!BMP.begin()) { BMPON = false; }
	else { BMPON = true; }
}

//Цикл чтения
void BMPRead() {
	if (BMPON == true && PeriodBMP<Period) {
		temperature = BMP.readTemperature();
		pressure = BMP.readPressure() / 133.3;
		altitude = BMP.readAltitude(100838);
		PeriodBMP = Period + IntervalBMP;
		BMPTemper = int(temperature);
		BMPPress = int(pressure);
		BMPAlt = int(altitude);
	}
}

/*-------------------------------------------Температура и влажность----------------------------------------------------*/

//Библиотека 

#include <DHT.h>

#define DHT1 22
#define DHT2 23
#define DHT3 24
#define DHT4 25
#define DHT5 26

#define DHTTYPE DHT11

DHT Dht1(DHT1, DHTTYPE);
DHT Dht2(DHT2, DHTTYPE);
DHT Dht3(DHT3, DHTTYPE);
DHT Dht4(DHT4, DHTTYPE);
DHT Dht5(DHT5, DHTTYPE);

//интервал запроса
unsigned long PeriodDHT;
unsigned long IntervalDHT = 13000;

//Переменные запроса
int dhtT1, dhtT2, dhtT3, dhtT4, dhtT5;     //Температура с датчиков
int dhtH1, dhtH2, dhtH3, dhtH4, dhtH5;	   //Влажность с датчиков

//Рабочие переменные
bool DHTON;																				//Выключатель опроса
//Установка 

//Изменение

//Запуск
void TempHumDHTSetup() {
	pinMode(DHT1, INPUT);
	pinMode(DHT2, INPUT);
	pinMode(DHT3, INPUT);
	pinMode(DHT4, INPUT);
	pinMode(DHT5, INPUT);

	Dht1.begin();
	Dht2.begin();
	Dht3.begin();
	Dht4.begin();
	Dht5.begin();
}
//Чтение датчика
int DHTREAD(int Num, int Type) {
	int Value;
	if (Num == 1) {
		if (Type == 1) {
			Value = Dht1.readHumidity();
		}
		else {
			Value = Dht1.readTemperature();
		}
	}
	else if (Num == 2) {
		if (Type == 1) {
			Value = Dht2.readHumidity();
		}
		else {
			Value = Dht2.readTemperature();
		}
	}
	else if (Num == 3) {
		if (Type == 1) {
			Value = Dht3.readHumidity();
		}
		else {
			Value = Dht3.readTemperature();
		}
	}
	else if (Num == 4) {
		if (Type == 1) {
			Value = Dht4.readHumidity();
		}
		else {
			Value = Dht4.readTemperature();
		}
	}
	else if (Num == 5) {
		if (Type == 1) {
			Value = Dht5.readHumidity();
		}
		else {
			Value = Dht5.readTemperature();
		}
	}
	else {

	}
	if (isnan(Value)) {
		Value = -99;
	}
	return Value;
}

//Цикл
void TempHumDHT() {
	if (DHTON == true && PeriodDHT < Period) {
		dhtH1 = DHTREAD(1, 1);
		dhtH2 = DHTREAD(2, 1);
		dhtH3 = DHTREAD(3, 1);
		dhtH4 = DHTREAD(4, 1);
		dhtH5 = DHTREAD(5, 1);
		dhtT1 = DHTREAD(1, 0);
		dhtT2 = DHTREAD(2, 0);
		dhtT3 = DHTREAD(3, 0);
		dhtT4 = DHTREAD(4, 0);
		dhtT5 = DHTREAD(5, 0);
		PeriodDHT = Period + IntervalDHT;
	}
}

/*-------------------------------------------ОБОГРЕВ-------------------------------------------------*/

//Библиотека 
#define HeatPin1 2
#define HeatPin2 3
#define HeatPin3 4
#define HeatPin4 5
#define HeatPin5 6
//интервал запроса

unsigned long PeriodHeat;
unsigned long IntervalHeat = 5000;


//Переменные 
int Heat1, Heat2, Heat3, Heat4, Heat5;     //Температура включения
int HeatGyst;                              //Гистерезис

//Рабочие переменные

bool HeatAll;																   //Выключатель обогрева общий
bool HeatSw1, HeatSw2, HeatSw3, HeatSw4, HeatSw5;							   //Выключатель обогрева по отсекам
bool HeatOn1, HeatOn2, HeatOn3, HeatOn4, HeatOn5;							   //Команда на MOSFET




//Запуск
void HeatSetup() {
	pinMode(HeatPin1, OUTPUT);
	digitalWrite(HeatPin1, LOW);
	pinMode(HeatPin2, OUTPUT);
	digitalWrite(HeatPin2, LOW);
	pinMode(HeatPin3, OUTPUT);
	digitalWrite(HeatPin3, LOW);
	pinMode(HeatPin4, OUTPUT);
	digitalWrite(HeatPin4, LOW);
	pinMode(HeatPin5, OUTPUT);
	digitalWrite(HeatPin5, LOW);
}
//Обработка
bool SwitchHeat(bool HeatS, int TempDHT, int TempHeat) {
	bool SwH = false;
	int TempHigh = TempHeat+ HeatGyst;
	if (HeatAll && HeatS && TempDHT < TempHeat && TempDHT < TempHigh) {
		SwH = true;
	}
	return SwH;
}

//Проверка 
void HeatCheck() {
	if (dhtT1 == -99) {
		HeatOn1 = false;
	}
	if (dhtT2 == -99) {
		HeatOn2 = false;
	}
	if (dhtT3 == -99) {
		HeatOn3 = false;
	}
	if (dhtT4 == -99) {
		HeatOn4 = false;
	}
	if (dhtT5 == -99) {
		HeatOn5 = false;
	}
}

//Цикл
void Heat() {
	if (PeriodHeat < Period) {
		HeatOn1 = SwitchHeat(HeatSw1, dhtT1, Heat1);
		HeatOn2 = SwitchHeat(HeatSw2, dhtT2, Heat2);
		HeatOn3 = SwitchHeat(HeatSw3, dhtT3, Heat3);
		HeatOn4 = SwitchHeat(HeatSw4, dhtT4, Heat4);
		HeatOn5 = SwitchHeat(HeatSw5, dhtT5, Heat5);
		HeatCheck();
		digitalWrite(HeatPin1, HeatOn1);
		digitalWrite(HeatPin2, HeatOn2);
		digitalWrite(HeatPin3, HeatOn3);
		digitalWrite(HeatPin4, HeatOn4);
		digitalWrite(HeatPin5, HeatOn5);
		PeriodHeat = Period + IntervalHeat;
	}
}



/*-------------------------------------------Влажность земли-------------------------------------------------*/

//Библиотека 
#define AWL A12		   //Пин уровня воды для полива

#define HWLvPin A13    //Пин питания датчиков влажности почвы

#define AVl11 A0	   //Пин датчика влажности почвы 1 отсек 1
#define AVl12 A1	   //Пин датчика влажности почвы 2 отсек 1
#define AVl13 A2	   //Пин датчика влажности почвы 3 отсек 1
#define AVl14 A3	   //Пин датчика влажности почвы 4 отсек 1
#define AVl15 A4	   //Пин датчика влажности почвы 5 отсек 1
#define AVl16 A5	   //Пин датчика влажности почвы 6 отсек 1

#define AVl21 A6	   //Пин датчика влажности почвы 1 отсек 2
#define AVl22 A7	   //Пин датчика влажности почвы 2 отсек 2
#define AVl23 A8	   //Пин датчика влажности почвы 3 отсек 2
#define AVl24 A9	   //Пин датчика влажности почвы 4 отсек 2
#define AVl25 A10	   //Пин датчика влажности почвы 5 отсек 2
#define AVl26 A11	   //Пин датчика влажности почвы 6 отсек 2

//интервал запроса
unsigned long PeriodHWL;
unsigned long IntervalHWL=60000; 

//Переменные запроса
int HVl11, HVl12, HVl13, HVl14, HVl15, HVl16, HVl21, HVl22, HVl23, HVl24, HVl25, HVl26; //Влажность земли
int WLv;																				//Уровень Воды

//Рабочие переменные
bool HWLvSw;																			//Питание датчиков
//Установка 

//Изменение

//Запуск
void GroundSetup() {
	pinMode(HWLvPin, OUTPUT);
	digitalWrite(HWLvPin, HIGH);
	pinMode(AWL, INPUT);
	pinMode(AVl11, INPUT);
	pinMode(AVl12, INPUT);
	pinMode(AVl13, INPUT);
	pinMode(AVl14, INPUT);
	pinMode(AVl15, INPUT);
	pinMode(AVl16, INPUT);
	pinMode(AVl21, INPUT);
	pinMode(AVl22, INPUT);
	pinMode(AVl23, INPUT);
	pinMode(AVl24, INPUT);
	pinMode(AVl25, INPUT);
	pinMode(AVl26, INPUT);
	HWLvSw = true;
}
//Чтение

int VlRead(int Sensor) {
	int Value;
	Value = map(constrain(1024 - analogRead(Sensor), 0, 1024), 0, 1024, 0, 100);
	return Value;
}

//Цикл

void Ground() {
	if (HWLvSw == true && PeriodHWL < Period) {
		digitalWrite(HWLvPin, LOW);
		delay(500);
		HVl11 = VlRead(AVl11); 
		HVl12 = VlRead(AVl12);
		HVl13 = VlRead(AVl13);
		HVl14 = VlRead(AVl14);
		HVl15 = VlRead(AVl15);
		HVl16 = VlRead(AVl16);
		HVl21 = VlRead(AVl21);
		HVl22 = VlRead(AVl22);
		HVl23 = VlRead(AVl23);
		HVl24 = VlRead(AVl24);
		HVl25 = VlRead(AVl25);
		HVl26 = VlRead(AVl26);
		WLv= VlRead(AWL);
		digitalWrite(HWLvPin, HIGH);
		PeriodHWL = IntervalHWL + Period;
	}
}

/*---------------------------------------ПОЛИВ--------------------------------------------------*/

#define VlPPin1 27
#define VlPPin2 28
#define VlPPin3 29
#define VlPPin4 30
#define VlPPin5 31
//интервал запроса

unsigned long PeriodPoliv;
unsigned long IntervalPoliv = 60000;


//Переменные 
int VlP11, VlP12, VlP13, VlP14, VlP15, VlP16;     //Влажность полива отсек1
int VlP21, VlP22, VlP23, VlP24, VlP25, VlP26;     //Влажность полива отсек2
int VlPTime;									  //Время включения помпы

//Рабочие переменные

bool VlPAll;																       //Выключатель полива общий
bool VlPSw11, VlPSw1, VlPSw12, VlPSw13, VlPSw14, VlPSw15, VlPSw16;						   //Выключатель полива по горшкам отсек1
bool VlPSw21, VlPSw2, VlPSw22, VlPSw23, VlPSw24, VlPSw25, VlPSw26;						   //Выключатель полива по горшкам отсек2




//Запуск
void PolivSetup() {
	pinMode(VlPPin1, OUTPUT);
	digitalWrite(VlPPin1, LOW);
	pinMode(VlPPin2, OUTPUT);
	digitalWrite(VlPPin2, LOW);
	pinMode(VlPPin3, OUTPUT);
	digitalWrite(VlPPin3, LOW);
	pinMode(VlPPin4, OUTPUT);
	digitalWrite(VlPPin4, LOW);
	pinMode(VlPPin5, OUTPUT);
	digitalWrite(VlPPin5, LOW);
}

//Цикл
void Poliv() {
	if (VlPAll && PeriodPoliv < Period && WLv>10) {
//Отсек 1
		if (VlPSw1 && VlPSw11 && HVl11 < VlP11) { digitalWrite(VlPPin1, HIGH); delay(VlPTime * 1000); digitalWrite(VlPPin1, LOW); } else { digitalWrite(VlPPin1, LOW);}
		if (VlPSw1 && VlPSw12 && HVl11 < VlP12) { digitalWrite(VlPPin1, HIGH); delay(VlPTime * 1000); digitalWrite(VlPPin1, LOW); } else { digitalWrite(VlPPin1, LOW);}
		if (VlPSw1 && VlPSw13 && HVl11 < VlP13) { digitalWrite(VlPPin1, HIGH); delay(VlPTime * 1000); digitalWrite(VlPPin1, LOW); } else { digitalWrite(VlPPin1, LOW);}
		if (VlPSw1 && VlPSw14 && HVl11 < VlP14) { digitalWrite(VlPPin1, HIGH); delay(VlPTime * 1000); digitalWrite(VlPPin1, LOW); } else { digitalWrite(VlPPin1, LOW);}
		if (VlPSw1 && VlPSw15 && HVl11 < VlP15) { digitalWrite(VlPPin1, HIGH); delay(VlPTime * 1000); digitalWrite(VlPPin1, LOW); } else { digitalWrite(VlPPin1, LOW);}
		if (VlPSw1 && VlPSw16 && HVl11 < VlP16) { digitalWrite(VlPPin1, HIGH); delay(VlPTime * 1000); digitalWrite(VlPPin1, LOW); } else { digitalWrite(VlPPin1, LOW);}
//Отсек 2
		if (VlPSw2 && VlPSw21 && HVl21 < VlP11) { digitalWrite(VlPPin2, HIGH); delay(VlPTime * 1000); digitalWrite(VlPPin2, LOW); } else { digitalWrite(VlPPin2, LOW);}
		if (VlPSw2 && VlPSw22 && HVl22 < VlP11) { digitalWrite(VlPPin2, HIGH); delay(VlPTime * 1000); digitalWrite(VlPPin2, LOW); } else { digitalWrite(VlPPin2, LOW);}
		if (VlPSw2 && VlPSw23 && HVl23 < VlP11) { digitalWrite(VlPPin2, HIGH); delay(VlPTime * 1000); digitalWrite(VlPPin2, LOW); } else { digitalWrite(VlPPin2, LOW);}
		if (VlPSw2 && VlPSw24 && HVl24 < VlP11) { digitalWrite(VlPPin2, HIGH); delay(VlPTime * 1000); digitalWrite(VlPPin2, LOW); } else { digitalWrite(VlPPin2, LOW);}
		if (VlPSw2 && VlPSw25 && HVl25 < VlP11) { digitalWrite(VlPPin2, HIGH); delay(VlPTime * 1000); digitalWrite(VlPPin2, LOW); } else { digitalWrite(VlPPin2, LOW);}
		if (VlPSw2 && VlPSw26 && HVl26 < VlP11) { digitalWrite(VlPPin2, HIGH); delay(VlPTime * 1000); digitalWrite(VlPPin2, LOW); } else { digitalWrite(VlPPin2, LOW);}
		PeriodPoliv = IntervalPoliv + Period;
	}
}

/*-------------------------------------------ОСВЕЩЕНИЕ-------------------------------------------------*/

//Библиотека и пины
#define Light11 7        //Пин освещения отсек 1-1
#define Light12 8        //Пин освещения отсек 1-2
#define Light21 9        //Пин освещения отсек 2-1
#define Light22 10       //Пин освещения отсек 2-2
#define Light3 11        //Пин освещения отсек 3
#define Light4 12        //Пин освещения отсек 4
#define Light5 13        //Пин освещения отсек 5

//интервал запроса
unsigned long PeriodLight;
unsigned long IntervalLight = 3000;

//Переменные запроса
int ONhh11, ONmm11, ONhh12, ONmm12, ONhh21, ONmm21, ONhh22, ONmm22, ONhh3, ONmm3, ONhh4, ONmm4, ONhh5, ONmm5;
int OFFhh11, OFFmm11, OFFhh12, OFFmm12, OFFhh21, OFFmm21, OFFhh22, OFFmm22, OFFhh3, OFFmm3, OFFhh4, OFFmm4, OFFhh5, OFFmm5;
int LightON11, LightON12, LightON21, LightON22, LightON3, LightON4, LightON5;
int LightOFF11, LightOFF12, LightOFF21, LightOFF22, LightOFF3, LightOFF4, LightOFF5;

//Рабочие переменные
bool LtSwAll, LtSw11, LtSw12, LtSw21, LtSw22, LtSw3, LtSw4, LtSw5;

bool LtOn11, LtOn12, LtOn21, LtOn22, LtOn3, LtOn4, LtOn5;
bool LtUpd=false;
int LightTime;

//Изменение
int TimeMinutes(int rhh, int rmm) {
	int Minutes = ((rhh*60) + rmm);
	return Minutes;
}

//Установка 
void LightUpdate() {
	LightON11= TimeMinutes(ONhh11, ONmm11);
	LightON12 = TimeMinutes(ONhh12, ONmm12);
	LightON21 = TimeMinutes(ONhh21, ONmm21);
	LightON22 = TimeMinutes(ONhh22, ONmm22);
	LightON3 = TimeMinutes(ONhh3, ONmm3);
	LightON4 = TimeMinutes(ONhh4, ONmm4);
	LightON5 = TimeMinutes(ONhh5, ONmm5);
	LightOFF11 = TimeMinutes(OFFhh11, OFFmm11);
	LightOFF12 = TimeMinutes(OFFhh12, OFFmm12);
	LightOFF21 = TimeMinutes(OFFhh21, OFFmm21);
	LightOFF22 = TimeMinutes(OFFhh22, OFFmm22);
	LightOFF3 = TimeMinutes(OFFhh3, OFFmm3);
	LightOFF4 = TimeMinutes(OFFhh4, OFFmm4);
	LightOFF5 = TimeMinutes(OFFhh5, OFFmm5);
}

//Запуск
void LightSetup() {
	pinMode(Light11, OUTPUT);
	pinMode(Light12, OUTPUT);
	pinMode(Light21, OUTPUT);
	pinMode(Light22, OUTPUT);
	pinMode(Light3, OUTPUT);
	pinMode(Light4, OUTPUT);
	pinMode(Light5, OUTPUT);
	digitalWrite(Light11, LOW);
	digitalWrite(Light12, LOW);
	digitalWrite(Light21, LOW);
	digitalWrite(Light22, LOW);
	digitalWrite(Light3, LOW);
	digitalWrite(Light4, LOW);
	digitalWrite(Light5, LOW);
	LightUpdate();
}

//Цикл
void Light() {
	if (PeriodLight < Period) {
		if (LtUpd){LightUpdate(); LtUpd = false;}
		LightTime = TimeMinutes(hh, mm);
		if (LtSwAll && LtSw11 && LightTime >= LightON11 && LightTime <= LightOFF11) { digitalWrite(Light11, HIGH); LtOn11 = true; }
		else { digitalWrite(Light11, LOW); LtOn11 = false;}
		if (LtSwAll && LtSw12 && LightTime >= LightON12 && LightTime <= LightOFF12) { digitalWrite(Light12, HIGH); LtOn12 = true; }
		else { digitalWrite(Light12, LOW); LtOn12 = false;}
		if (LtSwAll && LtSw21 && LightTime >= LightON21 && LightTime <= LightOFF21) { digitalWrite(Light21, HIGH); LtOn21 = true;}
		else { digitalWrite(Light21, LOW); LtOn21 = false;}
		if (LtSwAll && LtSw22 && LightTime >= LightON22 && LightTime <= LightOFF22) { digitalWrite(Light22, HIGH); LtOn22 = true;}
		else { digitalWrite(Light22, LOW); LtOn22 = false;}
		if (LtSwAll && LtSw3 && LightTime >= LightON3 && LightTime <= LightOFF3) { digitalWrite(Light3, HIGH); LtOn3 = true;}
		else { digitalWrite(Light3, LOW); LtOn3 = false;}
		if (LtSwAll && LtSw4 && LightTime >= LightON4 && LightTime <= LightOFF4) { digitalWrite(Light4, HIGH); LtOn4 = true;}
		else { digitalWrite(Light4, LOW); LtOn4 = false;}
		if (LtSwAll && LtSw5 && LightTime >= LightON5 && LightTime <= LightOFF5) { digitalWrite(Light5, HIGH); LtOn5 = true;}
		else { digitalWrite(Light5, LOW); LtOn5 = false;}
		PeriodLight = IntervalLight + Period;
	}
}

/*--------------------------------------------Переменные из памяти-------------------------------------------------*/
/*
/Часы
+unsigned long IntervalClocks

//Температура снаружи
+unsigned long IntervalBMP
+bool BMPON

//Температура в отсеках
+unsigned long IntervalDHT
+bool DHTON

//Обогрев
+unsigned long IntervalHeat
+int Heat1, Heat2, Heat3, Heat4, Heat5;     //Температура включения
+int HeatGyst;                              //Гистерезис
+bool HeatAll;																   //Выключатель обогрева общий
+bool HeatSw1, HeatSw2, HeatSw3, HeatSw4, HeatSw5;							   //Выключатель обогрева по отсекам

//Влажность почвы
+unsigned long IntervalHWL
bool HWLvSw

//Полив
+unsigned long IntervalPoliv

//Переменные
+int VlP11, VlP12, VlP13, VlP14, VlP15, VlP16;     //Влажность полива отсек1
+int VlP21, VlP22, VlP23, VlP24, VlP25, VlP26;     //Влажность полива отсек2
+int VlPTime;									  //Время включения помпы

//Рабочие переменные
+bool VlPAll;																       //Выключатель полива общий
+bool VlPSw11, VlPSw12, VlPSw13, VlPSw14, VlPSw15, VlPSw16;						   //Выключатель полива по горшкам отсек1
+bool VlPSw21, VlPSw22, VlPSw23, VlPSw24, VlPSw25, VlPSw26;						   //Выключатель полива по горшкам отсек2


//Освещение
+unsigned long IntervalLight

//Переменные запроса
int ONhh11, ONmm11, ONhh12, ONmm12, ONhh21, ONmm21, ONhh22, ONmm22, ONhh3, ONmm3, ONhh4, ONmm4, ONhh5, ONmm5;
int OFFhh11, OFFmm11, OFFhh12, OFFmm12, OFFhh21, OFFmm21, OFFhh22, OFFmm22, OFFhh3, OFFmm3, OFFhh4, OFFmm4, OFFhh5, OFFmm5;

//Рабочие переменные
bool LtSw11, LtSw12, LtSw21, LtSw22, LtSw3, LtSw4, LtSw5;
bool LtUpd=false;
*/

/*--------------------------------------------ЗАПИСЬ/ЧТЕНИЕ ПЕРЕМЕННЫХ В EEPROM-------------------------------------------------*/
//библиотеки
#include <EEPROM.h>
unsigned long ValEEPWR;
unsigned long ValEEPRD;

//Конвертации
int ROMWRITEUNS(unsigned long ValEEP) {
	int Value;
	Value = ValEEP / 1000;
	return Value;
}

unsigned long ROMREADUNS(int ValEEP) {
	unsigned long Value;
	Value = ValEEP;
	Value = Value * 1000;
	return Value;
}

//Чтение с конвертацией
int ROMREAD(int Num) {
	byte Val = EEPROM.read(Num);
	int Value = int(Val);
	return Value;
}

//Запись с конвертацией
void ROMWRITE(int Num, int Value) {
	byte Val = byte(Value);
	EEPROM.update(Num, Val);
}


//По умолчанию
void ROMDEFAULT() {
	IntervalClocks = 5000;
	ROMWRITE(1, ROMWRITEUNS(IntervalClocks));
	IntervalBMP = 7000;
	ROMWRITE(2, ROMWRITEUNS(IntervalBMP));
	IntervalDHT = 11000;
	ROMWRITE(3, ROMWRITEUNS(IntervalDHT));
	IntervalHeat = 13000;
	ROMWRITE(4, ROMWRITEUNS(IntervalHeat));
	IntervalHWL = 60000;
	ROMWRITE(5, ROMWRITEUNS(IntervalHWL));
	IntervalPoliv = 60000;
	ROMWRITE(6, ROMWRITEUNS(IntervalPoliv));
	IntervalLight = 3000;
	ROMWRITE(7, ROMWRITEUNS(IntervalLight));
	BMPON = true;
	ROMWRITE(8, int(BMPON));
	DHTON = true;
	ROMWRITE(9, int(DHTON));
	Heat1 = 18;
	ROMWRITE(10, Heat1);
	Heat2 = 18;
	ROMWRITE(11, Heat2);
	Heat3 = 18;
	ROMWRITE(12, Heat3);
	Heat4 = 18;
	ROMWRITE(13, Heat4);
	Heat5 = 18;
	ROMWRITE(14, Heat5);
	HeatGyst = 2;
	ROMWRITE(15, HeatGyst);
	HeatAll = true;
	ROMWRITE(16, int(HeatAll));
	HeatSw1 = true;
	ROMWRITE(17, int(HeatSw1));
	HeatSw2 = true;
	ROMWRITE(18, int(HeatSw2));
	HeatSw3 = false;
	ROMWRITE(19, int(HeatSw3));
	HeatSw4 = false;
	ROMWRITE(20, int(HeatSw4));
	HeatSw5 = false;
	ROMWRITE(21, int(HeatSw5));
	HWLvSw = true;
	ROMWRITE(22, int(HWLvSw));
	VlP11=30;
	ROMWRITE(23, Heat1);
	VlP12 = 30;
	ROMWRITE(24, Heat1);
	VlP13 = 30;
	ROMWRITE(25, Heat1);
	VlP14 = 30;
	ROMWRITE(26, Heat1);
	VlP15 = 30;
	ROMWRITE(27, Heat1);
	VlP16 = 30;
	ROMWRITE(28, Heat1);
	VlP21 = 30;
	ROMWRITE(29, Heat1);
	VlP22 = 30;
	ROMWRITE(30, Heat1);
	VlP23 = 30;
	ROMWRITE(31, Heat1);
	VlP24 = 30;
	ROMWRITE(32, Heat1);
	VlP25 = 30;
	ROMWRITE(33, Heat1);
	VlP26 = 30;
	ROMWRITE(34, Heat1);
	VlPTime=5;
	ROMWRITE(35, Heat1);
	VlPAll=false;
	ROMWRITE(36, int(VlPAll));
	VlPSw11 = false;
	ROMWRITE(37, int(VlPSw11));
	VlPSw12 = false;
	ROMWRITE(38, int(VlPSw12));
	VlPSw13 = false;
	ROMWRITE(39, int(VlPSw13));
	VlPSw14 = false;
	ROMWRITE(40, int(VlPSw14));
	VlPSw15 = false;
	ROMWRITE(41, int(VlPSw15));
	VlPSw16 = false;
	ROMWRITE(42, int(VlPSw16));
	VlPSw21 = false;
	ROMWRITE(43, int(VlPSw21));
	VlPSw22 = false;
	ROMWRITE(44, int(VlPSw22));
	VlPSw23 = false;
	ROMWRITE(45, int(VlPSw23));
	VlPSw24 = false;
	ROMWRITE(46, int(VlPSw24));
	VlPSw25 = false;
	ROMWRITE(47, int(VlPSw25));
	VlPSw26 = false;
	ROMWRITE(48, int(VlPSw26));
	ONhh11 = 6;
	ROMWRITE(49, ONhh11);
	ONmm11 = 0;
	ROMWRITE(50, ONmm11);
	ONhh12 = 6;
	ROMWRITE(51, ONhh12);
	ONmm12 = 0;
	ROMWRITE(52, ONmm12);
	ONhh21 = 6;
	ROMWRITE(53, ONhh21);
	ONmm21 = 0;
	ROMWRITE(54, ONmm21);
	ONhh22 = 6;
	ROMWRITE(55, ONhh22);
	ONmm22 = 0;
	ROMWRITE(56, ONmm22);
	ONhh3 = 6;
	ROMWRITE(57, ONhh3);
	ONmm3 = 0;
	ROMWRITE(58, ONmm3);
	ONhh4 = 6;
	ROMWRITE(59, ONhh4);
	ONmm4 = 0;
	ROMWRITE(60, ONmm4);
	ONhh5 = 6;
	ROMWRITE(61, ONhh5);
	ONmm5 = 0;
	ROMWRITE(62, ONmm5);
	OFFhh11 = 21;
	ROMWRITE(63, OFFhh11);
	OFFmm11 = 0;
	ROMWRITE(64, OFFmm11);
	OFFhh12 = 21;
	ROMWRITE(65, OFFhh12);
	OFFmm12 = 0;
	ROMWRITE(66, OFFmm12);
	OFFhh21 = 21;
	ROMWRITE(67, OFFhh21);
	OFFmm21 = 0;
	ROMWRITE(68, OFFmm21);
	OFFhh22 = 21;
	ROMWRITE(69, OFFhh22);
	OFFmm22 = 0;
	ROMWRITE(70, OFFmm22);
	OFFhh3 = 21;
	ROMWRITE(71, OFFhh3);
	OFFmm3 = 0;
	ROMWRITE(72, OFFmm3);
	OFFhh4 = 21;
	ROMWRITE(73, OFFhh4);
	OFFmm4 = 0;
	ROMWRITE(74, OFFmm4);
	OFFhh5 = 21;
	ROMWRITE(75, OFFhh5);
	OFFmm5 = 0;
	ROMWRITE(76, OFFmm5);
	LtSw11 = true;
	ROMWRITE(77, int(LtSw11));
	LtSw12 = true;
	ROMWRITE(78, int(LtSw12));
	LtSw21 = true;
	ROMWRITE(79, int(LtSw21));
	LtSw22 = true;
	ROMWRITE(80, int(LtSw22));
	LtSw3 = true;
	ROMWRITE(81, int(LtSw3));
	LtSw4 = true;
	ROMWRITE(82, int(LtSw4));
	LtSw5 = true;
	ROMWRITE(83, int(LtSw5));
	LtSwAll = true;
	ROMWRITE(84, int(LtSwAll));
	WFSw = true;
	ROMWRITE(85, int(WFSw));
	BthSw = true;
	ROMWRITE(86, int(BthSw));
	VlPSw1 = true;
	ROMWRITE(87, int(VlPSw1));
	VlPSw2 = true;
	ROMWRITE(88, int(VlPSw2));
}

String InputName;               //Имя изменяемой переменной
String InputVar;                //Значение переменной

//Конвертация Буль в Стринг
String BoolToString(bool Input) {
	String Output;
	if (Input == true) {
		Output = "ON";
		return Output;
	}
	else {
		Output = "OFF";
		return Output;
	}
}


//Конвертация Стринг в Буль
int StringConvert(String Input) {
	bool Output;
	int OutW;
	if (Input == "ON" || Input == "On" || Input == "on") {
		Output = true;
		return Output;
	}
	else if (Input == "OFF" || Input == "Off" || Input == "off"){
		Output = false;
		return Output;
	}
	else {
		OutW = Input.toInt();
		return OutW;
	}
}



void VarWrite(String InputName, String InputVar) {
	if (InputName == "IntervalClocks") {
		IntervalClocks = StringConvert(InputVar);
		ROMWRITE(1, ROMWRITEUNS(IntervalClocks));
	}
	else if (InputName == "IntervalBMP") {
		IntervalBMP = StringConvert(InputVar) * 1000;
		ROMWRITE(2, ROMWRITEUNS(IntervalBMP));
	}
	else if (InputName == "IntervalDHT") {
		IntervalDHT = StringConvert(InputVar) * 1000;
		ROMWRITE(3, ROMWRITEUNS(IntervalDHT));
	}
	else if (InputName == "IntervalHeat") {
		IntervalHeat = StringConvert(InputVar) * 1000;
		ROMWRITE(4, ROMWRITEUNS(IntervalHeat));
	}
	else if (InputName == "IntervalHWL") {
		IntervalHWL = StringConvert(InputVar) * 1000;
		ROMWRITE(5, ROMWRITEUNS(IntervalHWL));
	}
	else if (InputName == "IntervalPoliv") {
		IntervalPoliv = StringConvert(InputVar) * 1000;
		ROMWRITE(6, ROMWRITEUNS(IntervalPoliv));
	}
	else if (InputName == "IntervalLight") {
		IntervalLight = StringConvert(InputVar) * 1000;
		ROMWRITE(7, ROMWRITEUNS(IntervalLight));
	}
	else if (InputName == "BMPON") {
		BMPON = StringConvert(InputVar);
		ROMWRITE(8, int(BMPON));
	}
	else if (InputName == "DHTON") {
		DHTON = StringConvert(InputVar);
		ROMWRITE(9, int(DHTON));
	}
	else if (InputName == "Heat1") {
		Heat1 = StringConvert(InputVar);
		ROMWRITE(10, Heat1);
	}
	else if (InputName == "Heat2") {
		Heat2 = StringConvert(InputVar);
		ROMWRITE(11, Heat2);
	}
	else if (InputName == "Heat3") {
		Heat3 = StringConvert(InputVar);
		ROMWRITE(12, Heat3);
	}
	else if (InputName == "Heat4") {
		Heat4 = StringConvert(InputVar);
		ROMWRITE(13, Heat4);
	}
	else if (InputName == "Heat5") {
		Heat5 = StringConvert(InputVar);
		ROMWRITE(14, Heat5);
	}
	else if (InputName == "HeatGyst") {
		HeatGyst = StringConvert(InputVar);
		ROMWRITE(15, HeatGyst);
	}
	else if (InputName == "HeatAll") {
		HeatAll = StringConvert(InputVar);
		ROMWRITE(16, int(HeatAll));
	}
	else if (InputName == "HeatSw1") {
		HeatSw1 = StringConvert(InputVar);
		ROMWRITE(17, int(HeatSw1));
	}
	else if (InputName == "HeatSw2") {
		HeatSw2 = StringConvert(InputVar);
		ROMWRITE(18, int(HeatSw2));
	}
	else if (InputName == "HeatSw3") {
		HeatSw3 = StringConvert(InputVar);
		ROMWRITE(19, int(HeatSw3));
	}
	else if (InputName == "HeatSw4") {
		HeatSw4 = StringConvert(InputVar);
		ROMWRITE(20, int(HeatSw4));
	}
	else if (InputName == "HeatSw5") {
		HeatSw5 = StringConvert(InputVar);
		ROMWRITE(21, int(HeatSw5));
	}
	else if (InputName == "HWLvSw") {
		HWLvSw = StringConvert(InputVar);
		ROMWRITE(22, int(HWLvSw));
	}
	else if (InputName == "VlP11") {
		VlP11 = StringConvert(InputVar);
		ROMWRITE(23, Heat1);
	}
	else if (InputName == "VlP12") {
		VlP12 = StringConvert(InputVar);
		ROMWRITE(24, Heat1);
	}
	else if (InputName == "VlP13") {
		VlP13 = StringConvert(InputVar);
		ROMWRITE(25, Heat1);
	}
	else if (InputName == "VlP14") {
		VlP14 = StringConvert(InputVar);
		ROMWRITE(26, Heat1);
	}
	else if (InputName == "VlP15") {
		VlP15 = StringConvert(InputVar);
		ROMWRITE(27, Heat1);
	}
	else if (InputName == "VlP16") {
		VlP16 = StringConvert(InputVar);
		ROMWRITE(28, Heat1);
	}
	else if (InputName == "VlP21") {
		VlP21 = StringConvert(InputVar);
		ROMWRITE(29, Heat1);
	}
	else if (InputName == "VlP22") {
		VlP22 = StringConvert(InputVar);
		ROMWRITE(30, Heat1);
	}
	else if (InputName == "VlP23") {
		VlP23 = StringConvert(InputVar);
		ROMWRITE(31, Heat1);
	}
	else if (InputName == "VlP24") {
		VlP24 = StringConvert(InputVar);
		ROMWRITE(32, Heat1);
	}
	else if (InputName == "VlP25") {
		VlP25 = StringConvert(InputVar);
		ROMWRITE(33, Heat1);
	}
	else if (InputName == "VlP26") {
		VlP26 = StringConvert(InputVar);
		ROMWRITE(34, Heat1);
	}
	else if (InputName == "VlPTime") {
		VlPTime = StringConvert(InputVar);
		ROMWRITE(35, Heat1);
	}
	else if (InputName == "VlPAll") {
		VlPAll = StringConvert(InputVar);
		ROMWRITE(36, int(VlPAll));
	}
	else if (InputName == "VlPSw11") {
		VlPSw11 = StringConvert(InputVar);
		ROMWRITE(37, int(VlPSw11));
	}
	else if (InputName == "VlPSw12") {
		VlPSw12 = StringConvert(InputVar);
		ROMWRITE(38, int(VlPSw12));
	}
	else if (InputName == "VlPSw13") {
		VlPSw13 = StringConvert(InputVar);
		ROMWRITE(39, int(VlPSw13));
	}
	else if (InputName == "VlPSw14") {
		VlPSw14 = StringConvert(InputVar);
		ROMWRITE(40, int(VlPSw14));
	}
	else if (InputName == "VlPSw15") {
		VlPSw15 = StringConvert(InputVar);
		ROMWRITE(41, int(VlPSw15));
	}
	else if (InputName == "VlPSw16") {
		VlPSw16 = StringConvert(InputVar);
		ROMWRITE(42, int(VlPSw16));
	}
	else if (InputName == "VlPSw21") {
		VlPSw21 = StringConvert(InputVar);
		ROMWRITE(43, int(VlPSw21));
	}
	else if (InputName == "VlPSw22") {
		VlPSw22 = StringConvert(InputVar);
		ROMWRITE(44, int(VlPSw22));
	}
	else if (InputName == "VlPSw23") {
		VlPSw23 = StringConvert(InputVar);
		ROMWRITE(45, int(VlPSw23));
	}
	else if (InputName == "VlPSw24") {
		VlPSw24 = StringConvert(InputVar);
		ROMWRITE(46, int(VlPSw24));
	}
	else if (InputName == "VlPSw25") {
		VlPSw25 = StringConvert(InputVar);
		ROMWRITE(47, int(VlPSw25));
	}
	else if (InputName == "VlPSw26") {
		VlPSw26 = StringConvert(InputVar);
		ROMWRITE(48, int(VlPSw26));
	}
	else if (InputName == "ONhh11") {
		ONhh11 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(49, ONhh11);
	}
	else if (InputName == "ONmm11") {
		ONmm11 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(50, ONmm11);
	}
	else if (InputName == "ONhh12") {
		ONhh12 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(51, ONhh12);
	}
	else if (InputName == "ONmm12") {
		ONmm12 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(52, ONmm12);
	}
	else if (InputName == "ONhh21") {
		ONhh21 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(53, ONhh21);
	}
	else if (InputName == "ONmm21") {
		ONmm21 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(54, ONmm21);
	}
	else if (InputName == "ONhh22") {
		ONhh22 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(55, ONhh22);
	}
	else if (InputName == "ONmm22") {
		ONmm22 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(56, ONmm22);
	}
	else if (InputName == "ONhh3") {
		ONhh3 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(57, ONhh3);
	}
	else if (InputName == "ONmm3") {
		ONmm3 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(58, ONmm3);
	}
	else if (InputName == "ONhh4") {
		ONhh4 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(59, ONhh4);
	}
	else if (InputName == "ONmm4") {
		ONmm4 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(60, ONmm4);
	}
	else if (InputName == "ONhh5") {
		ONhh5 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(61, ONhh5);
	}
	else if (InputName == "ONmm5") {
		ONmm5 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(62, ONmm5);
	}
	else if (InputName == "OFFhh11") {
		OFFhh11 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(63, OFFhh11);
	}
	else if (InputName == "OFFmm11") {
		OFFmm11 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(64, OFFmm11);
	}
	else if (InputName == "OFFhh12") {
		OFFhh12 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(65, OFFhh12);
	}
	else if (InputName == "OFFmm12") {
		OFFmm12 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(66, OFFmm12);
	}
	else if (InputName == "OFFhh21") {
		OFFhh21 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(67, OFFhh21);
	}
	else if (InputName == "OFFmm21") {
		OFFmm21 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(68, OFFmm21);
	}
	else if (InputName == "OFFhh22") {
		OFFhh22 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(69, OFFhh22);
	}
	else if (InputName == "OFFmm22") {
		OFFmm22 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(70, OFFmm22);
	}
	else if (InputName == "OFFhh3") {
		OFFhh3 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(71, OFFhh3);
	}
	else if (InputName == "OFFmm3") {
		OFFmm3 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(72, OFFmm3);
	}
	else if (InputName == "OFFhh4") {
		OFFhh4 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(73, OFFhh4);
	}
	else if (InputName == "OFFmm4") {
		OFFmm4 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(74, OFFmm4);
	}
	else if (InputName == "OFFhh5") {
		OFFhh5 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(75, OFFhh5);
	}
	else if (InputName == "OFFmm5") {
		OFFmm5 = StringConvert(InputVar);
		LtUpd = true;
		ROMWRITE(76, OFFmm5);
	}
	else if (InputName == "LtSw11") {
		LtSw11 = StringConvert(InputVar);
		ROMWRITE(77, int(LtSw11));
	}
	else if (InputName == "LtSw12") {
		LtSw12 = StringConvert(InputVar);
		ROMWRITE(78, int(LtSw12));
	}
	else if (InputName == "LtSw21") {
		LtSw21 = StringConvert(InputVar);
		ROMWRITE(79, int(LtSw21));
	}
	else if (InputName == "LtSw22") {
		LtSw22 = StringConvert(InputVar);
		ROMWRITE(80, int(LtSw22));
	}
	else if (InputName == "LtSw3") {
		LtSw3 = StringConvert(InputVar);
		ROMWRITE(81, int(LtSw3));
	}
	else if (InputName == "LtSw4") {
		LtSw4 = StringConvert(InputVar);
		ROMWRITE(82, int(LtSw4));
	}
	else if (InputName == "LtSw5") {
		LtSw5 = StringConvert(InputVar);
		ROMWRITE(83, int(LtSw5));
	}
	else if (InputName == "LtSwAll") {
		LtSwAll = StringConvert(InputVar);
		ROMWRITE(84, int(LtSwAll));
	}
	else if (InputName == "WFSw") {
		WFSw = StringConvert(InputVar);
		ROMWRITE(85, int(WFSw));
	}
	else if (InputName == "BthSw") {
		BthSw = StringConvert(InputVar);
		ROMWRITE(86, int(BthSw));
	}
	else if (InputName == "VlPSw1") {
		VlPSw1 = StringConvert(InputVar);
		ROMWRITE(87, int(VlPSw1));
	}
	else if (InputName == "VlPSw2") {
		VlPSw2 = StringConvert(InputVar);
		ROMWRITE(88, int(VlPSw2));
	}
	else if (InputName == "SetYY") {
		SetYY= StringConvert(InputVar);
	}
	else if (InputName == "SetW") {
		SetW = StringConvert(InputVar);
	}
	else if (InputName == "SetMM") {
		SetMM = StringConvert(InputVar);
	}
	else if (InputName == "SetDD") {
		SetDD = StringConvert(InputVar);
	}
	else if (InputName == "Sethh") {
		Sethh = StringConvert(InputVar);
	}
	else if (InputName == "Setmm") {
		Setmm = StringConvert(InputVar);
	}
	else if (InputName == "Setss") {
		Setss = StringConvert(InputVar);
		SetupTime = true;
	}
	else {
		return;
	}
}


//Загрузка
void ROMSETUP() {
	IntervalClocks = ROMREADUNS(ROMREAD(1));
	IntervalBMP = ROMREADUNS(ROMREAD(2));
	IntervalDHT = ROMREADUNS(ROMREAD(3));
	IntervalHeat = ROMREADUNS(ROMREAD(4));
	IntervalHWL = ROMREADUNS(ROMREAD(5));
	IntervalPoliv = ROMREADUNS(ROMREAD(6));
	IntervalLight = ROMREADUNS(ROMREAD(7));
	BMPON = bool(ROMREAD(8));
	DHTON = bool(ROMREAD(9));
	Heat1 = ROMREAD(10);
	Heat2 = ROMREAD(11);
	Heat3 = ROMREAD(12);
	Heat4 = ROMREAD(13);
	Heat5 = ROMREAD(14);
	HeatGyst = ROMREAD(15);
	HeatAll = bool(ROMREAD(16));
	HeatSw1 = bool(ROMREAD(17));
	HeatSw2 = bool(ROMREAD(18));
	HeatSw3 = bool(ROMREAD(19));
	HeatSw4 = bool(ROMREAD(20));
	HeatSw5 = bool(ROMREAD(21));
	HWLvSw = bool(ROMREAD(22));
	VlP11 = ROMREAD(23);
	VlP12 = ROMREAD(24);
	VlP13 = ROMREAD(25);
	VlP14 = ROMREAD(26);
	VlP15 = ROMREAD(27);
	VlP16 = ROMREAD(28);
	VlP21 = ROMREAD(29);
	VlP22 = ROMREAD(30);
	VlP23 = ROMREAD(31);
	VlP24 = ROMREAD(32);
	VlP25 = ROMREAD(33);
	VlP26 = ROMREAD(34);
	VlPTime = ROMREAD(35);
	VlPAll = bool(ROMREAD(36));
	VlPSw11 = bool(ROMREAD(37));
	VlPSw12 = bool(ROMREAD(38));
	VlPSw13 = bool(ROMREAD(39));
	VlPSw14 = bool(ROMREAD(40));
	VlPSw15 = bool(ROMREAD(41));
	VlPSw16 = bool(ROMREAD(42));
	VlPSw21 = bool(ROMREAD(43));
	VlPSw22 = bool(ROMREAD(44));
	VlPSw23 = bool(ROMREAD(45));
	VlPSw24 = bool(ROMREAD(46));
	VlPSw25 = bool(ROMREAD(47));
	VlPSw26 = bool(ROMREAD(48));
	ONhh11 = ROMREAD(49);
	ONmm11 = ROMREAD(50);
	ONhh12 = ROMREAD(51);
	ONmm12 = ROMREAD(52);
	ONhh21 = ROMREAD(53);
	ONmm21 = ROMREAD(54);
	ONhh22 = ROMREAD(55);
	ONmm22 = ROMREAD(56);
	ONhh3 = ROMREAD(57);
	ONmm3 = ROMREAD(58);
	ONhh4 = ROMREAD(59);
	ONmm4 = ROMREAD(60);
	ONhh5 = ROMREAD(61);
	ONmm5 = ROMREAD(62);
	OFFhh11 = ROMREAD(63);
	OFFmm11 = ROMREAD(64);
	OFFhh12 = ROMREAD(65);
	OFFmm12 = ROMREAD(66);
	OFFhh21 = ROMREAD(67);
	OFFmm21 = ROMREAD(68);
	OFFhh22 = ROMREAD(69);
	OFFmm22 = ROMREAD(70);
	OFFhh3 = ROMREAD(71);
	OFFmm3 = ROMREAD(72);
	OFFhh4 = ROMREAD(73);
	OFFmm4 = ROMREAD(74);
	OFFhh5 = ROMREAD(75);
	OFFmm5 = ROMREAD(76);
	LtSw11 = bool(ROMREAD(77));
	LtSw12 = bool(ROMREAD(78));
	LtSw21 = bool(ROMREAD(79));
	LtSw22 = bool(ROMREAD(80));
	LtSw3 = bool(ROMREAD(81));
	LtSw4 = bool(ROMREAD(82));
	LtSw5 = bool(ROMREAD(83));
	LtSwAll = bool(ROMREAD(84));
	BthSw = bool(ROMREAD(85));
	WFSw = bool(ROMREAD(86));
	VlPSw1= bool(ROMREAD(87));
	VlPSw2 = bool(ROMREAD(88));
}

//*************************************Модуль обработки команд дисплея********************************************************//
//Функция посылки значения экрану
void NexSend(String Var, String Name) {
	String Msg = "P:";
	Serial1.print(Msg + Name + '.' + Var + ';');
	//Serial.println(Name + "." + Type + "=\"" + Var + "\"" + char(255) + char(255) + char(255));
}

//Запуск Портов
void NextionSetup() {
	Serial1.begin(115200);
}

//Чтение портов
String NexInputName;               //Имя изменяемой переменной
String NexInputvar;                //Значение переменной
int NexPage = 1;				   //Номер экрана

								   

//Расшифровка входящих


//Прием Команды
void NextionSerial() {
	String input;
	if (Serial1.available() > 0) {
		NexInputName = "";
		NexInputvar = "";
		while (Serial1.available() > 0) {
			char inputchar;
			inputchar = Serial1.read();
			if (inputchar == '.') {
				NexInputName = input;
				//Serial.print(NexInputName);
				//Serial.print('.');
				input = "";
				delay(20);
			}
			else if (inputchar == ';') {
				NexInputvar = input;
				input = "";
				//Serial.print(NexInputvar);
				//Serial.println(';');
				delay(20);
			}
			else {
				input += inputchar;
				delay(50);
			}
		}
		if (NexInputName == "Page") {
			NexPage = NexInputvar.toInt();
			switch (NexPage)
			{
			case 1:
				NexSend(String(YY), "YY");
				NexSend(String(MM), "MM");
				NexSend(String(DD), "DD");
				NexSend(String(hh), "hh");
				NexSend(String(mm), "mm");
				NexSend(String(BMPTemper), "BMPTemper");
				NexSend(String(BMPPress), "BMPPress");
				NexSend(String(BMPAlt), "BMPAlt");
				NexSend(BoolToString(HeatAll), "HeatAll");
				NexSend(String(WLv), "WLv");
				NexSend(BoolToString(VlPAll), "VlPAll");
				NexSend(BoolToString(LtSwAll), "LtSwAll");
				NexSend(BoolToString(BthSw), "BthSw");
				NexSend(BoolToString(WFSw), "WFSw");
				break;
			case 2:
				break;
			case 3:
				NexSend(String(dhtT1), "dhtT1");
				NexSend(String(dhtH1), "dhtH1");
				NexSend(String(Heat1), "Heat1");
				NexSend(BoolToString(HeatSw1), "HeatSw1");
				NexSend(BoolToString(HeatOn1), "HeatOn1");
				NexSend(String(HVl11), "HVl11");
				NexSend(String(HVl12), "HVl12");
				NexSend(String(HVl13), "HVl13");
				NexSend(String(HVl14), "HVl14");
				NexSend(String(HVl15), "HVl15");
				NexSend(String(HVl16), "HVl16");
				NexSend(BoolToString(VlPSw1), "VlPSw1");
				NexSend(String(ONhh11), "ONhh11");
				NexSend(String(ONmm11), "ONmm11");
				NexSend(String(ONhh12), "ONhh12");
				NexSend(String(ONmm12), "ONmm12");
				NexSend(String(OFFhh11), "OFFhh11");
				NexSend(String(OFFmm11), "OFFmm11");
				NexSend(String(OFFhh12), "OFFhh12");
				NexSend(String(OFFmm12), "OFFmm12");
				NexSend(BoolToString(LtSw11), "LtSw11");
				NexSend(BoolToString(LtSw12), "LtSw12");
				NexSend(BoolToString(LtOn11), "LtOn11");
				NexSend(BoolToString(LtOn12), "LtOn12");
				NexSend(BoolToString(VlPSw11), "VlPSw11");
				NexSend(BoolToString(VlPSw12), "VlPSw12");
				NexSend(BoolToString(VlPSw13), "VlPSw13");
				NexSend(BoolToString(VlPSw14), "VlPSw14");
				NexSend(BoolToString(VlPSw15), "VlPSw15");
				NexSend(BoolToString(VlPSw16), "VlPSw16");
				break;
			case 4:
				NexSend(String(dhtT2), "dhtT2");
				NexSend(String(dhtH2), "dhtH2");
				NexSend(String(Heat2), "Heat2");
				NexSend(BoolToString(HeatSw2), "HeatSw2");
				NexSend(BoolToString(HeatOn2), "HeatOn2");
				NexSend(String(HVl21), "HVl21");
				NexSend(String(HVl22), "HVl22");
				NexSend(String(HVl23), "HVl23");
				NexSend(String(HVl24), "HVl24");
				NexSend(String(HVl25), "HVl25");
				NexSend(String(HVl26), "HVl26");
				NexSend(BoolToString(VlPSw2), "VlPSw2");
				NexSend(String(ONhh21), "ONhh21");
				NexSend(String(ONmm21), "ONmm21");
				NexSend(String(ONhh22), "ONhh22");
				NexSend(String(ONmm22), "ONmm22");
				NexSend(String(OFFhh21), "OFFhh21");
				NexSend(String(OFFmm21), "OFFmm21");
				NexSend(String(OFFhh22), "OFFhh22");
				NexSend(String(OFFmm22), "OFFmm22");
				NexSend(BoolToString(LtSw21), "LtSw21");
				NexSend(BoolToString(LtSw22), "LtSw22");
				NexSend(BoolToString(LtOn21), "LtOn21");
				NexSend(BoolToString(LtOn22), "LtOn22");
				NexSend(BoolToString(VlPSw21), "VlPSw21");
				NexSend(BoolToString(VlPSw22), "VlPSw22");
				NexSend(BoolToString(VlPSw23), "VlPSw23");
				NexSend(BoolToString(VlPSw24), "VlPSw24");
				NexSend(BoolToString(VlPSw25), "VlPSw25");
				NexSend(BoolToString(VlPSw26), "VlPSw26");

				break;
			case 5:
				NexSend(String(dhtT3), "dhtT3");
				NexSend(String(dhtH3), "dhtH3");
				NexSend(String(Heat3), "Heat3");
				NexSend(BoolToString(HeatSw3), "HeatSw3");
				NexSend(BoolToString(HeatOn3), "HeatOn3");
				NexSend(String(ONhh3), "ONhh3");
				NexSend(String(ONmm3), "ONmm3");
				NexSend(String(OFFhh3), "OFFhh3");
				NexSend(String(OFFmm3), "OFFmm3");
				NexSend(BoolToString(LtSw3), "LtSw3");
				NexSend(BoolToString(LtOn3), "LtOn3");

				break;
			case 6:

				NexSend(String(dhtT4), "dhtT4");
				NexSend(String(dhtH4), "dhtH4");
				NexSend(String(Heat4), "Heat4");
				NexSend(BoolToString(HeatSw4), "HeatSw4");
				NexSend(BoolToString(HeatOn4), "HeatOn4");
				NexSend(String(ONhh4), "ONhh4");
				NexSend(String(ONmm4), "ONmm4");
				NexSend(String(OFFhh4), "OFFhh4");
				NexSend(String(OFFmm4), "OFFmm4");
				NexSend(BoolToString(LtSw4), "LtSw4");
				NexSend(BoolToString(LtOn4), "LtOn4");
				break;
			case 7:

				NexSend(String(dhtT5), "dhtT5");
				NexSend(String(dhtH5), "dhtH5");
				NexSend(String(Heat5), "Heat5");
				NexSend(BoolToString(HeatSw5), "HeatSw5");
				NexSend(BoolToString(HeatOn5), "HeatOn5");
				NexSend(String(ONhh5), "ONhh5");
				NexSend(String(ONmm5), "ONmm5");
				NexSend(String(OFFhh5), "OFFhh5");
				NexSend(String(OFFmm5), "OFFmm5");
				NexSend(BoolToString(LtSw5), "LtSw5");
				NexSend(BoolToString(LtOn5), "LtOn5");
				break;
			case 8:
				NexSend(String(IntervalClocks / 1000), "IntervalClocks");
				
				NexSend(String(IntervalBMP / 1000), "IntervalBMP");
				
				NexSend(BoolToString(BMPON), "BMPON");
				
				NexSend(String(IntervalDHT / 1000), "IntervalDHT");
				NexSend(BoolToString(DHTON), "DHTON");
				NexSend(String(IntervalHeat / 1000), "IntervalHeat");
				
				NexSend(String(HeatGyst), "HeatGyst");
				NexSend(BoolToString(HeatAll), "HeatAll");
				NexSend(String(IntervalPoliv / 1000), "IntervalPoliv");
				NexSend(String(VlPTime), "VlPTime");
				NexSend(BoolToString(VlPAll), "VlPAll");
				NexSend(String(IntervalLight / 1000), "IntervalLight");
				NexSend(BoolToString(LtSwAll), "LtSwAll");
				NexSend(BoolToString(BthSw), "BthSw");
				NexSend(BoolToString(WFSw), "WFSw");
				break;
			case 9:
				NexSend(String(Heat1), "Heat1");
				NexSend(BoolToString(HeatSw1), "HeatSw1");
				NexSend(BoolToString(LtSw11), "LtSw11");
				NexSend(String(ONhh11), "ONhh11");
				NexSend(String(ONmm11), "ONmm11");
				NexSend(String(OFFhh11), "OFFhh11");
				NexSend(String(OFFmm11), "OFFmm11");
				NexSend(BoolToString(LtSw12), "LtSw12");
				NexSend(String(ONhh12), "ONhh12");
				NexSend(String(ONmm12), "ONmm12");
				NexSend(String(OFFhh12), "OFFhh12");
				NexSend(String(OFFmm12), "OFFmm12");
				NexSend(String(VlP11), "VlP11");
				NexSend(String(VlP12), "VlP12");
				NexSend(String(VlP13), "VlP13");
				NexSend(String(VlP14), "VlP14");
				NexSend(String(VlP15), "VlP15");
				NexSend(String(VlP16), "VlP16");
				NexSend(BoolToString(VlPSw1), "VlPSw1");

				break;
			case 10:
				NexSend(String(Heat2), "Heat2");
				NexSend(BoolToString(HeatSw2), "HeatSw2");
				NexSend(BoolToString(LtSw21), "LtSw21");
				NexSend(String(ONhh21), "ONhh21");
				NexSend(String(ONmm21), "ONmm21");
				NexSend(String(OFFhh21), "OFFhh21");
				NexSend(String(OFFmm21), "OFFmm21");
				NexSend(BoolToString(LtSw22), "LtSw22");
				NexSend(String(ONhh22), "ONhh22");
				NexSend(String(ONmm22), "ONmm22");
				NexSend(String(OFFhh22), "OFFhh22");
				NexSend(String(OFFmm22), "OFFmm22");
				NexSend(String(VlP21), "VlP21");
				NexSend(String(VlP22), "VlP22");
				NexSend(String(VlP23), "VlP23");
				NexSend(String(VlP24), "VlP24");
				NexSend(String(VlP25), "VlP25");
				NexSend(String(VlP26), "VlP26");
				NexSend(BoolToString(VlPSw2), "VlPSw2");

				break;
			case 11:
				NexSend(String(Heat3), "Heat3");
				NexSend(BoolToString(HeatSw3), "HeatSw3");
				NexSend(BoolToString(LtSw3), "LtSw3");
				NexSend(String(ONhh3), "ONhh3");
				NexSend(String(ONmm3), "ONmm3");
				NexSend(String(OFFhh3), "OFFhh3");
				NexSend(String(OFFmm3), "OFFmm3");

				break;
			case 12:
				NexSend(String(Heat4), "Heat4");
				NexSend(BoolToString(HeatSw4), "HeatSw4");
				NexSend(BoolToString(LtSw4), "LtSw4");
				NexSend(String(ONhh4), "ONhh4");
				NexSend(String(ONmm4), "ONmm4");
				NexSend(String(OFFhh4), "OFFhh4");
				NexSend(String(OFFmm4), "OFFmm4");
				break;
			case 13:
				NexSend(BoolToString(LtSw5), "LtSw5");
				NexSend(String(OFFhh5), "OFFhh5");
				NexSend(String(OFFmm5), "OFFmm5");
				NexSend(String(ONhh5), "ONhh5");
				NexSend(String(ONmm5), "ONmm5");
				NexSend(BoolToString(HeatSw5), "HeatSw5");
				NexSend(String(Heat5), "Heat5");
				
				break;
			case 14:

				break;
			case 15:
				NexSend(String(YY), "YY");
				NexSend(String(MM), "MM");
				NexSend(String(DD), "DD");
				NexSend(String(hh), "hh");
				NexSend(String(mm), "mm");
				break;
			default:
				break;
			}
		}
		else {
			VarWrite(NexInputName, NexInputvar);

		}
	}
}
void HC12() {
	String ser0, ser1;
	if (Serial1.available() > 0) {
		while (Serial1.available() > 0) {
			char inputchar;
			inputchar = Serial1.read();
			ser1 = ser1 + inputchar;
			delay(10);
		}
		Serial.println(ser1);
		ser1 = "";
	}
	if (Serial.available() > 0) {
		while (Serial.available() > 0) {
			char inputchar;
			inputchar = Serial.read();
			ser0 = ser0 + inputchar;
			delay(10);
		}
		Serial1.print(ser0);
		ser0 = "";
	}

}

void NextionParser() {
	NexSend(String(IntervalClocks / 1000), "IntervalClocks");
	NexSend(String(YY), "YY");
	NexSend(String(W), "W");
	NexSend(String(MM), "MM");
	NexSend(String(DD), "DD");
	NexSend(String(hh), "hh");
	NexSend(String(mm), "mm");
	NexSend(String(ss), "ss");
	NexSend(String(IntervalBMP / 1000), "IntervalBMP");
	NexSend(String(BMPTemper), "BMPTemper");
	NexSend(String(BMPPress), "BMPPress");
	NexSend(String(BMPAlt), "BMPAlt");
	NexSend(BoolToString(BMPON), "BMPON");
	NexSend(String(dhtT1), "dhtT1");
	NexSend(String(dhtT2), "dhtT2");
	NexSend(String(dhtT3), "dhtT3");
	NexSend(String(dhtT4), "dhtT4");
	NexSend(String(dhtT5), "dhtT5");
	NexSend(String(dhtH1), "dhtH1");
	NexSend(String(dhtH2), "dhtH2");
	NexSend(String(dhtH3), "dhtH3");
	NexSend(String(dhtH4), "dhtH4");
	NexSend(String(dhtH5), "dhtH5");
	NexSend(String(IntervalDHT / 1000), "IntervalDHT");
	NexSend(BoolToString(DHTON), "DHTON");
	NexSend(String(IntervalHeat / 1000), "IntervalHeat");
	NexSend(String(Heat1), "Heat1");
	NexSend(String(Heat2), "Heat2");
	NexSend(String(Heat3), "Heat3");
	NexSend(String(Heat4), "Heat4");
	NexSend(String(Heat5), "Heat5");
	NexSend(String(HeatGyst), "HeatGyst");
	NexSend(BoolToString(HeatAll), "HeatAll");
	NexSend(BoolToString(HeatSw1), "HeatSw1");
	NexSend(BoolToString(HeatSw2), "HeatSw2");
	NexSend(BoolToString(HeatSw3), "HeatSw3");
	NexSend(BoolToString(HeatSw4), "HeatSw4");
	NexSend(BoolToString(HeatSw5), "HeatSw5");
	NexSend(BoolToString(HeatOn1), "HeatOn1");
	NexSend(BoolToString(HeatOn2), "HeatOn2");
	NexSend(BoolToString(HeatOn3), "HeatOn3");
	NexSend(BoolToString(HeatOn4), "HeatOn4");
	NexSend(BoolToString(HeatOn5), "HeatOn5");
	NexSend(String(HVl11), "HVl11");
	NexSend(String(HVl12), "HVl12");
	NexSend(String(HVl13), "HVl13");
	NexSend(String(HVl14), "HVl14");
	NexSend(String(HVl15), "HVl15");
	NexSend(String(HVl16), "HVl16");
	NexSend(String(HVl21), "HVl21");
	NexSend(String(HVl22), "HVl22");
	NexSend(String(HVl23), "HVl23");
	NexSend(String(HVl24), "HVl24");
	NexSend(String(HVl25), "HVl25");
	NexSend(String(HVl26), "HVl26");
	NexSend(String(WLv), "WLv");
	NexSend(String(IntervalPoliv / 1000), "IntervalPoliv");
	NexSend(String(VlP11), "VlP11");
	NexSend(String(VlP12), "VlP12");
	NexSend(String(VlP13), "VlP13");
	NexSend(String(VlP14), "VlP14");
	NexSend(String(VlP15), "VlP15");
	NexSend(String(VlP16), "VlP16");
	NexSend(String(VlP21), "VlP21");
	NexSend(String(VlP22), "VlP22");
	NexSend(String(VlP23), "VlP23");
	NexSend(String(VlP24), "VlP24");
	NexSend(String(VlP25), "VlP25");
	NexSend(String(VlP26), "VlP26");
	NexSend(String(VlPTime), "VlPTime");
	NexSend(BoolToString(VlPAll), "VlPAll");
	NexSend(BoolToString(VlPSw1), "VlPSw1");
	NexSend(BoolToString(VlPSw11), "VlPSw11");
	NexSend(BoolToString(VlPSw12), "VlPSw12");
	NexSend(BoolToString(VlPSw13), "VlPSw13");
	NexSend(BoolToString(VlPSw14), "VlPSw14");
	NexSend(BoolToString(VlPSw15), "VlPSw15");
	NexSend(BoolToString(VlPSw16), "VlPSw16");
	NexSend(BoolToString(VlPSw2), "VlPSw2");
	NexSend(BoolToString(VlPSw21), "VlPSw21");
	NexSend(BoolToString(VlPSw22), "VlPSw22");
	NexSend(BoolToString(VlPSw23), "VlPSw23");
	NexSend(BoolToString(VlPSw24), "VlPSw24");
	NexSend(BoolToString(VlPSw25), "VlPSw25");
	NexSend(BoolToString(VlPSw26), "VlPSw26");
	NexSend(String(IntervalLight / 1000), "IntervalLight");
	NexSend(String(ONhh11), "ONhh11");
	NexSend(String(ONmm11), "ONmm11");
	NexSend(String(ONhh12), "ONhh12");
	NexSend(String(ONmm12), "ONmm12");
	NexSend(String(ONhh21), "ONhh21");
	NexSend(String(ONmm21), "ONmm21");
	NexSend(String(ONhh22), "ONhh22");
	NexSend(String(ONmm22), "ONmm22");
	NexSend(String(ONhh3), "ONhh3");
	NexSend(String(ONmm3), "ONmm3");
	NexSend(String(ONhh4), "ONhh4");
	NexSend(String(ONmm4), "ONmm4");
	NexSend(String(ONhh5), "ONhh5");
	NexSend(String(ONmm5), "ONmm5");
	NexSend(String(OFFhh11), "OFFhh11");
	NexSend(String(OFFmm11), "OFFmm11");
	NexSend(String(OFFhh12), "OFFhh12");
	NexSend(String(OFFmm12), "OFFmm12");
	NexSend(String(OFFhh21), "OFFhh21");
	NexSend(String(OFFmm21), "OFFmm21");
	NexSend(String(OFFhh22), "OFFhh22");
	NexSend(String(OFFmm22), "OFFmm22");
	NexSend(String(OFFhh3), "OFFhh3");
	NexSend(String(OFFmm3), "OFFmm3");
	NexSend(String(OFFhh4), "OFFhh4");
	NexSend(String(OFFmm4), "OFFmm4");
	NexSend(String(OFFhh5), "OFFhh5");
	NexSend(String(OFFmm5), "OFFmm5");
	NexSend(BoolToString(LtSw11), "LtSw11");
	NexSend(BoolToString(LtSw12), "LtSw12");
	NexSend(BoolToString(LtSw21), "LtSw21");
	NexSend(BoolToString(LtSw22), "LtSw22");
	NexSend(BoolToString(LtSw3), "LtSw3");
	NexSend(BoolToString(LtSw4), "LtSw4");
	NexSend(BoolToString(LtSw5), "LtSw5");
	NexSend(BoolToString(LtOn11), "LtOn11");
	NexSend(BoolToString(LtOn12), "LtOn12");
	NexSend(BoolToString(LtOn21), "LtOn21");
	NexSend(BoolToString(LtOn22), "LtOn22");
	NexSend(BoolToString(LtOn3), "LtOn23");
	NexSend(BoolToString(LtOn4), "LtOn24");
	NexSend(BoolToString(LtOn5), "LtOn25");
	NexSend(BoolToString(LtSwAll), "LtSwAll");
	NexSend(BoolToString(BthSw), "BthSw");
	NexSend(BoolToString(WFSw), "WFSw");
}

/*--------------------------------------------Изменение переменных-------------------------------------------------*/
/*
int YY, W, MM, DD, hh, mm, ss;
unsigned long IntervalClocks = 7000;
//установка времени
bool SetupTime = false;
int SetYY, SetW, SetMM, SetDD, Sethh, Setmm, Setss; //день недели указывается в виде числа от 0-воскресенье до 6-суббота


int BMPTemper;
int BMPPress;
int BMPAlt;
bool BMPON;


int dhtT1, dhtT2, dhtT3, dhtT4, dhtT5;     //Температура с датчиков
int dhtH1, dhtH2, dhtH3, dhtH4, dhtH5;	   //Влажность с датчиков
unsigned long IntervalDHT = 13000;
bool DHTON;


unsigned long IntervalHeat = 5000;
int Heat1, Heat2, Heat3, Heat4, Heat5;     //Температура включения
int HeatGyst;                              //Гистерезис
bool HeatAll;																   //Выключатель обогрева общий
bool HeatSw1, HeatSw2, HeatSw3, HeatSw4, HeatSw5;							   //Выключатель обогрева по отсекам
bool HeatOn1, HeatOn2, HeatOn3, HeatOn4, HeatOn5;							   //Команда на MOSFET


unsigned long IntervalHWL = 60000;
int HVl11, HVl12, HVl13, HVl14, HVl15, HVl16, HVl21, HVl22, HVl23, HVl24, HVl25, HVl26; //Влажность земли
int WLv;	

unsigned long IntervalPoliv = 60000;
int VlP11, VlP12, VlP13, VlP14, VlP15, VlP16;     //Влажность полива отсек1
int VlP21, VlP22, VlP23, VlP24, VlP25, VlP26;     //Влажность полива отсек2
int VlPTime;									  //Время включения помпы
bool VlPAll;																       //Выключатель полива общий
bool VlPSw11, VlPSw12, VlPSw13, VlPSw14, VlPSw15, VlPSw16;						   //Выключатель полива по горшкам отсек1
bool VlPSw21, VlPSw22, VlPSw23, VlPSw24, VlPSw25, VlPSw26;						   //Выключатель полива по горшкам отсек2



unsigned long IntervalLight = 3000;
int ONhh11, ONmm11, ONhh12, ONmm12, ONhh21, ONmm21, ONhh22, ONmm22, ONhh3, ONmm3, ONhh4, ONmm4, ONhh5, ONmm5;
int OFFhh11, OFFmm11, OFFhh12, OFFmm12, OFFhh21, OFFmm21, OFFhh22, OFFmm22, OFFhh3, OFFmm3, OFFhh4, OFFmm4, OFFhh5, OFFmm5;
bool LtSw11, LtSw12, LtSw21, LtSw22, LtSw3, LtSw4, LtSw5;
bool LtUpd = false;
*/

void setup()
{
	CurrentTime();
	ClockSetup();
	//ROMDEFAULT();
	//Time.settime(Setss, Setmm, Sethh, SetDD, SetMM, SetYY, SetW);  //день недели указывается в виде числа от 0-воскресенье до 6-суббота	
	//Time.settime(-1, -1, -1, -1, -1, -1, 1); 
	BMPSetup();
	TempHumDHTSetup();
	HeatSetup();
	GroundSetup();
	PolivSetup();
	ROMSETUP();
	LightSetup();
	NextionSetup();
	Serial.begin(115200);
}

void loop()
{
	SetupTimeClock(SetYY, SetW, SetMM, SetDD, Sethh, Setmm, Setss);
	CurrentTime();
	ReadClock();
	BMPRead();
	TempHumDHT();
	Heat();
	Ground();
	Poliv();
	Light();
	NextionSerial();
	//HC12();
}

	/*
	Serial.print(DD); 
	Serial.print('.');
	Serial.print(MM);
	Serial.print('.');
	Serial.print(YY);
	Serial.print(" /");
	Serial.println(W);
	Serial.print(hh);
	Serial.print(':');
	Serial.print(mm);
	Serial.print(':');
	Serial.println(ss);
	Serial.println(LightON11);
	Serial.println(LightTime);
	delay(1000);*/