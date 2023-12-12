#include <stdio.h>

class MathOperation
{
	public:
		static int a;
	public:
		static int add(int num1, int num2)
		{	
			return num1+num2;
		}
		static int multiply(int num1, int num2)
		{
			return num1*num2;
		}
		void increment()
		{
			a++;
		}
		static int getCount()
		{
			return a;
		}
};
int MathOperation::a = 0;
int main()
{
	MathOperation mathOp, mathOp2;
	printf("Class : (%d) (%d)\n", mathOp.add(3,4),mathOp.multiply(3,4));

	printf("Not Class : (%d) (%d)\n", MathOperation::add(3,4), MathOperation::multiply(3,4));


	//MathOperation::increment(); 
	mathOp.increment();
	printf("a : (%d)\n", mathOp.getCount());
	printf("a : (%d)\n", MathOperation::getCount());

	printf("a2 : (%d)\n", mathOp2.getCount());
        printf("a : (%d)\n", MathOperation::getCount());
	return 0;
}
