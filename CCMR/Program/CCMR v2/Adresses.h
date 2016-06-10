#pragma once

struct PCF8575
{
	byte adress = 0x21;
	int bus = 1;
};
struct MAX1238
{
	byte adress = 0x35;
	int bus = 1;
};
struct AD7828
{
	byte adress = 0x48;
	int bus = 1;
};
struct TMP100
{
	byte adress = 0x48;
	int bus = 2;
};
struct PCA9545
{
	byte adress = 0x70;
	int bus = 0;
};
struct MAX5815
{
	byte adress = 0x21;
	int bus = 1;
};
struct HTY_1
{
	byte adress = 0x28;
	int bus = 1;
};
struct HTY_2
{
	byte adress = 0x28;
	int bus = 2;
};
struct SM5812
{
	byte adress = 0x90;
	int bus = 1;
};
struct SM5812
{
	byte adress = 0x90;
	int bus = 2;
};
