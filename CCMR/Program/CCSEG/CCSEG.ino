//PROG CCSEG\\

#include <Wire.h>
#include "CCSEG.h"
#include "Communication.h"
//#include <avr/interrupt.h>
//#include <TimedAction.h>
ComputerControlledMethanReactor CCMR;
Utils utils;
//settings

//##############################################\\

unsigned long previousMillis_A = 0;
unsigned long previousMillis_B = 0;
unsigned long previousMillis_C = 0;

void setup()
{
	
CCMR.init(false);
Serial.setTimeout(5);

// richtung ändern CCMR.utils.I2CWRITE(PCF8574::adress, 1, 1);

}

void loop()
{
	Timed_loop();
	continousLoop();
}
void Timed_loop()
{
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis_A >= 100)
	{
		previousMillis_A = currentMillis;
		FastLoop();
	}
	if (currentMillis - previousMillis_B >= 500)
	{
		previousMillis_B = currentMillis;
		MidLoop();
	}
	if (currentMillis - previousMillis_C >= 5000)
	{
		previousMillis_C = currentMillis;
		SlowLoop();
	}
}

//LOOPS
void continousLoop()
{
	//CCMR.valves.Butan_Flowrate_step(5);

}
void FastLoop()
{
}
void MidLoop()
{
	//Serial.println(CCMR.sensors.Motor_valve());
	CCMR.valves.Butan(true);
	delay(1000);
	CCMR.valves.Butan(false);
	handleCommands();
}
void SlowLoop()
{
	//updateGUI();
	Serial.println("slowloop!");
}


void handleCommands()
{
	byte cmdid;
	byte cmdval;
	if (Serial2.available())
	{
		cmdid = Serial2.read();
		cmdval = Serial2.read();
	}
	switch (cmdid)
	{
	case CMD_start_CMD:
	{
		break;
	}
	
	default:

		break;
	}
}
void updateGUI()
{
	//utils.Send_to_GUI(SNS_H2Storagepressure_val, CCMR.sensors.H2_storagepressure());
}
