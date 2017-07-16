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

int desired_H2_storage_pressure = 1000;
//##############################################\\

unsigned long previousMillis_A = 0;
unsigned long previousMillis_B = 0;
unsigned long previousMillis_C = 0;
unsigned long previousMillis_S = 0;
int upd = 0;
void setup()
{
	
CCMR.init(false);
Serial.setTimeout(5);
pinMode(13, OUTPUT);
}

void loop()
{
	Timed_loop();
	continousLoop();
	
}
void Timed_loop()
{
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis_A >= 200)
	{
		previousMillis_A = currentMillis;
		FastLoop();
	}
	if (currentMillis - previousMillis_S >= 500)
	{
		previousMillis_S = currentMillis;
		specialloop();
	}
	if (currentMillis - previousMillis_B >= 1000)
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
	/*CCMR.valves.CO2_Flowrate_step(0);
	delay(4);*/
}
void FastLoop()
{
	handleCommands();
}
void MidLoop()
{
	

}
void SlowLoop()
{
	Serial.println(CCMR.sensors.H2_storagepressure());
}
void specialloop()
{
	updateGUI();
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
	case STT_CHWATER:
	{
		if (cmdval == 1)
		{
			CCMR.valves.CH_Water(true);
		}
		else
		{
			CCMR.valves.CH_Water(false);
		}
		break;
	}
	default:

		break;
	}
}

void updateGUI()
{
	switch (upd)
	{
	default: break;
	case 0:
		//utils.Send_to_GUI(SNS_ovenTemp_deg, CCMR.sensors.Oven_temp());
		utils.Send_to_GUI(SNS_ovenTemp_deg, CCMR.sensors.Oven_temp());
		break;
	case 1:
		utils.Send_to_GUI(SNS_H2water_val, CCMR.sensors.H2_water(1) ? (CCMR.sensors.H2_water(2) ? 2 : 1) : 0);//0=0;1=1;2=1&2
		break;
	case 2:
		utils.Send_to_GUI(SNS_CHwater_val, CCMR.sensors.CH_water(1) ? (CCMR.sensors.CH_water(2) ? 2 : 1) : 0);//0=0;1=1;2=1&2
		break;
	case 3:
		utils.Send_to_GUI(SNS_O2storagepressure_mbar, CCMR.sensors.O2_storagepressure());
		break;
	case 4:
		utils.Send_to_GUI(SNS_H2Storagepressure_mbar, CCMR.sensors.H2_storagepressure());
		break;
	case 5:
		utils.Send_to_GUI(SNS_CO2pressure_mbar, CCMR.sensors.CO2_pressure());
		break;
	case 6:
		utils.Send_to_GUI(SNS_H2valve_perc, CCMR.sensors.H2_outvalve());
		break;
	case 7:
		utils.Send_to_GUI(SNS_CO2valve_perc, CCMR.sensors.CO2_valve());
		break;
	case 8:
		utils.Send_to_GUI(SNS_O2electrolyzerpressure_mbar, CCMR.sensors.O2_pressure_Electrolyzer());
		break;
	case 9:
		utils.Send_to_GUI(SNS_H2electrolyzerpressure_mbar, CCMR.sensors.H2_pressure_Electrolyzer());
		break;
	case 10:
		utils.Send_to_GUI(SNS_O2water_val, CCMR.sensors.O2_water(1) ? (CCMR.sensors.O2_water(2) ? 2 : 1) : 0);//0=0;1=1;2=1&2
		break;

	}
	if (upd >= 10)
	{
		upd = 0;
	}
	else
	{
		upd++;
	}

	
}

void Mix()
{
  if ( abs(CCMR.sensors.H2_percentage() - 80) > H2_percentage_tolerance)
  {
  if (CCMR.sensors.H2_percentage() > 80)
  {
     CCMR.valves.H2_Flowrate_step(close);
  }
  else
  {
    CCMR.valves.H2_Flowrate_step(open);
  }
  }
 // CCMR.valves.H2_needle_idle();
}

void check_H2()
{
	
  if ( abs(CCMR.sensors.H2_pressure_Electrolyzer() - desired_H2_pressure) > H2_pressure_tolerance )
  {
    
 if (CCMR.sensors.H2_pressure_Electrolyzer() > desired_H2_pressure)
 {   
   CCMR.items.Electrolyzer_1(false);  
   CCMR.items.Electrolyzer_2(false);
 }
 else
 {
	 
   CCMR.items.Electrolyzer_1(true);  
   CCMR.items.Electrolyzer_2(true);
 }
  }

  if (abs(CCMR.sensors.H2_storagepressure() - desired_H2_storage_pressure) > H2_pressure_tolerance)
  {

	  if (CCMR.sensors.H2_storagepressure() > desired_H2_storage_pressure)
	  {
		  CCMR.valves.H2_in(false);
	  }
	  else
	  {

		  CCMR.valves.H2_in(true);
	  }
  }
  

}

void check_Oven()
{

	if (abs(CCMR.sensors.Oven_temp() - desired_Oven_temperature) > Oven_temperature_tolerance)
	{

		if (CCMR.sensors.Oven_temp() > desired_Oven_temperature)
		{
			CCMR.items.Oven(false);
		}
		else
		{
			CCMR.items.Oven(true);
		}
	}

	

	

}

void check_water()
{
	if (CCMR.sensors.H2_water(1 && 2))
	{
		CCMR.valves.Water_reflux_H2(true);
		while (CCMR.sensors.H2_water(1))
		{
			delay(5);
		}
		CCMR.valves.Water_reflux_H2(false);
	}
	if (CCMR.sensors.O2_water(1 && 2))
	{
		CCMR.valves.Water_reflux_O2(true);
		while (CCMR.sensors.H2_water(1))
		{
			delay(5);
		}
		CCMR.valves.Water_reflux_H2(false);
	}
}

void check_temperature()
{
	if (abs(CCMR.sensors.cooler() - desired_Cooler_temperature) > Cooler_temperature_tolerance)
	{

		if (CCMR.sensors.cooler() > desired_Cooler_temperature)
		{
			CCMR.items.Fan(true);
		}
		else
		{
			CCMR.items.Fan(false);
		}
	}

	if (abs(CCMR.sensors.peltier() - desired_Peltier_temperature) > Peltier_temperature_tolerance)
	{

		if (CCMR.sensors.peltier() > desired_Peltier_temperature)
		{
			CCMR.items.Peltier(true);
		}
		else
		{
			CCMR.items.Peltier(false);
		}
	}
}




/*
1 -- 3
0 -- 2
6 -- 11
7 -- 10

ss
*/
