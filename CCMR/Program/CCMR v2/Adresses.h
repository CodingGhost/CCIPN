#pragma once

static struct PCF8575
{
	byte adress = 0x21;
	int bus = 1;
};
struct MAX1238
{
	static byte adress = 0x35;
	int bus = 1;
};
struct AD7828
{
	static byte adress = 0x48;
	int bus = 1;
};
struct TMP100
{
	static byte adress = 0x48;
	int bus = 2;
};
struct PCA9545
{
	static byte adress = 0x70;
	int bus = 0;
};
struct MAX5815
{
	static byte adress = 0x21;
	int bus = 1;
};
struct HTY_1
{
	static byte adress = 0x28;
	int bus = 1;
};
struct HTY_2
{
	static byte adress = 0x28;
	int bus = 2;
};
struct SM5812
{
	static byte adress = 0x90;
	int bus = 1;
};
struct SM5812
{
	static byte adress = 0x90;
	int bus = 2;
};
struct CVAI
{
	static byte adress = 0x10;
	int bus = 1;
};
