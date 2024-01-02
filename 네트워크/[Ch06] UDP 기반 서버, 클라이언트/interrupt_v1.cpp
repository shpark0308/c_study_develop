#include <stdio.h>

class InterruptHandler
{
	public:
		virtual void handleInterrupt() = 0;
};

class HardwareDevice
{

	private:
		InterruptHandler* handler;

	public:
		void setInterruptHandler(InterruptHandler* handler_)
		{
			handler = handler_;
		}

		void simulateInterrupt()
		{
			handler->handleInterrupt();
		}
};

class MyInterruptHandler : public InterruptHandler{
	public:
		void handleInterrupt() override
		{
			printf("Interrupt handled!\n");
		}
};
int main()
{
	MyInterruptHandler myHandler;
	HardwareDevice myDevice;

	myDevice.setInterruptHandler(&myHandler);

	myDevice.simulateInterrupt();
	return 0;
}
