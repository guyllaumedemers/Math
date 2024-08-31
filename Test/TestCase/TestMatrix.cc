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

#include "Utilities/Matrix.hh"

#include "gtest/gtest.h"

class TestTMatrix : public testing::Test
{
protected:
	virtual void SetUp() override
	{
		FloatSquared =
		{
			Private::TVector<float, 3>{1,2,3},
			Private::TVector<float, 3>{4,5,6},
			Private::TVector<float, 3>{7,8,9}
		};

		FloatNonSquared =
		{
			Private::TVector<float, 3>{1,2,3},
			Private::TVector<float, 3>{4,5,6}
		};
	}

	virtual void TearDown() override
	{
		// stack allocation, will be released when going out-of-scope
	}

	// target properties
	Private::TMatrix<float, 3, 3> FloatSquared{};
	Private::TMatrix<float, 2, 3> FloatNonSquared{};
};

/**
 *	Impl already handle compile-time assertion on non-floating point types
 *	and zero-size matrix.
 */

TEST_F(TestTMatrix, DeterminantWorks)
{
}

TEST_F(TestTMatrix, TransposeWorks)
{
}

TEST_F(TestTMatrix, AdjugateWorks)
{
}

TEST_F(TestTMatrix, MatrixMatrixProductWorks)
{
}

TEST_F(TestTMatrix, MatrixVectorProductWorks)
{
}