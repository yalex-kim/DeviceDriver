#include "gmock/gmock.h"
#include "device_driver.h"

class MockFlashMemoryDevice : public FlashMemoryDevice {
	public:
	MOCK_METHOD(unsigned char, read, (long address), ());
	MOCK_METHOD(void, write, (long address, unsigned char data), ());
};

TEST(DeviceDriver, ReadFromHW) {
	MockFlashMemoryDevice hardware;
	DeviceDriver driver{ &hardware };

	EXPECT_CALL(hardware, read(0xFF))
		.WillOnce(testing::Return(0xAB));

	int data = driver.read(0xFF);
	EXPECT_EQ(0xAB, data);
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}