#pragma once
#include "flash_memory_device.h"

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice* hardware);
    virtual int read(long address);
    virtual void write(long address, int data);

private:
    const int READ_CHECK_COUNT = 4;
    void CheckReadDataValidity(long address, int firstData);
protected:
    FlashMemoryDevice* m_hardware;
};