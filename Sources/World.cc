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

#include <imgui.h>

#include "Memory.hh"
#include "Concept/DemoExpression.hh"

extern FArenaAllocator gArenaAllocator;

void FWorld::Draw()
{
	WorldContext.ApplicationDraw(Viewport, Camera);
}

void FWorld::DrawImGui()
{
	ImGui::Begin("Tool");

	ImGui::Text("Camera");
	ImGui::Separator();

	ImGui::NewLine();

	{
		ImGui::Text("Axis-Aligned Bounding Box");
		ImGui::Separator();

		ImGui::BeginGroup();

		static char const* const lTitle = "left";
		static char const* const rTitle = "right";
		static char const* const bTitle = "bottom";
		static char const* const tTitle = "top";
		static char const* const nTitle = "near";
		static char const* const fTitle = "far";
		int static left = Camera.ViewVolume.Left;
		int static right = Camera.ViewVolume.Right;
		int static bottom = Camera.ViewVolume.Bottom;
		int static top = Camera.ViewVolume.Top;
		int static near = Camera.ViewVolume.Near;
		int static far = Camera.ViewVolume.Far;
		if (ImGui::InputInt(lTitle, &left)) { Camera.ViewVolume.Left = left; };
		if (ImGui::InputInt(rTitle, &right)) { Camera.ViewVolume.Right = right; };
		if (ImGui::InputInt(bTitle, &bottom)) { Camera.ViewVolume.Bottom = bottom; };
		if (ImGui::InputInt(tTitle, &top)) { Camera.ViewVolume.Top = top; };
		if (ImGui::InputInt(nTitle, &near)) { Camera.ViewVolume.Near = near; };
		if (ImGui::InputInt(fTitle, &far)) { Camera.ViewVolume.Far = far; };

		ImGui::EndGroup();

		static const char* const ResetTitle = "Reset AABB";
		if (ImGui::Button(ResetTitle, { ImGui::GetContentRegionAvail().x , 0 }))
		{
			Camera.ViewVolume = FAxisAlignBoundingBox(-10, 10, -10, 10, -10, 10);
			left = right = bottom = top = near = far = 0;
		}
	}

	ImGui::NewLine();

	{
		ImGui::Text("Translation");
		ImGui::Separator();

		int static xValue = 0;
		int static yValue = 0;
		int static zValue = 0;

		{
			ImGui::BeginGroup();

			static char const* const xTitle = "X";
			int const static xMin = -10;
			int const static xMax = 10;
			if (ImGui::SliderInt(xTitle, &xValue, xMin, xMax)) { Camera.Transform.Position[0] = xValue; }

			ImGui::EndGroup();
		}

		{
			ImGui::BeginGroup();

			static char const* const yTitle = "Y";
			int const static yMin = -10;
			int const static yMax = 10;
			if (ImGui::SliderInt(yTitle, &yValue, yMin, yMax)) { Camera.Transform.Position[1] = yValue; }

			ImGui::EndGroup();
		}

		{
			ImGui::BeginGroup();

			static char const* const zTitle = "Z";
			int const static zMin = -10;
			int const static zMax = 10;
			if (ImGui::SliderInt(zTitle, &zValue, zMin, zMax)) { Camera.Transform.Position[2] = zValue; }

			ImGui::EndGroup();
		}

		static const char* const ResetTitle = "Reset Translation";
		if (ImGui::Button(ResetTitle, { ImGui::GetContentRegionAvail().x , 0 }))
		{
			Camera.Transform.Position = FVector3d::Zero;
			xValue = yValue = zValue = 0;
		}
	}

	ImGui::End();

	WorldContext.ImGuiDraw();
}

void FWorld::Tick()
{
	WorldContext.Tick();
}

FWorld FWorld::Factory(IBatchResource&& BatchResource)
{
	return FWorldContext{ std::move(BatchResource) };
}

FWorld::FWorld(FWorldContext&& WorldContext)
{
	// move operation doesn't invalidate correctly resources, an overload of the move assignment is required
	this->WorldContext = std::move(WorldContext);
}

FWorld::FWorldContext& FWorld::FWorldContext::operator=(FWorld::FWorldContext&& WorldContext)
{
	this->Handle = std::move(WorldContext.Handle);
	WorldContext.Handle = {};
	return *this;
}

FWorld::FWorldContext::FWorldContext(IBatchResource&& BatchResource)
{
	uint32_t static RefCount = 0;
	Handle.MemoryBlock = FMemory::Malloc({ &gArenaAllocator, BatchResource.Size() }, &BatchResource);
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

void FWorld::FWorldContext::ImGuiDraw()
{
	if (Handle.MemoryBlock.Payload == nullptr) { return; }
	auto* const Payload = static_cast<UDemoExpression*>(Handle.MemoryBlock.Payload);
	assert(!!Payload);
	Payload->ImGuiDraw();
}

void FWorld::FWorldContext::Tick()
{
	if (Handle.MemoryBlock.Payload == nullptr) { return; }
	auto* const Payload = static_cast<UDemoExpression*>(Handle.MemoryBlock.Payload);
	assert(!!Payload);
	Payload->Tick();
}