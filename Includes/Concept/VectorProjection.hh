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

#include "DemoExpression.hh"

class UVectorProjection : public UDemoExpression
{
	// c being a scalar factor
	// b being a vector
	// {c*'b' e R}

	// proj[b]a - reads projection of 'a' onto 'b'
	// 'a' casting a shadow on 'L' being the line segment to with 'b' is part of

	// proj[b]a == c*'b', they are the same

	// rej[b]a == 'a' - proj[b]a == 'a' - c*'b'

	// 'b' dot (a - c*'b') == 0 knowing that the rejection of 'a' from 'b' is orthogonal to the projection of 'a' onto 'b'

	// 'b' * 'a' - c*'b'*'b' == 0

	// 'b' * 'a' = c * 'b' * 'b'

	// ('b' * 'a') / ('b' * 'b') = c

	// and back to the projection intuition, we can say that proj[b]a == c*'b'

	// and since c == ('b' * 'a') / ('b' * 'b')

	// ('b' * (('b' * 'a') / ('b' * 'b'))) == proj[b]a

public:
	virtual std::size_t Size() const override { return sizeof(UVectorProjection); };
	virtual void Tick() override;
	virtual void ApplicationDraw() override;
	virtual void ImGuiDraw() override;
};