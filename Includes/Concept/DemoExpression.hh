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

#include "IBatchResource.hh"
#include "IDrawable.hh"
#include "IMathExpression.hh"
#include "ITickable.hh"

// define the default implementation details of a Math expression
class UDemoExpression :
	public IBatchResource,
	public IDrawable,
	public IMathExpression,
	public ITickable
{
public:
	UDemoExpression() = default;
	UDemoExpression(UDemoExpression const&) = delete;
	UDemoExpression(UDemoExpression&&) = default;
	UDemoExpression& operator=(UDemoExpression const&) = delete;
	UDemoExpression& operator=(UDemoExpression&&) = default;
	virtual std::size_t Size() const override;
	virtual void ApplicationDraw() override;
	virtual void ImGuiDraw() override;
	virtual void Tick() override;

	void Init();
	void Cleanup();

protected:
	bool bIsOrthographic = false;
	bool bIs3d = false;
	bool bDisplayGrid = false;

private:
	struct FObject* DemoCube = nullptr;
};