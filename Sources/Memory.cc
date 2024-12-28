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

FStackAllocator gStackAllocator;

FMemoryBlock FMemory::Malloc(FAllocatorInfo const& Info)
{
	// TODO handle alignment later!
	assert(!!Info.Allocator);
	return FMemoryBlock{ Info.Size, Info.Allocator->Allocate(Info.Size) };
}

FMemoryBlock FMemory::Malloc(FAllocatorInfo const& Info, void* Data)
{
	return MemCpy(Malloc(Info), Data);
}

FMemoryBlock FMemory::MemCpy(FMemoryBlock MemoryBlock, void* Data)
{
	auto* const Dest = reinterpret_cast<std::size_t*>(MemoryBlock.Payload);
	auto* const Src = reinterpret_cast<std::size_t*>(Data);
	*Dest = *Src;
	return MemoryBlock;
}

void FMemory::Free(FAllocator* Allocator, FMemoryBlock& MemoryBlock)
{
	assert(!!Allocator);
	Allocator->Deallocate(MemoryBlock.Size);
	MemoryBlock = FMemoryBlock();
}

FStackAllocator::FStackAllocator()
{
	Head = ReservedMemory;
	// https://www.geeksforgeeks.org/what-is-array-decay-in-c-how-can-it-be-prevented/
	// back point to out-of-bound memory on purpose to warn against possible overflow
	Tail = (Head + (sizeof(ReservedMemory) / sizeof(ReservedMemory[0]))/*Arr num*/);
}

void* FStackAllocator::Allocate(std::size_t Size)
{
	assert((Head < Tail));
	void* Previous = Head;
	Head += (Size / sizeof(std::size_t));
	return Previous;
}

void FStackAllocator::Deallocate(std::size_t Size)
{
	if (Head == ReservedMemory) { return; }
	auto const MemoryJump = (Size / sizeof(std::size_t));
	assert((Head - MemoryJump) >= ReservedMemory);
	Head -= MemoryJump;
}