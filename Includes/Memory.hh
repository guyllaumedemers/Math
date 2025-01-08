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

#include <cstddef>

#ifndef STACK_ALLOCATOR_SIZE
#define STACK_ALLOCATOR_SIZE 1024
#endif

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
	virtual void Deallocate(std::size_t) = 0;
};

// memory system handling resource allocation/deallocation
struct FMemory
{
	static FMemoryBlock Malloc(FAllocator*, std::size_t);
	static FMemoryBlock Malloc(FAllocatorInfo const&, void*);
	static FMemoryBlock MemCpy(FMemoryBlock&&, void*);
	static void Free(FAllocator*, FMemoryBlock&);
};

struct FStackAllocator : public FAllocator
{
	FStackAllocator();
	virtual void* Allocate(std::size_t) override;
	virtual void Deallocate(std::size_t) override;

private:
	char ReservedMemory[STACK_ALLOCATOR_SIZE];
	char* Head = nullptr;
	char* Tail = nullptr;
};