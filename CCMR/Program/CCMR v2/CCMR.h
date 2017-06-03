#pragma once
#include <HardwareSerial.h>
#include "Arduino.h"
#include "Adresses.h"
#include "Communication.h"
#include "Wire.h"
boolean debug = false;
boolean Ready = false;
boolean Caution = false;
boolean Running = false;
boolean _init = false;
byte State_Lights = 0b11111111;  
int volt;
int st = 0;
int Sw_Reset[2];
int Set_Ref[2];
int Code_Load[2];
int RTR;
int CO2_reset = 25;
int H2_reset = 85;
byte AD[23];
byte RT[1];
#define define_adresses AD7828 AD_1; PCF8575 PCF;TMP100 TMP;PCA9545 PCA;MAX1238 MAX_1;MAX5815 MAX_2;HTY_1 HT_1;HTY_2 HT_2;SM5812 SM_1;SM5812 SM_2;CVAI DA;


class Utils
{
public:
void Send_to_GUI(byte CMDID, byte CMDVAL)
  {
	Serial2.write(CMDID);
	Serial2.write(CMDVAL);
  }
void channel_switch_2(int num)
{

	if (num == 0)
	{
		I2C(PCA9545_2::adress, 0xF0);
	}
	else if (num == 1)
	{
		I2C(PCA9545_2::adress, 0xF1);
	}
	else if (num == 2)
	{
		I2C(PCA9545_2::adress, 0xF2);
	}
	else if (num == 3)
	{
		//I2C(addr_switch,0xF4);
	}
	else if (num == 4)
	{
		//I2C(addr_switch,0xF8);
	}
}

void channel_switch(int num)
  {
	 
  if (num == 0)
  {
    I2C(PCA9545::adress,0xF0);   
  }
  else if (num == 1)
  {
	I2C(PCA9545::adress, 0xF1);
  }
  else if (num == 2)
  {
    I2C(PCA9545::adress,0xF2);
  }
  else if (num == 3)
  {
    //I2C(addr_switch,0xF4);
  }
  else if (num == 4)
  {
    //I2C(addr_switch,0xF8);
  }
  }
  byte* I2Customread(byte addr,int bytes)
  {
    Wire.beginTransmission(addr);
    Wire.requestFrom((int)addr, bytes);
    //int c = bytes;
    //while (Wire.available())
    //{

    _data[0] = Wire.read();    
    _data[1] = Wire.read();

    //Serial.println(_data[c]);
    // c++;
    //}

	if (debug)
	{
		char temp[256];

		sprintf(temp, " packets have been received from 0x%x with value 0x%x%x\n", addr, _data[0], _data[1]);

		if (Ready == 1)
		{
			Serial.write(temp);
		}

	}
    return _data;
  }

  void write16(uint8_t v_A,uint8_t v_B,uint8_t addr)
  {
	  
    Wire.beginTransmission(addr);
    Wire.write(v_A);
    Wire.write(v_B);

    Wire.endTransmission();

    if (debug)
    {
      char temp[256];

      sprintf(temp,"two packets have been sent to 0x%x with value 0x%x%x\n", addr, v_A,v_B);

      if (Ready == 1)
      {
        Serial.write(temp);
      }

    }
  }
  boolean I2CREAD_M(uint8_t address, uint8_t pin)
  {
	  I2Customread(address, 2);
	  if (pin < 8)
	  {
		  if (!(_data[0] & (1 << pin)))
		  {
			  return true;
		  }
		  else
		  {
			  return false;
		  }
	  }
	  else
	  {
		  if (!(_data[1] & (1 << (pin - 10))))
		  {
			  return true;
		  }
		  else
		  {
			  return false;
		  }
	  }
  }

  boolean I2CREAD(uint8_t address, uint8_t pin)
  {
	  I2Customread(address, 1);
	  if (pin < 8)
	  {
		  if (!(_data[0] & (1 << pin)))
		  {
			  return true;
		  }
		  else
		  {
			  return false;
		  }
	  }
  }

  void I2CWRITE_M(uint8_t address, uint8_t pin, boolean value)
  {
	  if (!_init)
	  {
	  /*if (pin <= 4)
	 {
	  pin = NULL;
	 }*/
   }
    value = !value;
    I2Customread(address,2); 

     // _data[0]=0b11111111;
    // _data[1]=0b11111111; //SIMULATE
    //Serial.write(value);
    if (pin < 8)
    {
      if (value == 0) 
      {
        _data[0] &= ~(1<<pin);

      }
      else 
      {
        _data[0] |= (1<<pin);

      }
      //Serial.println(_data[0]);
      //Serial.println(_data[1]);
    }
    else
    {
      pin-=10;
      if (value == 0) 
      {

        _data[1] &= ~(1<<pin);
      }
      else 
      {

        _data[1] |= (1<<pin);
      }
      //Serial.write(_data);

      //State_Lights = _data; //SIMULATE 
    }
	
    write16(_data[0],_data[1], address);
  }

  byte I2Cread(byte addr)
  {
    Wire.beginTransmission(addr);
    Wire.requestFrom((int)addr, 1);
    if (ARDUINO <  100)
    {
      _data[2] = Wire.read();
    }
    else
    {
      _data[2] = Wire.read();
    }
    return _data[2];
  }


  void write8(uint8_t value,uint8_t addr)
  {
    _data[2] = value;
    I2C(addr,_data[2]);
  }

  void I2CWRITE(uint8_t address, uint8_t pin, uint8_t value)
  {
	  
    value = !value;
    I2Cread(address); 
    //_data=State_Lights; //SIMULATE
    //Serial.write(value);
    if (value == 0) 
    {
      _data[2] &= ~(1<<pin);
    }
    else 
    {
      _data[2] |= (1<<pin);
    }
    //Serial.write(_data);
    write8(_data[2], address);
    //State_Lights = _data; //SIMULATE 
  }


  void I2C(uint8_t addr, uint8_t value)
  {
    Wire.beginTransmission(addr);
    Wire.write(value);
    Wire.endTransmission();

    if (debug)
    {
      char temp[60];

      sprintf(temp,"a packet has been sent to 0x%x with value: 0x%x\n", addr, value);

      if (Ready)
      {
        Serial.write(temp);
      }

    }
  }

public:
  uint8_t _address;
  uint8_t _data[1];
  bool _debug;
  byte L_Status[];
  //status 0 --> Lights
  //status 1 --> Valves
  //status 2 --> items
};

class Sys_stat
{
	Utils utils;
public:
	//<Magnetic Valves
	boolean CO2()
	{
		utils.channel_switch(1);
		return utils.I2CREAD(PCF8574_2::adress, 0);
		utils.channel_switch(0);
	}

	boolean H2_in()
	{
		utils.channel_switch(1);
		return utils.I2CREAD_M(PCF8575::adress, 12);
		utils.channel_switch(0);
	}

	boolean H2_out()
	{
		utils.channel_switch(1);
		return utils.I2CREAD(PCF8574::adress, 4);
		utils.channel_switch(0);
	}

	boolean O2_in()
	{
		utils.channel_switch(1);
		return utils.I2CREAD_M(PCF8575::adress, 13);
		utils.channel_switch(0);
	}

	boolean O2_out()
	{
		utils.channel_switch(1);
		return utils.I2CREAD_M(PCF8575::adress, 16);
		utils.channel_switch(0);
	}

	boolean Water_reflux_H2()
	{
		utils.channel_switch(1);
		return utils.I2CREAD_M(PCF8575::adress, 10);
		utils.channel_switch(0);
	}

	boolean Water_reflux_O2()
	{
		utils.channel_switch(1);
		return utils.I2CREAD_M(PCF8575::adress, 11);
		utils.channel_switch(0);
	}
	boolean Pump()
	{
		utils.channel_switch(1);
		return utils.I2CREAD_M(PCF8575::adress, 14);
		utils.channel_switch(0);
	}
	boolean CH_water()
	{
		utils.channel_switch(1);
		return utils.I2CREAD_M(PCF8575::adress, 11);
		utils.channel_switch(0);
	}
	boolean Flush()
	{
		utils.channel_switch(1);
		return utils.I2CREAD(PCF8574_2::adress, 4);
		utils.channel_switch(0);
	}
	//>Magnetic Valves


};

class Sensors
{
  Utils utils;

public:
	//new
	int H2_storagepressure() //Y
	{
		utils.channel_switch(MAX1238::bus);
		Wire.beginTransmission(MAX1238::adress);
		Wire.requestFrom(MAX1238::adress, byte(24));
		//------------------------
		for (int i = 0; i <= 23; i++)
		{
			AD[i] = Wire.read();
		}
		
		Wire.endTransmission(MAX1238::adress);

		int output = word(AD[12], AD[13]); //AIN 6
		output = output & 0x0FFF;
		float fout = output;
		//double p = (((fout / 5000) - 0.04) / 0.009) * 10;
		double p = map(fout,160,3150,0,2000);
		utils.channel_switch(0);
		return p;
	}
	int O2_storagepressure()//Y
	{
		utils.channel_switch(MAX1238::bus);
		Wire.beginTransmission(MAX1238::adress);
		Wire.requestFrom(MAX1238::adress, byte(24));
		//------------------------
		for (int i = 0; i <= 23; i++)
		{
			AD[i] = Wire.read();
		}

		Wire.endTransmission(MAX1238::adress);

		int output = word(AD[14], AD[15]); //AIN 7
		output = output & 0x0FFF;
		float fout = output;
		//double p = (((fout / 5000) - 0.04) / 0.009) * 10;
		double p = map(fout, 160, 3150, 0, 2000);
		utils.channel_switch(0);
		return p;
	}
	int O2_pressure_Electrolyzer()//Y
	{
		utils.channel_switch(MAX1238::bus);
		Wire.beginTransmission(MAX1238::adress);
		Wire.requestFrom(MAX1238::adress, byte(24));
		//------------------------
		for (int i = 0; i <= 23; i++)
		{
			AD[i] = Wire.read();
		}

		Wire.endTransmission(MAX1238::adress);

		int output = word(AD[10], AD[11]); //AIN 7
		output = output & 0x0FFF;
		float fout = output;
		//double p = (((fout / 5000) - 0.04) / 0.009) * 10;
		double p = map(fout, 155, 3500, 0, 1000);
		utils.channel_switch(0);
		return p;
	}
	int H2_pressure_Electrolyzer()//Y
	{
		utils.channel_switch(MAX1238::bus);
		Wire.beginTransmission(MAX1238::adress);
		Wire.requestFrom(MAX1238::adress, byte(24));
		//------------------------
		for (int i = 0; i <= 23; i++)
		{
			AD[i] = Wire.read();
		}

		Wire.endTransmission(MAX1238::adress);

		int output = word(AD[8], AD[9]); //AIN 7
		output = output & 0x0FFF;
		float fout = output;
		//double p = (((fout / 5000) - 0.04) / 0.009) * 10;
		double p = fout;//map(fout, 160, 3150, 0, 2000);
		utils.channel_switch(0);
		return p;
	}
	int H2_outvalve()//Y
	{
		utils.I2C(AD7828::adress, 0x8C);
		Wire.beginTransmission(AD7828::adress);
		Wire.requestFrom(AD7828::adress, byte(2));
		int val(((Wire.read() << 8) + Wire.read()));
		Wire.endTransmission();
		//Serial.println(val);
		return val;
	}
	int CO2_valve()//Y
	{
		utils.I2C(AD7828_2::adress, 0x8C);
		Wire.beginTransmission(AD7828_2::adress);
		Wire.requestFrom(AD7828_2::adress, byte(2));
		int val = (((Wire.read() << 8) + Wire.read()));
		//Serial.println(val);
		Wire.endTransmission();
		return val;
	}
	int Room_temp()
	{
		utils.channel_switch(TMP100::bus);
		Wire.beginTransmission(TMP100::adress);
	Wire.requestFrom(TMP100::adress, byte(2));
	//byte MSB = Wire.read();
	//byte LSB = Wire.read();
	                        //MSB                  //LSB  
	int TemperatureSum = ((Wire.read() << 8) | Wire.read()) >> 4;

	float celsius = TemperatureSum*0.0625;
	Wire.endTransmission();
	

	utils.channel_switch(0);
	return celsius;
	}

  boolean Halt()
	{
		

		//utils.I2C(PCF8575::adress, 0xBC);
		// delay(1);
		Wire.beginTransmission(PCF8575::adress);
		Wire.requestFrom(PCF8575::adress,byte(2));
		int read = Wire.read()&0b00010000;
		Wire.read();
		Wire.endTransmission();
		if (read > 1)
		{
			return true;
		}
		else
		{
			return false;
		}
		
	}
  int Oven_temp()
	{
	  utils.channel_switch(MAX1238::bus);
		Wire.beginTransmission(MAX1238::adress);
		Wire.requestFrom(MAX1238::adress, byte(24));
		//------------------------
		for (int i = 0; i <= 23; i++)
		{
			AD[i] = Wire.read();
		}
		Wire.endTransmission(MAX1238::adress);
		int output = word(AD[22], AD[23]);
		output = output & 0x0FFF;
		double x = output; 
		//Serial.print(output);
		//Serial.print(" degree: ");
		//Serial.println(degree(output));
		return degree(output);
		utils.channel_switch(0);
	}
  double degree(double x) {
	  return   45.41742427610426 + 0.20515848069194526*x + 0.00003967586300264838 * pow(x,2);
			
			
	}
  int cooler()
  {
    utils.channel_switch(2);
    Wire.beginTransmission(AD7828::adress);
    Wire.requestFrom((int)AD7828::adress, 2);
    //int a =  word(Wire.read(),Wire.read());
	int a = ((Wire.read() << 8) + Wire.read());
	Serial.print("raw: ");
	Serial.print(a);
    utils.channel_switch(0);
	//a = a & 0b1111111110000000;
    a = (a >> 7);
    if (a < 256)
    {
      a = a / 2;
    }
    else
    {
      a = (a - 512) / 2;
    }
	Serial.print(" || degree:");
		Serial.println(a);
    return a;
  }
  int peltier()
  {
	  utils.channel_switch(2);
	  Wire.beginTransmission(0x49);
	  Wire.requestFrom((int)0x49, 2);
	  int a = word(Wire.read(), Wire.read());
	  utils.channel_switch(0);
	  a = a & 0b1111111110000000;
	  a = (a >> 7);
	  if (a < 256)
	  {
		  a = a / 2;
	  }
	  else
	  {
		  a = (a - 512) / 2;
	  }
	  return a;
    
    
    
  }

  boolean O2_water(int num)
  {
	  utils.channel_switch(AD7828_3::bus);
	  switch (num)
	  {
	  case 1:
	  {
		  utils.I2C(AD7828_3::adress, 0xec);
		  // delay(1);
		  Wire.beginTransmission(AD7828_3::adress);
		  Wire.requestFrom(AD7828_3::adress, byte(2));
		  volt = ((Wire.read() << 8) + Wire.read());
		  Wire.endTransmission();
		  /*Serial.write("1: ");
		  Serial.print(volt);*/

		  if (volt > 1500) {
			  return false;
		  }
		  else {
			  return true;
		  }
	  }
	  case 2:
	  {
		  utils.I2C(AD7828_3::adress, 0xbC);
		  // delay(1);
		  Wire.beginTransmission(AD7828_3::adress);
		  Wire.requestFrom(AD7828_3::adress, byte(2));
		  volt = ((Wire.read() << 8) + Wire.read());

		  Wire.endTransmission();
		 /* Serial.write(" ----- 2: ");
		  Serial.println(volt);*/


		  if (volt > 2500) {
			  return false;
		  }
		  else {
			  return true;
		  }
	  }
	  }
	  utils.channel_switch(0);
  }
  boolean H2_water(int num)
  {
	  utils.channel_switch(AD7828_3::bus);
    switch (num)
    {
    case 1:
      { 
        utils.I2C(AD7828_3::adress,0xac);
        // delay(1);
        Wire.beginTransmission(AD7828_3::adress);
        Wire.requestFrom(AD7828_3::adress, byte(2));
       volt = ((Wire.read() << 8) + Wire.read());
        Wire.endTransmission();
		/*Serial.write("1: ");
         Serial.print(volt);*/
         
        if (volt > 1600) {
          return false;
        } 
        else {
          return true;
        }
      }
    case 2:
      { 
        utils.I2C(AD7828_3::adress,0xdC);
        // delay(1);
        Wire.beginTransmission(AD7828_3::adress);
        Wire.requestFrom(AD7828_3::adress, byte(2));
        volt = ((Wire.read() << 8) + Wire.read());

        Wire.endTransmission();
		/*Serial.write(" ----- 2: ");
		Serial.println(volt);*/
		

        if (volt > 2000) {
          return false;
        } 
        else {
          return true;
        }
      }
    }   
	utils.channel_switch(0);
  }
  boolean CH_water(int num)
  {
    switch (num)
    {
    case 1:
      { 
        utils.I2C(0x00,0xFC);
        // delay(1);
        Wire.beginTransmission(0x00);
        Wire.requestFrom(0x00, 2);
        volt = ((Wire.read() << 8) + Wire.read());
        Wire.endTransmission();
        Serial.write("1:");
        Serial.println(volt);
        if (volt > 4000) {
          return false;
        } 
        else {
          return true;
        }
      }
    case 2:
      { 
        utils.I2C(0x00,0xBC);
        // delay(1);
        Wire.beginTransmission(0x00);
        Wire.requestFrom(0x00, 2);
        volt = ((Wire.read() << 8) + Wire.read());
        Wire.endTransmission();
        Serial.write("2:");
        Serial.println(volt);

        if (volt > 3000) {
          return false;
        } 
        else {
          return true;
        }
      }
    }   
  }
  
  int CO2_percentage()
  {
    return 20;
  }
  int H2_percentage()
  {
    return 80;
  }
  int CO2_pressure()
  {
    Wire.beginTransmission(0x00);
    Wire.requestFrom(0x00, 24);
    //------------------------
	for (int i = 0; i <= 23; i++)
	{
		AD[i] = Wire.read();
	}
    Wire.endTransmission(0x00);
    int output = word(AD[18], AD[19]);
    output = output & 0x0FFF;
    float fout = output;
    double p = (((fout / 5000) - 0.04) / 0.009)*10;
    return p;
  }
  

};
//########################################################################################################################################\\ 

class Lights 
{
  Utils utils;

public:
	
	void test()
	{
		pinMode(13, OUTPUT);
		digitalWrite(13, HIGH);
		delay(1000);
	    digitalWrite(13, LOW);

	}
  void Caution(boolean state) 
  {

    if (state)
    {
      utils.I2CWRITE_M(0x00,2,1); 
    }
    else
    {
      utils.I2CWRITE_M(0x00,2,0); 
    }

  }

  void Running(boolean state) 
  {

    if (state)
    {

      utils.I2CWRITE_M(0x00,0,1); //A4
    }
    else
    {
      utils.I2CWRITE_M(0x00,0,0); //A4
    }
  }

  void Ready(boolean state) 
  {
    if (state)
    {
      utils.I2CWRITE_M(0x00,1,1); 
    }
    else
    {
      utils.I2CWRITE_M(0x00,1,0); 
    }
  }

  void Starting(boolean state) 
  {
    if (state)
    {
      utils.I2CWRITE_M(0x00,1,1);  //A4+A2
      utils.I2CWRITE_M(0x00,0,1);  //A4+A2
    }
    else
    {
      utils.I2CWRITE_M(0x00,1,0);  //A4+A2
      utils.I2CWRITE_M(0x00,0,0);//A4+A2
    }
  }
}; 


class Valves
{
	Sys_stat sys_stat;
  Sensors sensors;
  Utils utils;
public:
	//<Magnetic Valves
  void CO2(boolean state) //V2_007
  {
	  utils.channel_switch(PCF8574_2::bus);
    if (state)
    {
		
		utils.I2CWRITE(PCF8574_2::adress, 0, 1);
    }
    else
    {
		
		utils.I2CWRITE(PCF8574_2::adress, 0, 0);
		
    }
	utils.Send_to_GUI(STT_CO2VALVE, sys_stat.CO2());
	utils.channel_switch(0);
  } 

  void H2_in(boolean state) //V2_002
  {
	  utils.channel_switch(1);

    if (state)
    {
		utils.I2CWRITE_M(PCF8575::adress, 12, 1);
    }
    else
    {
      utils.I2CWRITE_M(PCF8575::adress,12,0); 
    }
	utils.Send_to_GUI(STT_H2INVALVE, sys_stat.H2_in());
	utils.channel_switch(0);

  } 

  void H2_out(boolean state) //V2_004
  {
	  utils.channel_switch(1);
	  if (state)
	  {
		  utils.I2CWRITE(PCF8574::adress, 4, 1); 
	  }
	  else
	  {
		  utils.I2CWRITE(PCF8574::adress, 4, 0);
	  }
	  utils.Send_to_GUI(STT_H2OUTVALVE, sys_stat.H2_out());
	  utils.channel_switch(0);
  }

  void O2_in(boolean state) //V2_003
  {
	  utils.channel_switch(1);
	  if (state)
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 13, 1); 
	  }
	  else
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 13, 0); 
	  }
	  utils.Send_to_GUI(STT_O2INVALVE,sys_stat.O2_in());
	  utils.channel_switch(0);
  }


  void CH_Water(boolean state) //V2_003
  {
	  utils.channel_switch(1);
	  if (state)
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 11, 1);
	  }
	  else
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 11, 0);
	  }
	  utils.Send_to_GUI(STT_CHWATER, sys_stat.CH_water());
	  utils.channel_switch(0);
  }

  void O2_out(boolean state) //V2_005
  {
	  utils.channel_switch(1);
	  if (state)
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 16, 1); 
	  }
	  else
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 16, 0); 
	  }
	  utils.Send_to_GUI(STT_O2OUTVALVE, sys_stat.O2_out());
	  utils.channel_switch(0);
  }

  void Water_reflux_H2(boolean state) //V2_001
  {
	  utils.channel_switch(1);
    if (state)
    {
      utils.I2CWRITE_M(PCF8575::adress,10,1); 
    }
    else
    {
      utils.I2CWRITE_M(PCF8575::adress,10,0); 
    }
	utils.Send_to_GUI(STT_H2REFLUX, sys_stat.Water_reflux_H2());
	utils.channel_switch(0);
  } 

  void Water_reflux_O2(boolean state) //V2_000
  {
	  utils.channel_switch(1);
	  if (state)
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 11, 1); 
	  }
	  else
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 11, 0); 
	  }
	  utils.Send_to_GUI(STT_O2REFLUX, sys_stat.Water_reflux_O2());
	  utils.channel_switch(0);
  }

  void Flush(boolean state) //V3_000
  {
	  utils.channel_switch(1);
	  if (state)
	  {
		  utils.I2CWRITE(PCF8574_2::adress, 4, 1);
	  }
	  else
	  {
		  utils.I2CWRITE(PCF8574_2::adress, 4, 0);
	  }
	  utils.Send_to_GUI(STT_FLUSH, sys_stat.Flush());
	  utils.channel_switch(0);
  }
  //>Magnetic Valves

  //<Needle Valves
  boolean H2_Flowrate_step(int val, int dir = 3)//Y
  {
	  boolean returnval;
	  utils.channel_switch(PCF8574::bus);
	  if (val >= 0 && val <= 100)
	  
		  const int bot = 520;
		  const int top = 1290;
		  val = 8.1*val + 480;
		  //map(val, 0, 100, bot, top);
			  if (dir == 3)
			  {
				  if (abs(sensors.H2_outvalve() - val) > 50)
				  {
					   returnval = false;
					  if (sensors.H2_outvalve() < val)
					  {
						  utils.I2CWRITE(PCF8574::adress, 1, open);
					  }
					  else if (sensors.H2_outvalve() > val)
					  {
						  utils.I2CWRITE(PCF8574::adress, 1, close);
					  }
					  utils.I2CWRITE(PCF8574::adress, 2, 0);
					  utils.I2CWRITE(PCF8574::adress, 2, 1);
				  }
				  else
				  {
					   returnval = true;
				  }
			  }
			  else
			  {
				  if (dir == open)
				  {
					  utils.I2CWRITE(PCF8574::adress, 1, open);
					  utils.I2CWRITE(PCF8574::adress, 2, 0);
					  utils.I2CWRITE(PCF8574::adress, 2, 1);
				  }
				  else
				  {
					  utils.I2CWRITE(PCF8574::adress, 1, close);
					  utils.I2CWRITE(PCF8574::adress, 2, 0);
					  utils.I2CWRITE(PCF8574::adress, 2, 1);
				  }
			  }
			  utils.channel_switch(0);
			  return returnval;
  }
  void CO2_needle_reset(int perc)
  {
	  while (!CO2_Flowrate_step(perc));
  }
  void H2_needle_reset(int perc)
  {
	  while (!H2_Flowrate_step(perc));
  }
  boolean CO2_Flowrate_step(int val, int dir = 3)//X
  {
	  boolean returnval;
	  utils.channel_switch(PCF8574::bus);
	  if (val >= 0 && val <= 100)
	  {
		  const int bot = 780;
		  const int top = 1420;
		  val = 6.4*val + 780.0;
		  //map(val, 0, 100, bot, top);
		  if (dir == 3)
		  {
			  if (abs(sensors.CO2_valve() - val) > 15)
			  {
				  returnval = false;
				  if (sensors.CO2_valve() < val)
				  {
					  utils.I2CWRITE(PCF8574_2::adress, 1, open);
				  }
				  else if (sensors.CO2_valve() > val)
				  {
					  utils.I2CWRITE(PCF8574_2::adress, 1, close);
				  }
				  utils.I2CWRITE(PCF8574_2::adress, 2, 0);
				  utils.I2CWRITE(PCF8574_2::adress, 2, 1);
			  }
			  else
			  {
				  returnval = true;
			  }
		  }
		  else
		  {
			  if (dir == open)
			  {
				  utils.I2CWRITE(PCF8574_2::adress, 1, open);
				  utils.I2CWRITE(PCF8574_2::adress, 2, 0);
				  utils.I2CWRITE(PCF8574_2::adress, 2, 1);
			  }
			  else
			  {
				  utils.I2CWRITE(PCF8574_2::adress, 1, close);
				  utils.I2CWRITE(PCF8574_2::adress, 2, 0);
				  utils.I2CWRITE(PCF8574_2::adress, 2, 1);
			  }
		  }
	  }
	  utils.channel_switch(0);
	  return returnval;
  }
  //>Needle Valves
};

class Items
{
	Sys_stat sys_stat;
  Utils utils;
public:
	
    
  
  void Fan(boolean state)
  {
	  utils.channel_switch_2(PSW_PCF8575::bus);
    if (state)
    {
	 utils.I2CWRITE_M(PSW_PCF8575::adress,0,0); //5V open
     


    }
    else
    {
      utils.I2CWRITE_M(PSW_PCF8575::adress,0,1);
    } 
	utils.channel_switch_2(0);
  }

  void Fuelcell(boolean state)
  {
	  utils.channel_switch_2(PCF8575::bus);
	  if (state)
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 7, 1); //GND open



	  }
	  else
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 7, 0);
	  }
	  utils.channel_switch_2(0);
  }

  void Peltier(boolean state)
  {
	  utils.channel_switch_2(PSW_PCF8575::bus);
	  if (state)
	  {
		  utils.I2CWRITE_M(PSW_PCF8575::adress, 6, 0); //5V open



	  }
	  else
	  {
		  utils.I2CWRITE_M(PSW_PCF8575::adress, 6, 1);
	  }
	  utils.channel_switch_2(0);
  }

  void Electrolyzer_1(boolean state)
  {
	  utils.channel_switch(PCF8575::bus);
    if (state)
    {
      utils.I2CWRITE_M(PCF8575::adress,5,1);
    }
    else
    {
      utils.I2CWRITE_M(PCF8575::adress,5,0);
    }
	utils.channel_switch(0);
  }

  void Electrolyzer_2(boolean state)
  {
	  utils.channel_switch(PCF8575::bus);

	  if (state)
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 6, 1);
	  }
	  else
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 6, 0);
	  }
	  utils.channel_switch(0);

  }

  void Oven(boolean state)
  {
	  utils.channel_switch_2(PSW_PCF8575::bus);
	  if (state)
	  {
		  utils.I2CWRITE_M(PSW_PCF8575::adress, 5, 1); //5V open



	  }
	  else
	  {
		  utils.I2CWRITE_M(PSW_PCF8575::adress, 5, 0);
	  }
	  utils.channel_switch_2(0);
  }

  void Pump(boolean state)
  {
	  utils.channel_switch(1);
	  if (state)
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 14, 1);
	  }
	  else
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 14, 0);
	  }
	  utils.Send_to_GUI(STT_PUMP, sys_stat.Pump());
	  utils.channel_switch(1);
  }


};




//########################################################################################################################################\\  

class ComputerControlledMethanReactor
{
  
public:
	Utils utils;
  void init(boolean Flush) 
  {
    Serial.begin(9600);
	Serial2.begin(9600);
    



    Wire.begin();
	
  
	//INIT
	Serial.println("-initializing-");
	utils.channel_switch(1);
	utils.I2C(PCF8574::adress, 0xDF);
	utils.I2C(PCF8574_2::adress, 0xDF);
	utils.I2C(MAX1238::adress, 0x17);
	utils.I2C(MAX1238::adress, 0XD2);
	utils.write16(0xFF, 0xFF, PCF8575::adress);
	utils.write8(0xFF, PCF8575::adress);
	utils.channel_switch(0);
	utils.channel_switch_2(2);
	utils.write16(0b11100000, 0xFF, PSW_PCF8575::adress);
	utils.channel_switch_2(0);
	// bus 
	//END INIT
    Serial.println("\nCCMR program initialized");
	Serial.println("registering Serial --> GUI...");
		Serial2.begin(9600);

		utils.Send_to_GUI(STT_CO2VALVE, sys_stat.CO2());
		delay(5);
		utils.Send_to_GUI(STT_H2INVALVE, sys_stat.H2_in());
		delay(5);
		utils.Send_to_GUI(STT_H2OUTVALVE, sys_stat.H2_out());
		delay(5);
		utils.Send_to_GUI(STT_O2INVALVE, sys_stat.O2_in());
		delay(5);
		utils.Send_to_GUI(STT_O2OUTVALVE, sys_stat.O2_out());
		delay(5);
		utils.Send_to_GUI(STT_H2REFLUX, sys_stat.Water_reflux_H2());
		delay(5);
		utils.Send_to_GUI(STT_O2REFLUX, sys_stat.Water_reflux_O2());
		delay(5);
		utils.Send_to_GUI(STT_PUMP, sys_stat.Pump());
		delay(5);
		utils.Send_to_GUI(SNS_ovenTemp_deg, sensors.Oven_temp());
		delay(5);
		utils.Send_to_GUI(SNS_H2water_val, sensors.H2_water(1) ? (sensors.H2_water(2) ? 2 : 1) : 0);//0=0;1=1;2=1&2
		delay(5);
		utils.Send_to_GUI(SNS_CHwater_val, sensors.CH_water(1) ? (sensors.CH_water(2) ? 2 : 1) : 0);//0=0;1=1;2=1&2
		delay(5);
		utils.Send_to_GUI(SNS_O2storagepressure_mbar, sensors.O2_storagepressure());
		delay(5);
		utils.Send_to_GUI(SNS_H2Storagepressure_mbar, sensors.H2_storagepressure());
		delay(5);
		utils.Send_to_GUI(SNS_CO2pressure_mbar, sensors.CO2_pressure());
		delay(5);
		utils.Send_to_GUI(SNS_H2valve_perc, sensors.H2_outvalve());
		delay(5);
		utils.Send_to_GUI(SNS_CO2valve_perc, sensors.CO2_valve());
		delay(5);
		utils.Send_to_GUI(SNS_O2electrolyzerpressure_mbar, sensors.O2_pressure_Electrolyzer());
		delay(5);
		utils.Send_to_GUI(SNS_H2electrolyzerpressure_mbar, sensors.H2_pressure_Electrolyzer());
		delay(5);
		utils.Send_to_GUI(SNS_O2water_val, sensors.O2_water(1) ? (sensors.O2_water(2) ? 2 : 1) : 0);//0=0;1=1;2=1&2


		Serial.println("GUI Link active");
    if (debug)
    {
      Serial.write(" [DEBUG MODE]\n") ;
    }
    else
    {
      Serial.write("\n") ;
    }
    delay(1000);

	_init = true;
	utils.write16(0xFF, 0xFF, PCF8575::adress);
	_init = false;

	if (Flush)
	{
		Serial.println("--Booting--");
		Serial.println("RESETTING VALVES");
		valves.CO2_needle_reset(100);
		delay(500);
		valves.H2_needle_reset(100);
		Serial.println("refluxing water.."); //TODO: ventil nach dem ofen!!!!!!!
		valves.Flush(true);
		valves.Water_reflux_H2(true);
		delay(2000);
		valves.Flush(false);
		valves.Water_reflux_H2(false);
		Serial.print("Cleaning Valves and Pipes");
		valves.Flush(true);
		valves.H2_in(true);
		valves.H2_out(true);
		delay(1000);
		Serial.print("<|..");
		delay(1000);
		Serial.print(".");
		delay(1000);
		Serial.print(".");
		delay(1000);
		Serial.print(".");
		delay(1000);
		Serial.print(".");
		delay(1000);
		Serial.print(".");
		delay(1000);
		Serial.print(".");
		delay(1000);
		Serial.print(".");
		delay(1000);
		Serial.print(".");
		delay(1000);
		Serial.print("..|>");
		//valves.H2(false);
		valves.Flush(false);
		valves.H2_in(false);
		valves.H2_out(false);
		Serial.println(" COMPLETE");
		delay(200);
		Serial.println("SETTING STANDART MIX...");
		Serial.println("CO2...");
		valves.CO2_needle_reset(CO2_reset);
		Serial.println("H2...");
		valves.H2_needle_reset(H2_reset);
		Serial.println("SYSTEM BOOTUP COMPLETE");
		Serial.println("USER INPUT REQUIRED!");
		Serial.println("PREHEAT OVEN ?");
		bool in = false;
		bool heat = false;
		if (!debug)
		{
			while (!in)
			{
				char read = Serial.read();
				if (read == 121)
				{
					in = true;
					heat = true;
				}
				if (read == 110)
				{
					in = true;
					heat = false;
				}
				delay(10);
			}
			if (heat)
			{
				Serial.println("PREHEATING OVEN... (this may take a while)");
				delay(1000);
				items.Oven(true);
				while (sensors.Oven_temp() < 300)
				{
					int perc = 300 / 100 * sensors.Oven_temp();
					Serial.print("Preheating.. || ");
					Serial.print(perc);
					Serial.print("% Finished");
					Serial.print(" || Temperature: ");
					Serial.print(sensors.Oven_temp());  //-> Optional
					Serial.println("°C");
					delay(1000);
				}
				items.Oven(false);
			}
			else
			{
				Serial.println("PREHEATING ABORTED");
			}
			Serial.println("PREPRODUCING H2");
		/*	valves.CO2_needle_idle();
			valves.H2_needle_idle();*/
				items.Electrolyzer_1(true);
				items.Electrolyzer_2(true);
				while (sensors.H2_pressure_Electrolyzer() < 500)
				{
					delay(2000);
					Serial.print("Pressure: ");
						Serial.print(sensors.H2_pressure_Electrolyzer());
					Serial.println(" mbar");
				}
				items.Electrolyzer_1(false);
				items.Electrolyzer_2(false);
		}
	}
	
    delay(500);
    Serial.write("\n\n----------READY TO BURN----------\n");
   // lights.Starting(false);
   // lights.Ready(true);
    Ready=true;
    delay(100);
  }





public:

  Lights lights;
  
  
  Items items;
  
  

  Sys_stat sys_stat;

  
  
	

	 
	  Valves valves;
	  Sensors sensors;
};







