#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

class MemoryPool
{
	public:
		MemoryPool(int block_size_, int block_num_);
		~MemoryPool();

		void* allocate();
		void deallocate(void* p);
		void printAllBlock();
	
	public:
		struct Block {
			struct Block* next;
		};

	public:
		int block_size;
		int block_num;
		Block* freelist; // 현재 블록 위치
		Block* firstBlock;
};

MemoryPool::MemoryPool(int block_size_, int block_num_):block_size(block_size_), block_num(block_num_)
{
	freelist = (Block*) malloc(sizeof(block_size*block_num));
	memset(freelist, 0, sizeof(block_size*block_num));

	firstBlock = freelist;

	// block 의 next를 연결해주는 작업

	Block* currentBlock = freelist;
	for (int i=0; i<block_num - 1; i++)
	{
		currentBlock->next = (Block*)(currentBlock+block_size);
		currentBlock = currentBlock->next;
	}
	currentBlock->next = nullptr;

	printAllBlock();
}

MemoryPool::~MemoryPool()
{
	free(firstBlock);
}

void* MemoryPool::allocate()
{
	if (freelist == nullptr)
		return nullptr;

	void* result = freelist;
	freelist = freelist->next;
	return result;
}

void MemoryPool::deallocate(void* p)
{
	if (p==nullptr)
		return;

	Block* temp = (Block*)p;
	freelist = temp->next;
	
}

void MemoryPool::printAllBlock()
{
	printf("========== Block Info =========\n");
	Block* currentBlock = firstBlock;
	for (int i=0; i<block_num; i++)
	{
		printf("[%d] block: %p -> %p\n", i, currentBlock, currentBlock->next);
		currentBlock = currentBlock->next;
	}
	printf("===============================\n");
}

int main()
{
	const int block_size_ = 16;
	const int block_num_ = 10;

	MemoryPool memoryPool(block_size_, block_num_);
	
	std::vector<void*> pointers; // 블록 데이터 관리자
	for (int i=0; i<block_num_; i++)
	{
		void* p = memoryPool.allocate();
		pointers.push_back(p);

		printf("[%d]. block allocate -->%p\n",i, p);
	}

	printf("-------------------------------\n");
	for (int i=0; i<block_num_; i++)
	{
		memoryPool.deallocate(pointers[i]);
		printf("[%d]. block deallocate -->%p\n",i, pointers[i]);
	}

	return 0;
}
