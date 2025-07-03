#include <stdexcept>
#include "device_driver.h"

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}

int DeviceDriver::read(long address)
{
	int firstData = (int)(m_hardware->read(address));
	CheckReadDataValidity(address, firstData);
	// If the data is consistent, return the first read value
    return firstData;
}

void DeviceDriver::write(long address, int data)
{
	int readData = (int)(m_hardware->read(address));
	if (readData != 0xFF) {
		throw std::runtime_error("WriteFailException");
	}
	m_hardware->write(address, (unsigned char)data);
}


void DeviceDriver::CheckReadDataValidity(long address, int firstData)
{
	for (int i = 0; i < READ_CHECK_COUNT; ++i) {
		int data = (int)(m_hardware->read(address));
		if (data != firstData) {
			throw std::runtime_error("ReadFailException");
		}
	}
}