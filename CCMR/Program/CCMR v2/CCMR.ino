#include <Wire.h>
#include "CCMR.h"
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

bool ok = false;
void setup()
{
	
CCMR.init(true);
CCMR.valves.CO2_needle_idle();
CCMR.valves.H2_needle_idle();
CCMR.lights.Ready(false);
CCMR.lights.Running(true);
CCMR.valves.CO2(false);
CCMR.valves.H2(false);
CCMR.items.Oven(false);
CCMR.valves.Water_reflux_CH4(false);
}

void loop()
{

	
	
}


void Mix()
{
  if ( abs(CCMR.sensors.H2_percentage() - 80) > CO2_percentage_tolerance)
  {
  if (CCMR.sensors.H2_percentage() > 80)
  {
     CCMR.valves.H2_needle(false);
  }
  else
  {
    CCMR.valves.H2_needle(true);
  }
  }
  CCMR.valves.H2_needle_idle();
}

//void check_H2()
//{
//  
//  if ( abs(CCMR.sensors.H2_pressure() - desired_H2_pressure) > H2_pressure_tolerance )
//  {
//    
// if (CCMR.sensors.H2_pressure() > desired_H2_pressure)
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
/*oid check_danger()
{
	if((CCMR.sensors.Oven_temp() < 300) || (CCMR.sensors.H2() < 400))
	{
		ok = false;
	}
	else
	{
		ok = true;
	}

	if ((CCMR.sensors.Oven_temp() > 400) || (CCMR.sensors.H2_pressure() > 900))
	{
		CCMR.lights.Caution(true);
	}
	else
	{
		CCMR.lights.Caution(false);
	}
}*/
void check_water()
{
	if (CCMR.sensors.H2_water(1 && 2))
	{
		CCMR.lights.Caution(true);
		CCMR.valves.Water_reflux_H2(true);
		while (CCMR.sensors.H2_water(1))
		{
			delay(50);
		}
		CCMR.valves.Water_reflux_H2(false);
		CCMR.lights.Caution(false);
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
void readmsg()
{
  

  
}
/*
1 -- 3
0 -- 2
6 -- 11
7 -- 10

ss
*/
