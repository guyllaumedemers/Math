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
		ZeroDeterminant =
		{
			Private::TVector<float, 3>{1,2,3},
			Private::TVector<float, 3>{4,5,6},
			Private::TVector<float, 3>{7,8,9}
		};

		NonZeroDeterminant =
		{
			Private::TVector<float, 3>{2,4,6},
			Private::TVector<float, 3>{7,5,2},
			Private::TVector<float, 3>{6,8,9}
		};

		Squared =
		{
			Private::TVector<float, 3>{1,2,3},
			Private::TVector<float, 3>{4,5,6},
			Private::TVector<float, 3>{7,8,9}
		};

		NonSquared =
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
	Private::TMatrix<float, 3, 3> ZeroDeterminant{};
	Private::TMatrix<float, 3, 3> NonZeroDeterminant{};
	Private::TMatrix<float, 3, 3> Squared{};
	Private::TMatrix<float, 2, 3> NonSquared{};
};

/**
 *	Impl already handle compile-time assertion on non-floating point types
 *	and zero-size matrix.
 *
 *	Transposition isn't allowed on non-squared matrix and won't compile due to compile-time assertion. Same
 *	for Determinant calculation and Adjugate.
 */

TEST_F(TestTMatrix, DeterminantWorks)
{
	// zero determinant output
	EXPECT_FLOAT_EQ(ZeroDeterminant.CalculateDeterminant(), 0.f) << "Expect Zero determinant";

	// determinant output
	EXPECT_FLOAT_EQ(NonZeroDeterminant.CalculateDeterminant(), 10.f);
}

TEST_F(TestTMatrix, TransposeWorks)
{
	auto const TransposeMatrix = Squared.Transpose();
	auto const Row = TransposeMatrix.GetRows();
	auto const Col = TransposeMatrix.GetCols();

	for (std::size_t i = 0; i < Row; ++i)
	{
		for (std::size_t j = 0; j < Col; ++j)
		{
			EXPECT_FLOAT_EQ(TransposeMatrix(i, j), Squared(j, i)) << "Expect Lhs[i,j] to EQU Rhs[j,i]";
		}
	}
}

TEST_F(TestTMatrix, AdjugateWorks)
{
	auto const AdjugateMatrix = NonZeroDeterminant.CalculateAdjugate();
	EXPECT_FLOAT_EQ(AdjugateMatrix(0, 0), 29.f);
	EXPECT_FLOAT_EQ(AdjugateMatrix(0, 1), 12.f);
	EXPECT_FLOAT_EQ(AdjugateMatrix(0, 2), -22.f);
	EXPECT_FLOAT_EQ(AdjugateMatrix(1, 0), -51.f);
	EXPECT_FLOAT_EQ(AdjugateMatrix(1, 1), -18.f);
	EXPECT_FLOAT_EQ(AdjugateMatrix(1, 2), 38.f);
	EXPECT_FLOAT_EQ(AdjugateMatrix(2, 0), 26.f);
	EXPECT_FLOAT_EQ(AdjugateMatrix(2, 1), 8.f);
	EXPECT_FLOAT_EQ(AdjugateMatrix(2, 2), -18.f);
}

TEST_F(TestTMatrix, MatrixInverseWorks)
{
	auto const InverseMatrix = NonZeroDeterminant.CalculateAdjugate() * (1.f / NonZeroDeterminant.CalculateDeterminant());
	float constexpr Tenth = 0.1f;
	float constexpr Fifth = 0.2f;

	EXPECT_FLOAT_EQ(InverseMatrix(0, 0), 29.f * Tenth);
	EXPECT_FLOAT_EQ(InverseMatrix(0, 1), 6.f * Fifth);
	EXPECT_FLOAT_EQ(InverseMatrix(0, 2), -11.f * Fifth);
	EXPECT_FLOAT_EQ(InverseMatrix(1, 0), -51.f * Tenth);
	EXPECT_FLOAT_EQ(InverseMatrix(1, 1), -9.f * Fifth);
	EXPECT_FLOAT_EQ(InverseMatrix(1, 2), 19.f * Fifth);
	EXPECT_FLOAT_EQ(InverseMatrix(2, 0), 13.f * Fifth);
	EXPECT_FLOAT_EQ(InverseMatrix(2, 1), 4.f * Fifth);
	EXPECT_FLOAT_EQ(InverseMatrix(2, 2), -9.f * Fifth);
}

TEST_F(TestTMatrix, MatrixMatrixProductWorks)
{
	auto const& NonSquaredOutput = NonSquared * Squared;

	ASSERT_NE(NonSquaredOutput.GetRows(), NonSquaredOutput.GetCols()) << "Expect Output to be a Non-Squared Matrix";
	ASSERT_EQ(NonSquaredOutput.GetRows(), 2) << "Expect a Matrix Row size of:";
	ASSERT_EQ(NonSquaredOutput.GetCols(), 3) << "Expect a Matrix Col size of:";
	EXPECT_FLOAT_EQ(NonSquaredOutput(0, 0), 30.f);
	EXPECT_FLOAT_EQ(NonSquaredOutput(0, 1), 36.f);
	EXPECT_FLOAT_EQ(NonSquaredOutput(0, 2), 42.f);
	EXPECT_FLOAT_EQ(NonSquaredOutput(1, 0), 66.f);
	EXPECT_FLOAT_EQ(NonSquaredOutput(1, 1), 81.f);
	EXPECT_FLOAT_EQ(NonSquaredOutput(1, 2), 96.f);

	auto const& SquaredOutput = Squared * Squared;

	ASSERT_EQ(SquaredOutput.GetRows(), SquaredOutput.GetCols()) << "Expect Output to be a Squared Matrix";
	ASSERT_EQ(SquaredOutput.GetRows(), 3) << "Expect a Matrix Row size of:";
	ASSERT_EQ(SquaredOutput.GetCols(), 3) << "Expect a Matrix Col size of:";
	EXPECT_FLOAT_EQ(SquaredOutput(0, 0), 30.f);
	EXPECT_FLOAT_EQ(SquaredOutput(0, 1), 36.f);
	EXPECT_FLOAT_EQ(SquaredOutput(0, 2), 42.f);
	EXPECT_FLOAT_EQ(SquaredOutput(1, 0), 66.f);
	EXPECT_FLOAT_EQ(SquaredOutput(1, 1), 81.f);
	EXPECT_FLOAT_EQ(SquaredOutput(1, 2), 96.f);
	EXPECT_FLOAT_EQ(SquaredOutput(2, 0), 102.f);
	EXPECT_FLOAT_EQ(SquaredOutput(2, 1), 126.f);
	EXPECT_FLOAT_EQ(SquaredOutput(2, 2), 150.f);
}

TEST_F(TestTMatrix, MatrixVectorProductWorks)
{
}