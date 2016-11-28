#pragma once

byte addr_Lights = 0x25;         ///
       ///
byte addr_Valves2 = 0x25;        ///
byte addr_Valves3 = 0x22;        ///
byte addr_Valves4 = 0x23;  
byte addr_M_Valve = 0x26;
byte addr_Items1 = 0x22;         ///
byte addr_Items2 = 0x23;         ///
byte addr_Items3 = 0x20;         ///
byte addr_Sensors = 0x4b;
byte addr_Sensors2 = 0x48;
byte addr_switch = 0x70;
byte addr_AD = 0x35;
byte addr_DA = 0x1F;
byte addr_RT = 0x4F;
      //NEW--
//0x48 = Temperatur Kühler
//0x28 = Feuchte/Temperatur Ofen auslass
//byte addr_Motpos = 0x48;
byte addr_Valves = 0x26; 
byte addr_Power = 0x22;
byte addr_Valves_sensor = 0x4B;
byte addr_Mot_sensors = 0x49;
byte addr_Mot = 0x20;
byte addr_AD1 = 0x4b;
byte addr_Sensors3 = 0x49;
byte addr_Sensorcard_1 = 0x00;
byte addr_Sensorcard_2 = 0x00;
byte addr_Sensorcard_3 = 0x00;
byte addr_Sensorcard_4 = 0x00;

/*
todo:

*/

//DC: -- 4 --> < 4000
//AC: -- 3 --> < 3000
//BC: -- 2 --> < 4000
//FC: -- 1 --> < 3000

//1234


/*

Motor 1: 1.6-1.0-1.7-1.1 (P6-P0-P7-P1)
Motor 2: 2.3-1.2-2.2-1.3 (P13-P2-P12-P3)
Motor 3: 2.0-1.4-2.1-1.5 (P10-P4-P11-P5)  

Input 1: 2.4 (P14)
Input 2: 2.5 (P15)
Input 3: 2.6 (P16)
Input 4: 2.7 (P17)

Motpos 1: 1.6 (K5)
Motpos 2: 1.7 (K5)

Ventile:
0:Working
1:Ready
2:Danger
3:Electrolyzer-LED
4:Oven
5:Electrolyzer
6:Oven-LED
7:Mix C02
8:Water outlet
9:Mix H2
10:Flush
11:Intake Co2
*/


