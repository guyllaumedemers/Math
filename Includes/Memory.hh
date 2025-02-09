//Copyright(c) 2024 gdemers
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#pragma once

#ifndef ARENA_ALLOCATOR_SIZE
#define ARENA_ALLOCATOR_SIZE 1024
#endif

#ifndef STACK_ALLOCATOR_SIZE
#define STACK_ALLOCATOR_SIZE 1024
#endif

#ifndef POOL_ALLOCATOR_SIZE
#define POOL_ALLOCATOR_SIZE 4096
#endif

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT 16
#endif

#ifndef CHUNK_SIZE
#define CHUNK_SIZE 64
#endif

#include <cstddef>
#include <cstdint>

struct FAllocatorInfo
{
	struct FAllocator* Allocator = nullptr;
	std::size_t Size = 0;
};

struct FMemoryBlock
{
	std::size_t Size = 0;
	void* Payload = nullptr;
};

struct FAllocator
{
	virtual ~FAllocator() = default;
	virtual void* Allocate(std::size_t) = 0;
	virtual void Deallocate(void*) = 0;
	virtual void DeallocateAll() = 0;
};

// memory system handling resource allocation/deallocation
struct FMemory
{
	static FMemoryBlock Malloc(FAllocator*, std::size_t);
	static FMemoryBlock Malloc(FAllocatorInfo const&, void*);
	static FMemoryBlock MemCpy(FMemoryBlock&&, void*);
	static void Free(FAllocator*, FMemoryBlock&);
	static void Free(FAllocator*, FMemoryBlock&&);
	static void FreeAll(FAllocator*);
	static std::size_t MemAlign(std::size_t, std::size_t);
	static bool IsPowerOfTwo(std::size_t);
};

// linear allocation
struct FArenaAllocator : public FAllocator
{
	FArenaAllocator();
	~FArenaAllocator();
	virtual void* Allocate(std::size_t) override;
	virtual void Deallocate(void*) override;
	virtual void DeallocateAll() override;

private:
	char MemoryBuffer[ARENA_ALLOCATOR_SIZE];
	std::size_t CurrOffset = 0;
};

// header allocated before a memory aligned block
struct FStackAllocatorHeader
{
	std::size_t PrevOffset = 0;
	std::size_t Padding = 0;
};

// similar but allow releasing chunks via its header layout
struct FStackAllocator : public FAllocator
{
	FStackAllocator();
	~FStackAllocator();
	virtual void* Allocate(std::size_t) override;
	virtual void Deallocate(void*) override;
	virtual void DeallocateAll() override;

private:
	char MemoryBuffer[STACK_ALLOCATOR_SIZE];
	std::size_t PrevOffset = 0;
	std::size_t CurrOffset = 0;
};

// similar to the stack allocator but track both end of the memory buffer to allocate memory
struct FDoubleEndStackAllocator : public FAllocator
{
	// TODO @gdemers Do Impl
};

// free list node pointing to available memory space
struct FPoolAllocatorFreeNode
{
	FPoolAllocatorFreeNode* Next = nullptr;
};

// memory layout segmented in chunks/bin of user defined size
struct FPoolAllocator : public FAllocator
{
	FPoolAllocator() = default;
	FPoolAllocator(std::size_t);
	~FPoolAllocator();
	virtual void* Allocate(std::size_t) override;
	virtual void Deallocate(void*) override;
	virtual void DeallocateAll() override;

private:
	char MemoryBuffer[POOL_ALLOCATOR_SIZE];
	std::size_t ChunkSize = CHUNK_SIZE;

	FPoolAllocatorFreeNode* FreeList = nullptr;
};