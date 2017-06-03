#pragma once
//command ID list\\

#define SNS_emergency_val				 100
#define SNS_ovenTemp_deg				 101
#define SNS_coolerTemp_deg				 102
#define SNS_peltier_deg					 103
#define SNS_H2water_val					 104//0=0;1=1;2=1&2
#define SNS_CHwater_val					 105//0=0;1=1;2=1&2
#define SNS_CO2_perc					 106
#define SNS_H2_perc						 107
#define SNS_O2storagepressure_mbar		 108
#define SNS_H2Storagepressure_mbar       109
#define SNS_CO2pressure_mbar		     110
#define SNS_O2water_val                  111//0=0;1=1;2=1&2
#define SNS_H2valve_perc				 112
#define SNS_CO2valve_perc				 113
#define SNS_O2electrolyzerpressure_mbar  114
#define SNS_H2electrolyzerpressure_mbar  114

#define STT_CO2VALVE            200
#define STT_H2INVALVE			201
#define STT_H2OUTVALVE			202
#define STT_O2INVALVE			203
#define STT_O2OUTVALVE			204
#define STT_H2REFLUX			205
#define STT_O2REFLUX			206
#define STT_PUMP				207
#define STT_HEARTBEAT           208
#define STT_FLUSH				209
#define STT_CHWATER             210

#define CMD_start_CMD           300
#define CMD_oventemp_val        301
#define CMD_CO2VALVE            302







//misc
#define open 1
#define close 0
/*
AD7828
CH0 8C
CH1 CC
CH2 9C
CH3 DC
CH4 AC
CH5 EC
CH6 FC
CH7 BC
*/

/*



*/

//todo:
/*
O2 Druck Elektrolyse -
H2 Druck EL -
O2 druck speicher -
H2/CO2 druck speicher -
Raumtemperatur -
Füllstand O2 -
Füllstand H2 -
Füllstand CH4
Ofentemperatur -
flow CO2H2
H2 konz.
CO2 konz.
HTY CO,H2
HTY H2
HTY O2


CH4 konz.

*/
