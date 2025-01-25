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

#include "Memory.hh"

#include <cassert>
#include <stdio.h>

FStackAllocator gStackAllocator;

FMemoryBlock FMemory::Malloc(FAllocator* Allocator, std::size_t Size)
{
	// TODO handle alignment later!
	assert(!!Allocator);
	return FMemoryBlock{ Size, Allocator->Allocate(Size) };
}

FMemoryBlock FMemory::Malloc(FAllocatorInfo const& Info, void* Data)
{
	return MemCpy(Malloc(Info.Allocator, Info.Size), Data);
}

FMemoryBlock FMemory::MemCpy(FMemoryBlock&& MemoryBlock, void* Data)
{
	auto* const Dest = reinterpret_cast<char*>(MemoryBlock.Payload);
	auto* const Src = reinterpret_cast<char*>(Data);
	for (std::size_t i = 0; i < MemoryBlock.Size; ++i) { Dest[i] = Src[i]; }
	return MemoryBlock;
}

void FMemory::Free(FAllocator* Allocator, FMemoryBlock& MemoryBlock)
{
	assert(!!Allocator);
	Allocator->Deallocate(MemoryBlock.Size);
	MemoryBlock = FMemoryBlock();
}

void FMemory::Free(FAllocator* Allocator, FMemoryBlock&& MemoryBlock)
{
	Free(Allocator, MemoryBlock/*rvalue, becomes a named variable, i.e lvalue*/);
}

FStackAllocator::FStackAllocator()
{
	Head = ReservedMemory;
	// https://www.geeksforgeeks.org/what-is-array-decay-in-c-how-can-it-be-prevented/
	// back point to out-of-bound memory on purpose to warn against possible overflow
	Tail = Head + sizeof(ReservedMemory)/*Arr num*/;
	printf("Head: %p, Tail: %p, Allocator Size in Bytes: %u \n", Head, Tail, sizeof(ReservedMemory));
}

void* FStackAllocator::Allocate(std::size_t Size)
{
	printf("Allocation Size in Bytes: %zu, Remaining Size: %zu \n", Size, (Tail - Head) - Size);
	assert((Head + Size) < Tail);
	Head += Size;
	return Head - Size;
}

void FStackAllocator::Deallocate(std::size_t Size)
{
	printf("DeAllocation Size in Bytes: %zu, Remaining Size: %zu \n", Size, (Tail - Head) + Size);
	assert((Head - Size) >= ReservedMemory);
	Head -= Size;
}