#include <stdio.h>
#include <stdlib.h>

int main()
{
	int size = 10;
	void* p1 = malloc(sizeof(size)); //123
	void* p2 = p1; // 123
	void** p3 = &p1;

	p1 = (char*)p1+size; //133

	printf("p1: %p:(%p) p2: %p:(%p) p3: %p:(%p)\n",&p1, p1, &p2,p2, &p3, *p3); // 133 123 133

	//free(p1);
	free(p2);
	//free(p3);
	return 0;
}
