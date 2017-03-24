#include <Wire.h>
#include "CCMR.h"
#include "Communication.h"
//#include <avr/interrupt.h>
//#include <TimedAction.h>
ComputerControlledMethanReactor CCMR;
Utils utils;
//settings
int desired_H2_precentage = 80;
int H2_percentage_tolerance = 0;

int desired_CO2_precentage = 20;
int CO2_percentage_tolerance = 0;

int desired_H2_pressure = 500;
int H2_pressure_tolerance = 10;

int desired_Oven_temperature = 300;
int Oven_temperature_tolerance = 5;

int desired_Peltier_temperature = 0;
int Peltier_temperature_tolerance = 2;

int desired_Cooler_temperature = 40;
int Cooler_temperature_tolerance = 5;

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
	CCMR.valves.CO2_Flowrate_step(0);
	delay(4);
}
void FastLoop()
{

}
void MidLoop()
{
	CCMR.sensors.H2_water(1);
	CCMR.sensors.H2_water(2);

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
	case STT_CO2VALVE:
	{
		if (cmdval == 1)
		{
			CCMR.valves.CO2(true);
		}
		else
		{
			CCMR.valves.CO2(false);
		}
		break;
	}

	case STT_H2INVALVE:
	{
		if (cmdval == 1)
		{
			Serial.println("ON!");
			CCMR.valves.H2_in(true);
		}
		else
		{
			CCMR.valves.H2_in(false);
		}
		break;
	}

	case STT_H2OUTVALVE:
	{
		if (cmdval == 1)
		{
			CCMR.valves.H2_out(true);
		}
		else
		{
			CCMR.valves.H2_out(false);
		}
		break;
	}

	case STT_H2REFLUX:
	{
		if (cmdval == 1)
		{
			CCMR.valves.Water_reflux_H2(true);
		}
		else
		{
			CCMR.valves.Water_reflux_H2(false);
		}
		break;
	}

	case STT_O2INVALVE:
	{
		if (cmdval == 1)
		{
			CCMR.valves.O2_in(true);
		}
		else
		{
			CCMR.valves.O2_in(false);
		}
		break;
	}

	case STT_O2OUTVALVE:
	{
		if (cmdval == 1)
		{
			CCMR.valves.O2_out(true);
		}
		else
		{
			CCMR.valves.O2_out(false);
		}
		break;
	}

	case STT_O2REFLUX:
	{
		if (cmdval == 1)
		{
			CCMR.valves.Water_reflux_O2(true);
		}
		else
		{
			CCMR.valves.Water_reflux_O2(false);
		}
		break;
	}

	case STT_PUMP:
	{
		if (cmdval == 1)
		{
			CCMR.items.Pump(true);
		}
		else
		{
			CCMR.items.Pump(false);
		}
		break;
	}
	case STT_FLUSH:
	{
		if (cmdval == 1)
		{
			CCMR.valves.Flush(true);
		}
		else
		{
			CCMR.valves.Flush(false);
		}
		break;
	}
	default:

		break;
	}
}
void updateGUI()
{
	/*CCMR.utils.Send_to_GUI(STT_CO2VALVE, CCMR.sys_stat.CO2());
	CCMR.utils.Send_to_GUI(STT_H2INVALVE, CCMR.sys_stat.H2_in());
	CCMR.utils.Send_to_GUI(STT_H2OUTVALVE, CCMR.sys_stat.H2_out());
	CCMR.utils.Send_to_GUI(STT_O2INVALVE, CCMR.sys_stat.O2_in());
	CCMR.utils.Send_to_GUI(STT_O2OUTVALVE, CCMR.sys_stat.O2_out());
	CCMR.utils.Send_to_GUI(STT_H2REFLUX, CCMR.sys_stat.Water_reflux_H2());
	CCMR.utils.Send_to_GUI(STT_O2REFLUX, CCMR.sys_stat.Water_reflux_O2());
	CCMR.utils.Send_to_GUI(STT_PUMP, CCMR.sys_stat.Pump());*/
	
	utils.Send_to_GUI(SNS_H2Storagepressure_val, CCMR.sensors.H2_storagepressure());
}
//void Mix()
//{
//  if ( abs(CCMR.sensors.H2_percentage() - 80) > CO2_percentage_tolerance)
//  {
//  if (CCMR.sensors.H2_percentage() > 80)
//  {
//     CCMR.valves.H2_needle(false);
//  }
//  else
//  {
//    CCMR.valves.H2_needle(true);
//  }
//  }
//  CCMR.valves.H2_needle_idle();
//}
//
//void check_H2()
//{
//  
//  if ( abs(CCMR.sensors.H2_pressure_Electrolyzer() - desired_H2_pressure) > H2_pressure_tolerance )
//  {
//    
// if (CCMR.sensors.H2_pressure_Electrolyzer() > desired_H2_pressure)
// {   
//   CCMR.items.Electrolyzer_1(false);  
//   CCMR.items.Electrolyzer_2(false);
// }
// else
// {
//   CCMR.items.Electrolyzer_1(true);  
//   CCMR.items.Electrolyzer_2(true);
// }
//  }
//  
//}
//
//void check_Oven()
//{
//
//	if (abs(CCMR.sensors.Oven_temp() - desired_Oven_temperature) > Oven_temperature_tolerance)
//	{
//
//		if (CCMR.sensors.Oven_temp() > desired_Oven_temperature)
//		{
//			CCMR.items.Oven(false);
//		}
//		else
//		{
//			CCMR.items.Oven(true);
//		}
//	}
//
//	
//
//	
//
//}
//
//void check_water()
//{
//	if (CCMR.sensors.H2_water(1 && 2))
//	{
//		CCMR.lights.Caution(true);
//		CCMR.valves.Water_reflux_H2(true);
//		while (CCMR.sensors.H2_water(1))
//		{
//			delay(50);
//		}
//		CCMR.valves.Water_reflux_H2(false);
//		CCMR.lights.Caution(false);
//	}
//}
//void check_temperature()
//{
//	if (abs(CCMR.sensors.cooler() - desired_Cooler_temperature) > Cooler_temperature_tolerance)
//	{
//
//		if (CCMR.sensors.cooler() > desired_Cooler_temperature)
//		{
//			CCMR.items.Fan(true);
//		}
//		else
//		{
//			CCMR.items.Fan(false);
//		}
//	}
//
//	if (abs(CCMR.sensors.peltier() - desired_Peltier_temperature) > Peltier_temperature_tolerance)
//	{
//
//		if (CCMR.sensors.peltier() > desired_Peltier_temperature)
//		{
//			CCMR.items.Peltier(true);
//		}
//		else
//		{
//			CCMR.items.Peltier(false);
//		}
//	}
//}
//void readmsg()
//{
//  
//
//  
//}
/*
1 -- 3
0 -- 2
6 -- 11
7 -- 10

ss
*/
