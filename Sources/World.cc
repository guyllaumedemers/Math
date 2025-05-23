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
#include "Concept/DemoExpression.hh"

extern FArenaAllocator gArenaAllocator;

void FWorld::Draw()
{
	WorldContext.ApplicationDraw(Viewport, Camera);
}

void FWorld::DrawImGui()
{
	WorldContext.ImGuiDraw(&Camera);
}

void FWorld::Tick()
{
	WorldContext.Tick();
}

FWorld FWorld::Factory(IBatchResource&& Rhs)
{
	return FWorldContext{ std::move(Rhs) };
}

FWorld::FWorld(FWorldContext&& Rhs)
{
	// move operation doesn't invalidate correctly resources, an overload of the move assignment is required
	this->WorldContext = std::move(Rhs);
}

FWorld::FWorldContext& FWorld::FWorldContext::operator=(FWorld::FWorldContext&& Rhs)
{
	this->Handle = std::move(Rhs.Handle);
	Rhs.Handle = {};
	return *this;
}

FWorld::FWorldContext::FWorldContext(IBatchResource&& Rhs)
{
	uint32_t static RefCount = 0;
	Handle.MemoryBlock = FMemory::Malloc({ &gArenaAllocator, Rhs.Size() }, &Rhs);
	Handle.HandleId = ++RefCount;

	// TODO find better architecture to support init an expression
	auto* const Payload = static_cast<UDemoExpression*>(Handle.MemoryBlock.Payload);
	assert(!!Payload);
	Payload->Init();
}

FWorld::FWorldContext::~FWorldContext()
{
	if (Handle.MemoryBlock.Payload == nullptr) { return; }

	// TODO find better architecture to support cleanup an expression
	auto* const Payload = static_cast<UDemoExpression*>(Handle.MemoryBlock.Payload);
	assert(!!Payload);
	Payload->Cleanup();
	FMemory::Free(&gArenaAllocator, Handle.MemoryBlock);
}

void FWorld::FWorldContext::ApplicationDraw(FViewport const& Viewport, FCamera const& Camera)
{
	// https://lukasatkinson.de/2018/interface-dispatch/
	// https://stackoverflow.com/questions/24067594/how-does-a-c-compiler-handle-offsets-with-multiple-inheritance
	// Conclusion : Compiler doesnt know how to handle void* cast to multiple inheritance. All allocated payload have to be casted to a common base type so object layout can be resolved,
	// otherwise compiler treat all vtables as the starting one (implying it may call the wrong function at starting address).
	if (Handle.MemoryBlock.Payload == nullptr) { return; }
	auto* const Payload = static_cast<UDemoExpression*>(Handle.MemoryBlock.Payload);
	assert(!!Payload);
	Payload->ApplicationDraw(Viewport, Camera);
}

void FWorld::FWorldContext::ImGuiDraw(FCamera* const Camera)
{
	if (Handle.MemoryBlock.Payload == nullptr) { return; }
	auto* const Payload = static_cast<UDemoExpression*>(Handle.MemoryBlock.Payload);
	assert(!!Payload);
	Payload->ImGuiDraw(Camera);
}

void FWorld::FWorldContext::Tick()
{
	if (Handle.MemoryBlock.Payload == nullptr) { return; }
	auto* const Payload = static_cast<UDemoExpression*>(Handle.MemoryBlock.Payload);
	assert(!!Payload);
	Payload->Tick();
}