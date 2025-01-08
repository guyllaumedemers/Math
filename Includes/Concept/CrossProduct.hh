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

class UCrossProduct : public UDemoExpression
{
	// A = Axi + Ayj + Azk
	// B = Bxi + Byj + Bzk

	// i x i = 0
	// i x j = k
	// i x k = j
	// j x i = -k
	// j x j = 0
	// j x k = i
	// k x i = -j
	// k x j = -i
	// k x k = 0

	// (Axi * Byj)k  + (Axi * Bzk)j
	// (Ayj * Bxi)-k + (Ayj * Bzk)i
	// (Azk * Bxi)-j + (Azk * Byj)-i

	// {(Ayj * Bzk)i + (Azk * Byj)-i, (Axi * Bzk)j + (Azk * Bxi)-j, (Axi * Byj)k + (Ayj * Bxi)-k}
public:
	virtual std::size_t Size() const override { return sizeof(UCrossProduct); };
	virtual void Tick() override;
	virtual void Draw() override;
};