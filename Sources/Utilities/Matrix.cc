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

namespace Private
{
	// subset calculation require to ignore the row[0], col[k] of the parent subset
	// and skip those entries
	void SubMatrix(std::size_t Size, float** Matrix, std::size_t Row, std::size_t Col, float** Output)
	{
		std::size_t k = 0, m = 0;

		for (std::size_t i = 0; i < Size; ++i)
		{
			if (i == Row)
			{
				continue;
			}

			m = 0;
			Output[k] = new float[Size - 1]();
			for (std::size_t j = 0; j < Size; ++j)
			{
				if (j == Col)
				{
					continue;
				}

				Output[k][m] = Matrix[i][j];
				++m;
			}

			++k;
		}
	};

	// determinant calculation require processing a subset of a matrix which isn't
	// possible with our current homogeneous FMatrix4x4
	float CalculateDeterminant(std::size_t Size, float** Matrix)
	{
		float OutResult = 0.f;

		if (Size == 1)
		{
			OutResult = Matrix[0][0];
		}
		else if (Size == 2)
		{
			float const A = Matrix[0][0] * Matrix[1][1];
			float const B = Matrix[1][0] * Matrix[0][1] * -1;
			OutResult = (A + B);
		}
		else
		{
			for (std::size_t k = 0; k < Size; ++k)
			{
				float** Output = new float* [Size - 1]();
				SubMatrix(Size, Matrix, 0, k, Output);
				OutResult += (Matrix[0][k] * CalculateDeterminant(Size - 1, Output) * (k & 1 ? -1.f : 1.f));
				delete[] Output;
			}
		}

		return OutResult;
	};

	void Transpose(std::size_t Size, float** Matrix)
	{
		for (std::size_t i = 0; i < Size; ++i)
		{
			for (std::size_t j = 0; j < Size; ++j)
			{
				if (j <= i)
				{
					continue;
				}

				float const A = Matrix[i][j];
				float const B = Matrix[j][i];
				Matrix[i][j] = B;
				Matrix[j][i] = A;
			}
		}
	}

	// adjugate of a matrix return the 'minor' matrix cofactor transposed
	// note : handle memory release post-return
	float** CalculateAdjugate(std::size_t Size, float** Matrix)
	{
		float** Result = new float* [Size]();

		// calculate 'minor' matrix and 'cofactor'
		for (std::size_t i = 0; i < Size; ++i)
		{
			Result[i] = new float[Size]();
			for (std::size_t j = 0; j < Size; ++j)
			{
				float** Output = new float* [Size - 1]();
				SubMatrix(Size, Matrix, i, j, Output);
				Result[i][j] = CalculateDeterminant(Size - 1, Output) * ((i + j) & 1 ? -1.f : 1.f);
				delete[] Output;
			}
		}

		// transpose 'cofactor' matrix
		Transpose(Size, Result);
		return Result;
	};
}

FMatrix4x4 FMatrix4x4::Zero()
{
	return FMatrix4x4
	{
		FVector4d(0.f, 0.f, 0.f, 0.f),
		FVector4d(0.f, 0.f, 0.f, 0.f),
		FVector4d(0.f, 0.f, 0.f, 0.f),
		FVector4d(0.f, 0.f, 0.f, 0.f)
	};
}

FMatrix4x4 FMatrix4x4::Identity()
{
	return FMatrix4x4
	{
		FVector4d(1.f, 0.f, 0.f, 0.f),
		FVector4d(0.f, 1.f, 0.f, 0.f),
		FVector4d(0.f, 0.f, 1.f, 0.f),
		FVector4d(0.f, 0.f, 0.f, 1.f)
	};
}

FMatrix4x4 FMatrix4x4::Translate(FVector4d const& Translate)
{
	return FMatrix4x4
	{
		FVector4d(1.f, 0.f, 0.f, 0.f),
		FVector4d(0.f, 1.f, 0.f, 0.f),
		FVector4d(0.f, 0.f, 1.f, 0.f),
		FVector4d(Translate.X, Translate.Y, Translate.Z, 1.f)
	};
}

FMatrix4x4 FMatrix4x4::Rotate(FVector4d const& Rotate)
{
	// TODO impl rotation matrix
	return FMatrix4x4::Identity();
}

FMatrix4x4 FMatrix4x4::Scale(FVector4d const& Scale)
{
	return FMatrix4x4
	{
		FVector4d(Scale.X, 0.f, 0.f, 0.f),
		FVector4d(0.f, Scale.Y, 0.f, 0.f),
		FVector4d(0.f, 0.f, Scale.Z, 0.f),
		FVector4d(0.f, 0.f, 0.f, 1.f)
	};
}
