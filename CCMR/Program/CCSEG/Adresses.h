#pragma once

struct PCF8575
{
	static byte adress;
	static int bus;
};
byte PCF8575::adress = 0x21;
int PCF8575::bus = 1;
struct MAX1238
{
	static byte adress;
	static int bus;
};
byte MAX1238::adress = 0x35;
int MAX1238::bus = 1;
struct AD7828
{
	static byte adress;
	static int bus;
};
byte AD7828::adress = 0x4B;
int AD7828::bus = 0;
struct AD7828_2
{
	static byte adress;
	static int bus;
};
byte AD7828_2::adress = 0x4A;
int AD7828_2::bus = 0;
struct AD7828_3
{
	static byte adress;
	static int bus;
};
byte AD7828_3::adress = 0x48;
int AD7828_3::bus = 1;
struct TMP100
{
	static byte adress;
	static int bus;
};
byte TMP100::adress = 0x48;
int TMP100::bus = 2;
struct PCA9545
{
	static byte adress;
	static int bus;
};
byte PCA9545::adress = 0x70;
int PCA9545::bus = 0;
struct PCA9545_2
{
	static byte adress;
	static int bus;
};
byte PCA9545_2::adress = 0x71;
int PCA9545_2::bus = 0;
struct MAX5815
{
	static byte adress;
	static int bus;
};
byte MAX5815::adress = 0x21;
int MAX5815::bus = 1;
struct HTY_1
{
	static byte adress;
	static int bus;
};
byte HTY_1::adress = 0x28;
int HTY_1::bus = 1;
struct HTY_2
{
	static byte adress;
	static int bus;
};
byte HTY_2::adress = 0x28;
int HTY_2::bus = 2;
struct SM5812
{
	static byte adress;
	static int bus;
};
byte SM5812::adress = 0x90;
int SM5812::bus = 1;
struct PCF8574
{
	static byte adress;
	static int bus;
};
byte PCF8574::adress = 0x26;
int PCF8574::bus = 0;
struct CVAI
{
	static byte adress;
	static int bus;
};
byte CVAI::adress = 0x10;
int CVAI::bus = 1;
struct PCF8574_2
{
	static byte adress;
	static int bus;
};
byte PCF8574_2::adress = 0x25;
int PCF8574_2::bus = 0;