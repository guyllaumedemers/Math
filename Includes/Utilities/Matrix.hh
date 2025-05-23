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

#include <atomic>
#include <type_traits>

#include "Vector.hh"

namespace Private
{
	template<typename T, std::size_t M, std::size_t N>
	struct TMatrix
	{
		static_assert(std::is_floating_point_v<T>, "TMatrix ill format, can only accept floating point types");

		template<std::size_t K, std::size_t L>
		TMatrix<T, M, L> operator*(TMatrix<T, K, L> const& Rhs) const;

		template<std::size_t K>
		TVector<T, M> operator*(TVector<T, K> const& Rhs) const;

		TMatrix operator*(float In) const
		{
			TMatrix<T, M, N> Result{};
			for (std::size_t i = 0; i < GetRows(); ++i)
			{
				for (std::size_t j = 0; j < GetCols(); ++j)
				{
					Result(i, j) += (this->RowsCols[i][j] * In);
				}
			}

			return Result;
		}

		T& operator()(std::size_t Row, std::size_t Col)
		{
			return RowsCols[Row][Col];
		}

		T const& operator()(std::size_t Row, std::size_t Col) const
		{
			return RowsCols[Row][Col];
		}

		std::size_t GetRows() const
		{
			return RowsCols.size();
		}

		std::size_t GetCols() const
		{
			return RowsCols[0].Components.size();
		}

		// src : https://en.wikipedia.org/wiki/Adjugate_matrix
		// description : the adjugate of a square matrix A is the transpose of its cofactor matrix and is denoted by adj(A).
		TMatrix<T, M, N> CalculateAdjugate() const;

		// src : https://en.wikipedia.org/wiki/Determinant
		// description : the determinant is a scalar-valued function of the entries of a square matrix.
		T CalculateDeterminant() const;

		// description : calculate recursively sub-matrix, reaching at 2x2 size to calculate determinant.
		template<std::size_t K, std::size_t L>
		TMatrix<T, K, L> SubMatrix(std::size_t IgnoredRow, std::size_t IgnoredCol) const;

		// src : https://en.wikipedia.org/wiki/Transpose
		// description : the transpose of a matrix is an operator which flips a matrix over its diagonal.
		TMatrix<T, M, N> Transpose() const;

		std::array<TVector<T, N>, M> RowsCols{};
	};

	template <typename T, std::size_t M, std::size_t N>
	TMatrix<T, M, N> TMatrix<T, M, N>::Transpose() const
	{
		static_assert(M == N, "TMatrix ill format. Transpose : Cannot transpose non-squared matrix");

		TMatrix<T, M, N> Result{};
		std::size_t Row = GetRows();
		std::size_t Col = GetCols();

		for (std::size_t i = 0; i < Row; ++i)
		{
			for (std::size_t j = 0; j < Col; ++j)
			{
				if (j < i)
				{
					continue;
				}

				T const A = RowsCols[i][j];
				T const B = RowsCols[j][i];
				Result(i, j) = B;
				Result(j, i) = A;
			}
		}

		return Result;
	}

	template<typename T, std::size_t M, std::size_t N>
	template<std::size_t K, std::size_t L>
	TMatrix<T, M, L> TMatrix<T, M, N>::operator*(TMatrix<T, K, L> const& Rhs) const
	{
		static_assert(N == K, "TMatrix size is ill format. Matrix product can only happen if Mat_A nbCol == Mat_B nbRow");

		TMatrix<T, M, L> Result{};
		for (std::size_t i = 0; i < GetRows(); ++i)
		{
			for (std::size_t j = 0; j < Rhs.GetCols(); ++j)
			{
				for (std::size_t k = 0; k < Rhs.GetRows(); ++k)
				{
					Result(i, j) += (RowsCols[i][k] * Rhs(k, j));
				}
			}
		}

		return Result;
	}

	template<typename T, std::size_t M, std::size_t N>
	template<std::size_t K>
	TVector<T, M> TMatrix<T, M, N>::operator*(TVector<T, K> const& Rhs) const
	{
		static_assert(N == K, "TMatrix size is ill format. Matrix product can only happen if Mat_A nbCol == Mat_B nbRow");

		TVector<T, M> Result{};
		for (std::size_t i = 0; i < GetRows(); ++i)
		{
			for (std::size_t j = 0; j < Rhs.GetRows() /*j = Rhs Row[index] AND Matrix Cols[index]*/; ++j)
			{
				Result[i] += (RowsCols[i][j] * Rhs[j]);
			}
		}

		return Result;
	}

	template <typename T, std::size_t M, std::size_t N>
	template <std::size_t K, std::size_t L>
	TMatrix<T, K, L> TMatrix<T, M, N>::SubMatrix(std::size_t IgnoredRow, std::size_t IgnoredCol) const
	{
		TMatrix<T, K, L> Result{};
		std::size_t Row = GetRows();
		std::size_t Col = GetCols();
		std::size_t k = 0, l = 0;

		for (std::size_t i = 0; i < Row; ++i)
		{
			if (i == IgnoredRow)
			{
				continue;
			}

			l = 0;
			for (std::size_t j = 0; j < Col; ++j)
			{
				if (j == IgnoredCol)
				{
					continue;
				}

				Result(k, l) = RowsCols[i][j];
				++l;
			}

			++k;
		}

		return Result;
	}

	template <typename T, std::size_t M, std::size_t N>
	T TMatrix<T, M, N>::CalculateDeterminant() const
	{
		static_assert(M == N, "TMatrix size is ill format. Determinant : Cannot calculate determinant of non-squared matrix");

		T OutResult{};
		std::size_t Row = GetRows();
		std::size_t Col = GetCols();

		if (Row == 1)
		{
			OutResult = RowsCols[0][0];
		}
		else if (Row == 2)
		{
			T const A = RowsCols[0][0] * RowsCols[1][1];
			T const B = RowsCols[1][0] * RowsCols[0][1] * -1;
			OutResult = (A + B);
		}
		else
		{
			for (std::size_t k = 0; k < Col; ++k)
			{
				TMatrix<T, M - 1, N - 1> const TempMatrix = this->template SubMatrix<M - 1, N - 1>(0, k);
				OutResult += (RowsCols[0][k] * TempMatrix.CalculateDeterminant() * (k & 1 ? -1.f : 1.f));
			}
		}

		return OutResult;
	}

	template <typename T, std::size_t M, std::size_t N>
	TMatrix<T, M, N> TMatrix<T, M, N>::CalculateAdjugate() const
	{
		static_assert(M == N, "TMatrix size is ill format. Adjugate : Cannot calculate determinant of non-squared matrix");

		TMatrix<T, M, N> Result{};
		std::size_t Row = GetRows();
		std::size_t Col = GetCols();

		for (std::size_t i = 0; i < Row; ++i)
		{
			for (std::size_t j = 0; j < Col; ++j)
			{
				TMatrix<T, M - 1, N - 1> const TempMatrix = this->template SubMatrix<M - 1, N - 1>(i, j);
				Result(i, j) = TempMatrix.CalculateDeterminant() * ((i + j) & 1 ? -1.f : 1.f);
			}
		}

		return Result.Transpose();
	}

	template<typename T>
	struct TMatrix<T, 0, 0>
	{
		[[noreturn]]
		TMatrix& operator=(TMatrix<T, 0, 0> const& Rhs)
		{
			throw;
		}

		[[noreturn]]
		T& operator()(std::size_t Row, std::size_t Col)
		{
			throw;
		}

		[[noreturn]]
		T const& operator()(std::size_t Row, std::size_t Col) const
		{
			throw;
		}

		[[noreturn]]
		TMatrix<T, 0, 0> Transpose() const
		{
			throw;
		}

		[[noreturn]]
		T CalculateDeterminant() const
		{
			throw;
		}

		[[noreturn]]
		TMatrix<T, 0, 0> CalculateAdjugate() const
		{
			throw;
		}

		[[noreturn]]
		TMatrix<T, 0, 0> SubMatrix(std::size_t IgnoredRow, std::size_t IgnoredCol) const
		{
			throw;
		}
	};
}

// homogeneous coordinate
struct FMatrix4x4
{
	FMatrix4x4() = default;
	FMatrix4x4(FMatrix4x4 const& Rhs) = default;
	FMatrix4x4(FMatrix4x4&& Rhs) = default;
	FMatrix4x4& operator=(FMatrix4x4 const& Rhs) = default;
	FMatrix4x4& operator=(FMatrix4x4&& Rhs) = default;

	explicit FMatrix4x4(Private::TMatrix<float, 4, 4> const& Rhs);

	FMatrix4x4 const operator*(float const Rhs) const;
	FMatrix4x4 const operator*(FMatrix4x4 const& Rhs) const;
	FVector4d const operator*(FVector4d const& Rhs) const;
	FMatrix4x4& operator*=(FMatrix4x4 const& Rhs);

	FMatrix4x4 const Adjugate() const;
	float const Determinant() const;

	// custom matrix function exposing target mutation applied in vector space
	static FMatrix4x4 const& Zero();
	static FMatrix4x4 const& Identity();
	static FMatrix4x4 const Translate(FVector3d const& Rhs);
	static FMatrix4x4 const Rotate(FVector3d const& Rhs);
	static FMatrix4x4 const Scale(FVector3d const& Rhs);

	Private::TMatrix<float, 4, 4> Matrix{};
};
