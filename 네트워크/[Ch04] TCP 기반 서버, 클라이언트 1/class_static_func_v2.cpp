#include <stdio.h>

class MyClass
{
	public:
		int a = 100;
		static int b;

	public:
		static void printClass()
		{
			printf("===== ClassFunc() =====\n");
		}
		void increment_A()
		{
			a++;
		}
		static void increment_B()
		{
			b++;
		}
		int getCount_A()
		{
			return a;
		}
		static int getCount_B()
		{
			return b;
		}
};

int MyClass::b = 100;

int main()
{
	MyClass::printClass();

	MyClass myClass;
	myClass.printClass();
	printf("myClass.a : %d\n", myClass.a);
	return 0;
}
