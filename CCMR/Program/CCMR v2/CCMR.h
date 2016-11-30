#pragma once
#include <HardwareSerial.h>
#include "Arduino.h"
#include "Adresses.h"
boolean debug = true;
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
  void I2CWRITE_M(uint8_t address, uint8_t pin, boolean value)
  {
	  if (!_init)
	  {
	  if (pin <= 4)
	 {
	  pin = NULL;
	 }
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

class Sensors
{
  Utils utils;

public:
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
		Wire.beginTransmission(MAX1238::adress);
		Wire.requestFrom(MAX1238::adress, byte(24));
		//------------------------
		AD[0] = Wire.read();
		AD[1] = Wire.read();
		AD[2] = Wire.read();
		AD[3] = Wire.read();
		AD[4] = Wire.read();
		AD[5] = Wire.read();
		AD[6] = Wire.read();
		AD[7] = Wire.read();
		AD[8] = Wire.read();
		AD[9] = Wire.read();
		AD[10] = Wire.read();
		AD[11] = Wire.read();
		AD[12] = Wire.read();
		AD[13] = Wire.read();
		AD[14] = Wire.read();
		AD[15] = Wire.read();
		AD[16] = Wire.read();
		AD[17] = Wire.read();
		AD[18] = Wire.read();
		AD[19] = Wire.read();
		AD[20] = Wire.read();
		AD[21] = Wire.read();
		AD[22] = Wire.read();
		AD[23] = Wire.read();
		Wire.endTransmission(MAX1238::adress);
		int output = word(AD[6], AD[7]);
		output = output & 0x0FFF;
		double x = output; 
		//Serial.print(output);
		//Serial.print(" degree: ");
		//Serial.println(degree(output));
		return degree(output);

	}
	double degree(double x) {
		return  1.2094505374378798e+001 * pow(x, 0)
			+ 7.2936082213848244e-002 * pow(x, 1)
			+ 1.1321194483966676e-004 * pow(x, 2)
			+ -6.6277232428908262e-008 * pow(x, 3)
			+ 1.2238390800688889e-011 * pow(x, 4); //nicht genau
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
  boolean H2_water(int num)
  {
    switch (num)
    {
    case 1:
      { 
        utils.I2C(AD7828::adress,0xAC);
        // delay(1);
        Wire.beginTransmission(AD7828::adress);
        Wire.requestFrom(AD7828::adress, byte(2));
        volt = ((Wire.read() << 8) + Wire.read());
        Wire.endTransmission();
         //Serial.write("1:");
         //Serial.println(volt);
        if (volt > 4000) {
          return false;
        } 
        else {
          return true;
        }
      }
    case 2:
      { 
        utils.I2C(AD7828::adress,0xDC);
        // delay(1);
        Wire.beginTransmission(AD7828::adress);
        Wire.requestFrom(AD7828::adress, byte(2));
        volt = ((Wire.read() << 8) + Wire.read());
        Wire.endTransmission();
         //Serial.write("2:");
        //Serial.println(volt);

        if (volt > 3000) {
          return false;
        } 
        else {
          return true;
        }
      }
    }   
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



  int H2_valve()
  {
    utils.I2C(0x00,0xFC);
    Wire.beginTransmission(0x00);
    Wire.requestFrom(0x00, 2);
    int val(((Wire.read() << 8) + Wire.read()));
    Wire.endTransmission();
    //Serial.println(val);
    return val;
  }
  int CO2_valve()
  {
    utils.I2C(0x00,0xBC);
    Wire.beginTransmission(0x00);
    Wire.requestFrom(0x00, 2);
    int val = (((Wire.read() << 8) + Wire.read()));
    //Serial.println(val);
    Wire.endTransmission();
    return val;  
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
    AD[0] = Wire.read();
    AD[1] = Wire.read();
    AD[2] = Wire.read();
    AD[3] = Wire.read();
    AD[4] = Wire.read();
    AD[5] = Wire.read();
    AD[6] = Wire.read();
    AD[7] = Wire.read();
    AD[8] = Wire.read();
    AD[9] = Wire.read();
    AD[10] = Wire.read();
    AD[11] = Wire.read();
    AD[12] = Wire.read();
    AD[13] = Wire.read();
    AD[14] = Wire.read();
    AD[15] = Wire.read();
    AD[16] = Wire.read();
    AD[17] = Wire.read();
    AD[18] = Wire.read();
    AD[19] = Wire.read();
    AD[20] = Wire.read();
    AD[21] = Wire.read();
    AD[22] = Wire.read();
    AD[23] = Wire.read();
    Wire.endTransmission(0x00);
    int output = word(AD[18], AD[19]);
    output = output & 0x0FFF;
    float fout = output;
    double p = (((fout / 5000) - 0.04) / 0.009)*10;
    return p;
  }
  int H2_pressure_Electrolyzer()
  {
    Wire.beginTransmission(MAX1238::adress);
    Wire.requestFrom(MAX1238::adress, byte(24));
    //------------------------
    AD[0] = Wire.read();
    AD[1] = Wire.read();
    AD[2] = Wire.read();
    AD[3] = Wire.read();
    AD[4] = Wire.read();
    AD[5] = Wire.read();
    AD[6] = Wire.read();
    AD[7] = Wire.read();
    AD[8] = Wire.read();
    AD[9] = Wire.read();
    AD[10] = Wire.read();
    AD[11] = Wire.read();
    AD[12] = Wire.read();
    AD[13] = Wire.read();
    AD[14] = Wire.read();
    AD[15] = Wire.read();
    AD[16] = Wire.read();
    AD[17] = Wire.read();
    AD[18] = Wire.read();
    AD[19] = Wire.read();
    AD[20] = Wire.read();
    AD[21] = Wire.read();
    AD[22] = Wire.read();
    AD[23] = Wire.read();
    Wire.endTransmission(MAX1238::adress);
    int output = word(AD[16], AD[17]);
    output = output & 0x0FFF;
    float fout = output;
    double p = (((fout / 5000) - 0.04) / 0.009)*10;
    return p;
    //Serial.println(output);
    //Serial.print(AD[16]);
    //Serial.print(":::");
    //Serial.println(AD[17]);
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
  
  Sensors sensors;
  Utils utils;
public:
	//<Magnetic Valves
  void CO2(boolean state)
  {
	  utils.channel_switch(1);
    if (state)
    {
		
			utils.I2CWRITE_M(PCF8575::adress, 17, 1);
    }
    else
    {
		
			utils.I2CWRITE_M(PCF8575::adress, 17, 0);
		
    }


  } 

  void H2_in(boolean state)
  {
	utils.channel_switch(1);
    if (state)
    {
      utils.I2CWRITE_M(PCF8575::adress,12,1); 
    }
    else
    {
      utils.I2CWRITE_M(PCF8575::adress,12,0); 
    }
  } 

  void H2_out(boolean state)
  {
	  utils.channel_switch(1);
	  if (state)
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 15, 1); 
	  }
	  else
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 15, 0); 
	  }
  }

  void O2_in(boolean state)
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
  }

  void O2_out(boolean state)
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
  }

  void Water_reflux_H2(boolean state)
  {
	  utils.channel_switch(1);
    if (state)
    {
      utils.I2CWRITE_M(PCF8575::adress,11,1); 
    }
    else
    {
      utils.I2CWRITE_M(PCF8575::adress,11,0); 
    }
  } 

  void Water_reflux_O2(boolean state)
  {
	  if (state)
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 10, 1); 
	  }
	  else
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 10, 0); 
	  }
  }
  //>Magnetic Valves

  //<Needle Valves
  void H2_Flowrate(int val)
  {
	  utils.I2CWRITE(PCF5874::adress, 3, 1);
	  delay(10);
	  utils.I2CWRITE(PCF5874::adress, 3, 0);
	  delay(10);
	  utils.I2CWRITE(PCF5874::adress, 3, 1);
	  delay(10);
	  utils.I2CWRITE(PCF5874::adress, 3, 0);
	  delay(10);
  }
  //>Needle Valves
};

class Items
{
  Utils utils;
public:
	
    
  
  void Fan(boolean state)
  {
    if (state)
    {
      utils.I2CWRITE_M(0x00,8,1);
    }
    else
    {
      utils.I2CWRITE_M(0x00,8,0);
    } 
  }

  void Peltier(boolean state)
  {
    if (state)
    {

      utils.I2CWRITE_M(0x00,9,1);
    }
    else
    {

      utils.I2CWRITE_M(0x00,9,0);
    } 
  }

  void Electrolyzer_1(boolean state)
  {
    if (state)
    {
      utils.I2CWRITE_M(PCF8575::adress,5,1);
    }
    else
    {
      utils.I2CWRITE_M(PCF8575::adress,5,0);
    }
  }

  void Electrolyzer_2(boolean state)
  {
	  if (state)
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 6, 1);
	  }
	  else
	  {
		  utils.I2CWRITE_M(PCF8575::adress, 6, 0);
	  }
  }

  void Oven(boolean state)
  {
    if (state)
    {
      utils.I2CWRITE_M(0x00,4,1);
      utils.I2CWRITE_M(0x00,6,1); 
    }
    else
    {
      utils.I2CWRITE_M(0x00,4,0);
      utils.I2CWRITE_M(0x00,6,0); 
    }
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
  }


};

class Sys_stat
{
	Utils utils;
public:
	//<Magnetic Valves
	boolean CO2()
	{
		utils.channel_switch(1);
		return utils.I2CREAD_M(PCF8575::adress, 17);
	}

	boolean H2_in()
	{
		utils.channel_switch(1);
		return utils.I2CREAD_M(PCF8575::adress, 12);
	}

	boolean H2_out()
	{
		utils.channel_switch(1);
		return utils.I2CREAD_M(PCF8575::adress, 15);
	}

	boolean O2_in()
	{
		utils.channel_switch(1);
		return utils.I2CREAD_M(PCF8575::adress, 13);
	}

	boolean O2_out()
	{
		utils.channel_switch(1);
		return utils.I2CREAD_M(PCF8575::adress, 16);
	}

	boolean Water_reflux_H2()
	{
		utils.channel_switch(1);
		return utils.I2CREAD_M(PCF8575::adress, 11);
	}

	boolean Water_reflux_O2()
	{
		return utils.I2CREAD_M(PCF8575::adress, 10);
	}
	//>Magnetic Valves


};


//########################################################################################################################################\\  

class ComputerControlledMethanReactor
{
  
public:
	Utils utils;
  void init(boolean Flush) 
  {
    Serial.begin(9600);
    Sw_Reset[0] = 0x51;
    Sw_Reset[1] = 0x00;
    Sw_Reset[2] = 0x00;

    Set_Ref[0] = 0x77;
    Set_Ref[1] = 0x00;
    Set_Ref[2] = 0x00;

    Code_Load[0] = 0x33;
    Code_Load[1] = 0x05;
    Code_Load[2] = 0x00;
    //deleting values



    Wire.begin();
	
    //utils.I2C(addr_Lights,0xff);
    //utils.I2C(addr_Valves2,0xff);
    //utils.I2C(addr_Valves3,0xff);
    //utils.I2C(addr_Valves4,0xff);
    //utils.I2C(addr_Valves4,0xff); 
    //utils.I2C(addr_Items1,0xff);
    //utils.I2C(addr_Items2,0xff);
    //utils.I2C(addr_Items3,0xff);
    //utils.I2C(0x00,0xff);
    //Wire.beginTransmission(addr_Valves);
    //Wire.write(0b11111111);
    //Wire.write(0b11111111);
    //Wire.endTransmission();

    //Wire.beginTransmission(0x00);
    //Wire.write(0b11111111);
    //Wire.write(0b11111111);
    //Wire.endTransmission();

    //utils.I2C(addr_AD,0x17);
    //utils.I2C(addr_AD,0xD2);
    //utils.I2C(addr_RT,0x00);

    //Wire.beginTransmission(addr_DA);
    //Wire.write(Sw_Reset[0]);
    //Wire.write(Sw_Reset[1]);
    //Wire.write(Sw_Reset[2]);
    //Wire.endTransmission();

    //Wire.beginTransmission(addr_DA);
    //Wire.write(Set_Ref[0]);
    //Wire.write(Set_Ref[1]);
    //Wire.write(Set_Ref[2]);
    //Wire.endTransmission();

    //Wire.beginTransmission(addr_DA);
    //Wire.write(Code_Load[0]);
    //Wire.write(Code_Load[1]);
    //Wire.write(Code_Load[2]);
    //Wire.endTransmission();

    //Wire.beginTransmission(addr_AD);
    //Wire.requestFrom(addr_AD, byte(24));
    ////------------------------
    //AD[0] = Wire.read();
    //AD[1] = Wire.read();
    //AD[2] = Wire.read();
    //AD[3] = Wire.read();
    //AD[4] = Wire.read();
    //AD[5] = Wire.read();
    //AD[6] = Wire.read();
    //AD[7] = Wire.read();
    //AD[8] = Wire.read();
    //AD[9] = Wire.read();
    //AD[10] = Wire.read();
    //AD[11] = Wire.read();
    //AD[12] = Wire.read();
    //AD[13] = Wire.read();
    //AD[14] = Wire.read();
    //AD[15] = Wire.read();
    //AD[16] = Wire.read();
    //AD[17] = Wire.read();
    //AD[18] = Wire.read();
    //AD[19] = Wire.read();
    //AD[20] = Wire.read();
    //AD[21] = Wire.read();
    //AD[22] = Wire.read();
    //AD[23] = Wire.read();
    ////--RT--
    //Wire.beginTransmission(addr_RT);
    //Wire.requestFrom(addr_AD, byte(2));
    ////------------------------
    //RT[0] = Wire.read();
    //RT[1] = Wire.read();
    //RTR = RT[0]*16 + RT[1]/16 ;
    //if (RTR < 2048)
    //{
    //  RTR /= 16;
    //} 
    //else
    //{ 
    //  RTR = (RTR - 4096) / 16;
    //}

    //Wire.endTransmission();

	//INIT
	utils.I2C(PCF5874::adress, 0xDF);
	//END INIT
    Serial.println("\nCCMR program initialized");
	Serial.println("registering Serial --> GUI...");
		Serial2.begin(9600);
		Serial.println("Connection to GUI established");
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

		

		lights.Starting(true);
		Serial.println("RESETTING VALVES");
		//valves.CO2_needle_reset(false);
		//delay(500);
		//valves.H2_needle_reset(true);
		//Serial.println("refluxing water.."); //TODO: ventil nach dem ofen!!!!!!!
		//valves.Flush(true);
		//delay(1000);
		//valves.Flush(false);
		//valves.Water_reflux_H2(true);
		//delay(3000);
		//valves.Water_reflux_H2(false);
		//Serial.print("Cleaning Valves and Pipes");
		//valves.Flush(true);
		////valves.H2(true);
		//valves.Water_reflux_CH4(true);
		//delay(1000);
		//Serial.print("<|..");
		//delay(1000);
		//Serial.print(".");
		//delay(1000);
		//Serial.print(".");
		//delay(1000);
		//Serial.print(".");
		//delay(1000);
		//Serial.print(".");
		//delay(1000);
		//Serial.print(".");
		//delay(1000);
		//Serial.print(".");
		//delay(1000);
		//Serial.print(".");
		//delay(1000);
		//Serial.print(".");
		//delay(1000);
		//Serial.print("..|>");
		////valves.H2(false);
		//valves.Flush(false);
		//valves.Water_reflux_CH4(false);
		//Serial.println(" COMPLETE");
		//delay(200);
		//Serial.println("SETTING STANDART MIX...");
		//Serial.println("CO2...");
		//valves.CO2_needle_percent(CO2_reset);
		//Serial.println("H2...");
		//valves.H2_needle_percent(H2_reset);
		//Serial.println("SYSTEM BOOTUP COMPLETE");
		//Serial.println("USER INPUT REQUIRED!");
		//Serial.println("PREHEAT OVEN ?");
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
  
  
  Sensors sensors;

  Sys_stat sys_stat;

  Valves valves;
};







