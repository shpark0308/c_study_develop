#include <stdio.h>
#include <stdlib.h>

class MemoryPool
{

	public:
		MemoryPool()
		{
			Block* block1 = (Block*) malloc(block_size);
			printf("block1: %p\n", block1);

			Block* block2 = (Block*) (block1+block_size);
			printf("block2: %p\n", block2);

			int size = 10;
			Block* block3 = (Block*) (block1+size);
			printf("block3: %p\n", block3);
		}
		~MemoryPool(){}
	private:
		struct Block{
			struct Block* next;
		};

	private:
		int block_size = 10;
};

int main()
{
	//MemoryPool memoryPool;

	void* double_v1 = (double*)malloc(sizeof(double));
	void* double_v2 = (double*)double_v1+sizeof(double);
	void* double_v3 = (char*)double_v1+sizeof(double);
	void* double_v4 = (double*)((char*)double_v1+sizeof(double));

	printf("[1]. double_v1: %p\n", double_v1);
	printf("[2]. double_v2: %p\n", double_v2);
	printf("[3]. double_v3: %p\n", double_v3);
	printf("[4]. double_v4: %p\n", double_v4);
	return 0;
}
