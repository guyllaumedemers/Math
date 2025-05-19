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

#include "IDrawable.hh"
#include "IBatchResource.hh"
#include "ITickable.hh"
#include "Utilities/Transform.hh"

// define a system handling a 2d/3d space layout
struct FGrid :
	public IBatchResource,
	public IDrawable
{
	FGrid() = default;
	FGrid static& Get();

	virtual std::size_t const Size() const;
	virtual void ApplicationDraw(FViewport const& Viewport, FCamera const& Camera) override;
	virtual void ImGuiDraw(FCamera* const Camera) override;

protected:
	FTransform Transform = FTransform::Default;
	std::size_t NumRows = 0;
	std::size_t NumCols = 0;

	FGrid static Singleton;
};
