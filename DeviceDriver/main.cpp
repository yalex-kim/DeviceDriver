#include "gmock/gmock.h"
#include "device_driver.h"
#include <stdexcept>

using namespace testing;

class MockFlashMemoryDevice : public FlashMemoryDevice {
	public:
	MOCK_METHOD(unsigned char, read, (long address), ());
	MOCK_METHOD(void, write, (long address, unsigned char data), ());
};


class DeviceDriverTest : public ::testing::Test {
public:
	MockFlashMemoryDevice hardware;
	DeviceDriver driver{ &hardware };
};

TEST_F(DeviceDriverTest, ReadFromHWFailException) {
	EXPECT_CALL(hardware, read(_))
		.WillOnce(Return(0xAB))
		.WillRepeatedly(Return(0xAC));

	// custom exception type is expected : readFailException
	EXPECT_THROW(driver.read(0xFF), std::runtime_error);
}

TEST_F(DeviceDriverTest, ReadFromHWPass) {
	EXPECT_CALL(hardware, read(_))
		.Times(5)
		.WillRepeatedly(Return(0xAB));

	try {
		int data = driver.read(0xFF);
		EXPECT_EQ(0xAB, data);
		
	} 
	catch (const std::exception& e) {
		FAIL() << "Exception thrown: " << e.what();
	}
}

TEST_F(DeviceDriverTest, WriteHWAlreadyWrittenException) {
	EXPECT_CALL(hardware, read(_))
		.WillRepeatedly(Return(0xDD));

	EXPECT_CALL(hardware, write(_, _))
		.WillRepeatedly(Return());

	EXPECT_THROW(driver.write(0x1, 0xDD), std::runtime_error);

}

TEST_F(DeviceDriverTest, WriteHWPass) {
	EXPECT_CALL(hardware, read(_))
		.WillRepeatedly(Return(0xFF));

	EXPECT_CALL(hardware, write(_, _))
		.WillRepeatedly(Return());

	try {
		driver.write(0x1, 0xAB);
	} 
	catch (const std::exception& e) {
		FAIL() << "Exception thrown: " << e.what();
	}
}


int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}