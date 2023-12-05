#include <stdio.h>
#include <stdlib.h>	// malloc
#include <string.h>
#include <vector>
#include <algorithm>

#define BLOCK_COUNT (10)
#define BLOCK_SIZE  (16)

class MemoryPool
{
	public:
		MemoryPool(int block_size_, int block_num_);
		~MemoryPool();

		void* allocate(int size);
		void deallocate();
		void printAllBlock();
		void printBlock(int index);

	private:
		struct Block {
			struct Block* next;
		};

	private:
		int block_size;
		int block_num;
		std::vector<Block*> BlockMgr;		// 메모리 블록 관리자
		Block* currentBlock;			// 현재 블록
		Block* blocklist;
		int offset;				// 블록에 write
};

MemoryPool::MemoryPool(int block_size_, int block_num_):block_size(block_size_), block_num(block_num_)
{
	// 어려운 방법
	blocklist = (Block*) malloc(sizeof(block_size*block_num));
	currentBlock = blocklist;
	offset = 0;

	Block* currentblock = blocklist;
	for (int i=0; i<block_num - 1; i++)
	{
		BlockMgr.push_back(currentblock);
		BlockMgr[i]->next = (Block*)(currentblock+block_size);
		currentblock = BlockMgr[i]->next;
	}
	BlockMgr.push_back(currentblock);
	BlockMgr[block_num-1]->next = nullptr;

	// 쉬운 방법
	/*
	blocklist = (Block*) malloc(sizeof(block_size*block_num));
	for (int i=0; i<block_num; i++)
	{
		BlockMgr.push_back((Block*) blocklist + i*block_size);
	}

	for (int i=0; i<block_num -1; i++)
	{
		BlockMgr[i]->next = BlockMgr[i+1];
	}
	
	BlockMgr[block_num-1]->next = nullptr;

	*/
	printAllBlock();
}

MemoryPool::~MemoryPool()
{
	BlockMgr.clear();

	free(blocklist);
}

void* MemoryPool::allocate(int size)
{
	if (currentBlock == nullptr ) // 블록을 모두 소모
		return nullptr;

	if (offset + size > block_size)
	{
		auto iter = std::find(BlockMgr.begin(), BlockMgr.end(), currentBlock);
		if (iter == BlockMgr.end())
		{
			printf("[Error] currentBlock is NONE Pointer\n");
			return nullptr;
		}

		int index = std::distance(BlockMgr.begin(), iter);
		if (index+1 == BlockMgr.size()) // 더이상 할당할 Block 의 갯수가 없음
		{
			printf("[Info] All Block used\n");
			return nullptr;
		}

		printf("[Info] New Block Allocated, index(%d)\n", index);
		offset = 0;
		currentBlock = BlockMgr[index+1];
	}

	void* result = (char*)currentBlock+offset;
	
	currentBlock = (Block*)result;
	offset += size;
	return result;
}

void MemoryPool::deallocate()
{
}

void MemoryPool::printAllBlock()
{
	printf("=========== Block Info ===========\n");
	for (int i=0; i<BlockMgr.size(); i++)
	{
		printf("[%d] block: (%p), next: (%p)\n", i, BlockMgr[i], BlockMgr[i]->next);
	}
	printf("==================================\n");
}

void MemoryPool::printBlock(int index)
{
	printf("Block[%d] : %s\n", index, (char*)BlockMgr[index]);
}

int main()
{
	int block_num_ = BLOCK_COUNT;
	int block_size_ = BLOCK_SIZE;

	MemoryPool memoryPool(block_size_, block_num_);
	/*for (int i=0; i<block_num_; i++)
	{
		char* block = (char*)memoryPool.allocate(block_size_); 
		printf("Block[%d] allocated: %p\n", (i+1), block);
	}*/

	char* block_v0 = (char*) memoryPool.allocate(6);
	memcpy(block_v0, "ABCDEF",6);

	char* block_v1 = (char*) memoryPool.allocate(4);
	memcpy(block_v1, "GHIJ", 4);

	printf("[0] block_v0: %p  %s\n", block_v0, block_v0);
	printf("[1] block_v1: %p  %s\n", block_v1, block_v1);

	memoryPool.printBlock(0);

	return 0;
}
