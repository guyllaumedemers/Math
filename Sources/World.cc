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

#include "World.hh"

#include "Memory.hh"

extern FStackAllocator gStackAllocator;

FWorld FWorld::Factory(IBatchResource&& BatchResource)
{
	return FWorldContext{ std::move(BatchResource) };
}

FWorld::FWorld(FWorldContext&& WorldContext)
{
	this->WorldContext = WorldContext;
}

FWorld::FWorldContext::FWorldContext(IBatchResource&& BatchResource)
{
	uint32_t static RefCount = 0;
	Handle.MemoryBlock = FMemory::Malloc({ &gStackAllocator, BatchResource.Size() }, &BatchResource);
	Handle.HandleId = ++RefCount;
}

FWorld::FWorldContext::~FWorldContext()
{
	if (Handle.MemoryBlock.Payload == nullptr) { return; }
	FMemory::Free(&gStackAllocator, Handle.MemoryBlock);
}

void FWorld::FWorldContext::Draw()
{
	// TODO Fix this! vtable to IDrawable doesnt have a valid address
	if (Handle.MemoryBlock.Payload == nullptr) { return; }
	auto* const Payload = reinterpret_cast<IDrawable*>(Handle.MemoryBlock.Payload);
	assert(!!Payload);
	Payload->Draw();
}

void FWorld::FWorldContext::Tick()
{
	// TODO Fix this! vtable to ITickable doesnt have a valid address
	if (Handle.MemoryBlock.Payload == nullptr) { return; }
	auto* const Payload = reinterpret_cast<ITickable*>(Handle.MemoryBlock.Payload);
	assert(!!Payload);
	Payload->Tick();
}