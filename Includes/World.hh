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

#include "Camera.hh"
#include "IDrawable.hh"
#include "IBatchResource.hh"
#include "ITickable.hh"
#include "Utilities/Transform.hh"
#include "Utilities/Viewport.hh"

// define a vector space, draw a grid to visual math function behaviours
class FWorld
{
public:
	FWorld() = default;
	FWorld(FWorld const& Rhs) = default;
	FWorld(FWorld&& Rhs) = default;
	FWorld& operator=(FWorld const& Rhs) = default;
	FWorld& operator=(FWorld&& Rhs) = default;

	void Draw();
	void DrawImGui();
	void Tick();

	// factory
	static FWorld Factory(IBatchResource&& Rhs);

protected:
	// context object for a simulation
	struct FWorldContext :
		public IDrawable,
		public ITickable
	{
		FWorldContext() = default;
		FWorldContext(FWorldContext const& Rhs) = delete;
		FWorldContext(FWorldContext&& Rhs) = default;
		FWorldContext& operator=(FWorldContext const& Rhs) = delete;
		FWorldContext& operator=(FWorldContext&& Rhs);

		FWorldContext(IBatchResource&&);
		~FWorldContext();

		virtual void ApplicationDraw(FViewport const& Viewport, FCamera const& Camera) override;
		virtual void ImGuiDraw(FCamera* const Camera) override;
		virtual void Tick() override;

		// resources handle
		FBatchResourceHandle Handle;
	};

	FWorld(FWorldContext&&);

	// TODO later addition to support multiple world context, i.e multiple viewport with
	// various running instance of math expression should be expected

	// Could add a Push/Pop function that append our world with various context that split screen
	// into each context available

	// world resources
	FWorldContext WorldContext;

	// viewport target
	FViewport Viewport = FViewport::Default;

	// user point of view
	FCamera Camera = FCamera::Default;
};