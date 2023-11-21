#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#define DEF_BLOCK_SIZE (15)
#define SUB_SIZE (4)

class MemoryPool
{
	public:
		MemoryPool():offset(0), currentMemory(nullptr) {};
		~MemoryPool();

		void* allocate(int size);
		void deallocate();

		void printBlock();
	private:
		const int BLOCK_SIZE = DEF_BLOCK_SIZE;		// 블록 크기
		std::vector<char*> memoryManager;		// 메모리 블록을 관리하는 벡터
		char* currentMemory;				// 현재 메모리
		int offset;
};

MemoryPool::~MemoryPool()
{
	for (auto iter = memoryManager.begin(); iter != memoryManager.end(); iter++)
	{
		delete[] *iter;
	}
	memoryManager.clear();
	currentMemory = nullptr;
}

void* MemoryPool::allocate(int size)
{
	if (currentMemory == nullptr || offset+size > BLOCK_SIZE)
	{
		// 새롭게 할당
		currentMemory = new char[BLOCK_SIZE]; // 정적 할당
		//memset(currentMemory, 0, BLOCK_SIZE);
		memoryManager.push_back(currentMemory);
		offset = 0;
	}

	void* result = currentMemory + offset;
	offset = offset + size;
	return result;
}

void MemoryPool::printBlock()
{
	printf("======== Block Info ========\n");
	for (int i=0; i<memoryManager.size(); i++)
	{
		printf("<%p>  ", memoryManager[i]);
		printf("[%d]. %s\n", i, memoryManager[i]);
	}
	printf("============================\n");
}

void MemoryPool::deallocate()
{
	if ( memoryManager.empty())
		return;

	printf("[0] index: %s will be erased\n", *memoryManager.begin());
	memoryManager.erase(memoryManager.begin());
}

int main()
{
	MemoryPool memoryPool;

	char* string_v1 = (char*)memoryPool.allocate(10);
	memcpy(string_v1, "string_v1", 9);
	string_v1[9]=' ';
	memoryPool.printBlock();
	//memcpy(string_v1+4, "hi", 3);

	memoryPool.printBlock();

	char* string_v2 = (char*)memoryPool.allocate(SUB_SIZE-1);
	strcpy(string_v2, "AB");
	
	memoryPool.printBlock();

	char* string_v3 = (char*)memoryPool.allocate(DEF_BLOCK_SIZE+SUB_SIZE);
        strcpy(string_v3, "string_v2 + string_v3");

	memoryPool.printBlock();

	memoryPool.deallocate();

	return 0;

}
