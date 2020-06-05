#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include <cmath>

#include "KellerLD.h"
//#include "utility.cpp"


#define LD_ADDR                     0x40
#define LD_REQUEST                  0xAC
#define LD_CUST_ID0                 0x00
#define LD_CUST_ID1                 0x01
#define LD_SCALING0                 0x12
#define LD_SCALING1                 0x13
#define LD_SCALING2                 0x14
#define LD_SCALING3                 0x15
#define LD_SCALING4                 0x16


KellerLD::KellerLD() 
{
	fluidDensity = 1029;
	bus = 0;
}


KellerLD::KellerLD(int i2c_bus) 
{
	fluidDensity = 1029;
	bus = i2c_bus;
}


void KellerLD::init()
{
	char buf[16];

	sprintf(buf, "/dev/i2c-%d", bus);
	
	if ((fd = open(buf, O_RDWR)) < 0)
	{
		fprintf(stderr, "Failed to open i2c bus /dev/i2c-%d\n", bus);
		exit(1);
	}

	// Initialize Keller LD
	selectDevice(LD_ADDR);

	// Request memory map information
	cust_id0 = readMemoryMap(LD_CUST_ID0);
	cust_id1 = readMemoryMap(LD_CUST_ID1);

	code = (uint32_t(cust_id1) << 16) | cust_id0;
	equipment = cust_id0 >> 10;
	place = cust_id0 & 0b000000111111111;
	file = cust_id1;

	uint16_t scaling0;
	scaling0 = readMemoryMap(LD_SCALING0);

	mode = scaling0 & 0b00000011;
	year = scaling0 >> 11;
	month = (scaling0 & 0b0000011110000000) >> 7;
	day = (scaling0 & 0b0000000001111100) >> 2;

	uint32_t scaling12 = (uint32_t(readMemoryMap(LD_SCALING1)) << 16) | readMemoryMap(LD_SCALING2);

	P_min = *reinterpret_cast<float*>(&scaling12);

	uint32_t scaling34 = (uint32_t(readMemoryMap(LD_SCALING3)) << 16) | readMemoryMap(LD_SCALING4);

	P_max = *reinterpret_cast<float*>(&scaling34);

}




void KellerLD::setFluidDensity(float density) {
	fluidDensity = density;
}


void KellerLD::readData() {
	uint8_t status;
	char buf[6];
	buf[0] = LD_REQUEST;

	if ((write(fd, buf, 1)) != 1)
	{
		fprintf(stderr, "Error writing to Keller LD\n");
		exit(1);
	}

	usleep(9000); // Max conversion time per datasheet

	if (read(fd, buf, 5) != 5)
	{
		fprintf(stderr, "Error reading from Keller LD\n");
		exit(1);
	}
	else 
	{
		status = buf[0];
		P = buf[1] << 8 | buf[2];
		uint16_t T = (buf[3] << 8) | buf[4];
		P_bar = (float(P)-16384)*(P_max-P_min)/32768 + P_min + 1.01325;
		T_degc = ((T>>4)-24)*0.05-50;
	}
	
}



int KellerLD::selectDevice(uint8_t dev_addr)
{
   int s;

    s = ioctl(fd, I2C_SLAVE, dev_addr);

    if (s == -1)
    {
       perror("selectDevice");
    }

    return s;
}


uint16_t KellerLD::readMemoryMap(uint8_t mtp_address) {
	char buf[4];
	uint8_t status;
	uint16_t memory_map; 

	buf[0] = mtp_address;
	if ((write(fd, buf, 1)) != 1)
	{
		fprintf(stderr, "Error writing to Keller LD\n");
		exit(1);
	}
	usleep(1000);

	if (read(fd, buf, 3) != 3)
	{
		fprintf(stderr, "Error reading from Keller LD\n");
		exit(1);
	}
	else 
	{
		status = buf[0];
		memory_map = buf[1] << 8 | buf[2];
	}
	return memory_map;
}


bool KellerLD::status() {
	if (equipment <= 62 ) {
		return true;
	} else {
		return false;
	}
}


float KellerLD::range() {
	return P_max-P_min;
}


float KellerLD::pressure(float conversion) {
	return P_bar*1000.0f*conversion;
}


float KellerLD::temperature() {
	return T_degc;
}


float KellerLD::depth() {
	return (pressure(KellerLD::Pa)-101325)/(fluidDensity*9.80665);
}


float KellerLD::altitude() {
	return (1-pow((pressure()/1013.25),0.190284))*145366.45*.3048;
}


bool KellerLD::isInitialized() {
	return (cust_id0 >> 10) != 63; // If not connected, equipment code == 63
}
