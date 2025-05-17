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
	FWorld(FWorld const&) = default;
	FWorld(FWorld&&) = default;
	FWorld& operator=(FWorld const&) = default;
	FWorld& operator=(FWorld&&) = default;

	void Draw();
	void DrawImGui();
	void Tick();

	// factory
	static FWorld Factory(IBatchResource&&);

protected:
	// context object for a simulation
	struct FWorldContext :
		public IDrawable,
		public ITickable
	{
		FWorldContext() = default;
		FWorldContext(FWorldContext const&) = delete;
		FWorldContext(FWorldContext&&) = default;
		FWorldContext& operator=(FWorldContext const&) = delete;
		FWorldContext& operator=(FWorldContext&& WorldContext);

		FWorldContext(IBatchResource&&);
		~FWorldContext();

		virtual void ApplicationDraw(struct FViewport const&, struct FCamera const&) override;
		virtual void ImGuiDraw() override;
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
	// @gdemers
	// note : AABB, if two bounds cancel each other, then it's sitting at the origin for this axis.
	// scaling still happen, but the view volume doesn't have to execute translation.
	// also, note that the scaling will result in the reciprocal of the range * 2.
	// example : [-10,10] = 20. reciprocal: 1/20, times 2; 2/20 or 1/10, which is how much we scale our world point by.
	// Pw(x) = 5; 5 * 1/10 = 5/10; Ps(s) = 0.5 which is within the bounds of our cannonical view [-1,1]. 
	FCamera Camera = FCamera{ FTransform::Default, FAxisAlignBoundingBox{-10, 10, -10, 10, -10, 10}, 45 };
};