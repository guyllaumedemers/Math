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
#include <cstring>
#include <stdio.h>

FArenaAllocator gArenaAllocator;
FStackAllocator gStackAllocator;

FMemoryBlock FMemory::Malloc(FAllocator* Allocator, std::size_t Bytes)
{
	assert(!!Allocator);
	return FMemoryBlock{ Bytes, Allocator->Allocate(Bytes) };
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

void FMemory::FreeAll(FAllocator* Allocator)
{
	assert(!!Allocator);
	Allocator->DeallocateAll();
}

std::size_t FMemory::MemAlign(std::size_t Address, std::size_t Alignment)
{
	assert(FMemory::IsPowerOfTwo(Alignment));

	std::size_t AlignedAddress = Address;
	auto const Remainder = (Address % Alignment);
	if (Remainder > 0) { AlignedAddress += (Alignment - Remainder); }
	return AlignedAddress;
}

bool FMemory::IsPowerOfTwo(std::size_t Bytes)
{
	return ((Bytes & (Bytes - 1)) == 0);
}

FArenaAllocator::FArenaAllocator()
{
	Allocate(0);
}

FArenaAllocator::~FArenaAllocator()
{
	DeallocateAll();
}

void* FArenaAllocator::Allocate(std::size_t Bytes)
{
	auto const Head = reinterpret_cast<std::size_t>(MemoryBuffer + CurrOffset);

	std::size_t const Padding = FMemory::MemAlign(Head, DEFAULT_ALIGNMENT) - Head;
	std::size_t const BytesDiff = ((Head + Padding) - reinterpret_cast<std::size_t>(MemoryBuffer));

	if ((BytesDiff + Bytes) <= sizeof(MemoryBuffer))
	{
		CurrOffset = BytesDiff + Bytes;
		printf("Allocation:%zu, Padding:%zu, Remainder:%zu\n", Bytes, Padding, sizeof(MemoryBuffer) - CurrOffset);
		return std::memset(&MemoryBuffer[BytesDiff], 0, Bytes);
	}
	else
	{
		printf("Allocation failed\n");
		return nullptr;
	}
}

void FArenaAllocator::Deallocate(std::size_t Bytes)
{
	// @gdemers remains empty
}

void FArenaAllocator::DeallocateAll()
{
	printf("Deallocate All\n");
	std::memset(MemoryBuffer, 0, ARENA_ALLOCATOR_SIZE);
}

FStackAllocator::FStackAllocator()
{
	Allocate(0);
}

FStackAllocator::~FStackAllocator()
{
	DeallocateAll();
}

void* FStackAllocator::Allocate(std::size_t Bytes)
{
	auto const Head = reinterpret_cast<std::size_t>(MemoryBuffer + CurrOffset);

	std::size_t Padding = FMemory::MemAlign(Head, DEFAULT_ALIGNMENT) - Head;
	std::size_t const BytesDiff = ((Head + Padding) - reinterpret_cast<std::size_t>(MemoryBuffer));
	std::size_t const HeaderPadding = sizeof(FStackAllocatorHeader);

	if (Padding < HeaderPadding)
	{
		std::size_t const Diff = HeaderPadding - Padding;
		if (FMemory::IsPowerOfTwo(Diff))
		{
			// @gdemers if (Diff / DEFAULT_ALIGNMENT) < 1, then it resolve to 0, unless the diff is bigger than the default alignment.
			// our header will then sit at the lower bound of the previous cache line, so accessing our header will require two cache line access.
			Padding += (DEFAULT_ALIGNMENT * (Diff / DEFAULT_ALIGNMENT));
		}
		else
		{
			// @gdemers if (Diff / DEFAULT_ALIGNMENT) < 1, then it resolve to 0. which we add 1 to and just jump to the next alignement.
			// our header will then sit at the lower bound of the previous cache line, so accessing our header will require two cache line access.
			Padding += (DEFAULT_ALIGNMENT * (1 + (Diff / DEFAULT_ALIGNMENT)));
		}
	}

	if ((BytesDiff + Bytes) <= sizeof(MemoryBuffer))
	{
		auto* Header = reinterpret_cast<FStackAllocatorHeader*>(Head + Padding - HeaderPadding);
		Header->Padding = Padding;

		CurrOffset = BytesDiff + Bytes;
		printf("Allocation:%zu, Padding:%zu, Remainder:%zu\n", Bytes, Padding, sizeof(MemoryBuffer) - CurrOffset);
		return std::memset(&MemoryBuffer[BytesDiff], 0, Bytes);
	}
	else
	{
		printf("Allocation failed\n");
		return nullptr;
	}
}

void FStackAllocator::Deallocate(std::size_t Bytes)
{
}

void FStackAllocator::DeallocateAll()
{
	printf("Deallocate All\n");
	std::memset(&MemoryBuffer, 0, STACK_ALLOCATOR_SIZE);
}
