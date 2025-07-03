#include <stdexcept>
#include "device_driver.h"

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}

int DeviceDriver::read(long address)
{
	int firstData = (int)(m_hardware->read(address));
	for (int i = 0; i < 5; ++i) {
		int data = (int)(m_hardware->read(address));
		if (data != firstData) {
			throw std::runtime_error("readFailException");
		}
	}
	// If the data is consistent, return the first read value
    return firstData;
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}