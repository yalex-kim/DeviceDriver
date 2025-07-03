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

	EXPECT_THROW(driver.read(0xFF), ReadFailException);
}

TEST_F(DeviceDriverTest, ReadFromHWPass) {
	EXPECT_CALL(hardware, read(_))
		.Times(5)
		.WillRepeatedly(Return(0xAB));

	int data = driver.read(0xFF);
	EXPECT_EQ(0xAB, data);
}

TEST_F(DeviceDriverTest, WriteHWAlreadyWrittenException) {
	EXPECT_CALL(hardware, read(_))
		.WillRepeatedly(Return(0xDD));

	EXPECT_CALL(hardware, write(_, _))
		.WillRepeatedly(Return());

	EXPECT_THROW(driver.write(0x1, 0xDD), WriteFailException);

}

TEST_F(DeviceDriverTest, WriteHWPass) {
	EXPECT_CALL(hardware, read(_))
		.WillRepeatedly(Return(0xFF));

	EXPECT_CALL(hardware, write(_, _))
		.WillRepeatedly(Return());

	driver.write(0x1, 0xAB);
}


int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}