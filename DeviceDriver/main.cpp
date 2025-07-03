#include "gmock/gmock.h"
#include "device_driver.h"
#include <stdexcept>

using namespace testing;

class MockFlashMemoryDevice : public FlashMemoryDevice {
	public:
	MOCK_METHOD(unsigned char, read, (long address), ());
	MOCK_METHOD(void, write, (long address, unsigned char data), ());
};


TEST(DeviceDriver, ReadFromHWFailException) {
	MockFlashMemoryDevice hardware;
	DeviceDriver driver{ &hardware };

	EXPECT_CALL(hardware, read(_))
		.WillOnce(Return(0xAB))
		.WillRepeatedly(Return(0xAC));

	// custom exception type is expected : readFailException
	EXPECT_THROW(driver.read(0xFF), std::runtime_error);
}

TEST(DeviceDriver, ReadFromHWPass) {
	MockFlashMemoryDevice hardware;
	DeviceDriver driver{ &hardware };

	EXPECT_CALL(hardware, read(_))
		.WillRepeatedly(Return(0xAB));

	try {
		int data = driver.read(0xFF);
		EXPECT_EQ(0xAB, data);
		
	} 
	catch (const std::exception& e) {
		FAIL() << "Exception thrown: " << e.what();
	}
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}