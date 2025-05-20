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

#include <array>
#include <type_traits>

#include "Utilities/Math.hh"

namespace Private
{
	template<typename T, std::size_t N>
	struct TVector
	{
		static_assert(std::is_floating_point_v<T>, "TVector ill format, can only accept floating point types");

		// left-hand side of operator=
		T& operator[](std::size_t const Rhs)
		{
			return Components[Rhs];
		}

		// right-hand side of operator=
		T const& operator[](std::size_t const Rhs) const
		{
			return Components[Rhs];
		}

		TVector& operator=(TVector<T, N> const& Rhs)
		{
			this->Components = Rhs.Components;
			return *this;
		}

		TVector operator+(TVector<T, N> const& Rhs) const
		{
			TVector<T, N> Result{};

			for (std::size_t i = 0; i < GetRows(); ++i)
			{
				Result[i] = (Components[i] + Rhs[i]);
			}

			return Result;
		}

		TVector& operator+=(TVector<T, N> const& Rhs)
		{
			*this = (*this + Rhs);
			return *this;
		}

		TVector operator-(TVector<T, N> const& Rhs) const
		{
			TVector<T, N> Result{};

			for (std::size_t i = 0; i < GetRows(); ++i)
			{
				Result[i] = (Components[i] - Rhs[i]);
			}

			return Result;
		}

		TVector& operator-=(TVector<T, N> const& Rhs)
		{
			*this = (*this - Rhs);
			return *this;
		}

		bool operator==(TVector<T, N> const& Rhs) const
		{
			return (this->Components == Rhs.Components);
		}

		T DotProduct(TVector<T, N> const& Rhs) const
		{
			return FMath::DotProduct<T, N>(Components, Rhs.Components);
		}

		TVector CrossProduct(TVector<T, N> const& Rhs) const
		{
			return TVector{ FMath::CrossProduct<T, N>(Components, Rhs.Components) };
		}

		TVector Projection(TVector<T, N> const& Rhs) const
		{
			return TVector{ FMath::Projection<T, N>(Components, Rhs.Components) };
		}

		TVector Rejection(TVector<T, N> const& Rhs) const
		{
			return TVector{ FMath::Rejection<T, N>(Components, Rhs.Components) };
		}

		TVector Normalize() const
		{
			return TVector{ FMath::Normalize<T, N>(Components) };
		}

		T SquaredMagnitude() const
		{
			return FMath::SquaredMagnitude<T, N>(Components);
		}

		T Magnitude() const
		{
			return FMath::Magnitude<T, N>(Components);
		}

		std::size_t GetRows() const
		{
			return Components.size();
		}

		std::array<T, N> Components{};
	};
}

struct FVector2d
{
	FVector2d() = default;
	FVector2d(FVector2d const& Rhs) = default;
	FVector2d(FVector2d&& Rhs) = default;
	FVector2d& operator=(FVector2d const& Rhs) = default;
	FVector2d& operator=(FVector2d&& Rhs) = default;

	explicit FVector2d(float const X, float const Y);
	explicit FVector2d(float const Rhs);

	float const& operator[](std::size_t const Rhs) const;
	float& operator[](std::size_t const Rhs);

	FVector2d const static Zero;
	FVector2d const static One;
	Private::TVector<float, 2> Vector{};
};

struct FVector3d
{
	FVector3d() = default;
	FVector3d(FVector3d const& Rhs) = default;
	FVector3d(FVector3d&& Rhs) = default;
	FVector3d& operator=(FVector3d const& Rhs) = default;
	FVector3d& operator=(FVector3d&& Rhs) = default;

	explicit FVector3d(FVector2d const& Rhs);
	explicit FVector3d(Private::TVector<float, 3> const& Rhs);
	explicit FVector3d(float const X, float const Y, float const Z);
	explicit FVector3d(float const Rhs);

	FVector3d const operator+(FVector3d const& Rhs) const;
	FVector3d& operator+=(FVector3d const& Rhs);

	float const& operator[](std::size_t const Rhs) const;
	float& operator[](std::size_t const Rhs);

	FVector3d const static Zero;
	FVector3d const static One;
	Private::TVector<float, 3> Vector{};
};

struct FVector4d
{
	FVector4d() = default;
	FVector4d(FVector4d const& Rhs) = default;
	FVector4d(FVector4d&& Rhs) = default;
	FVector4d& operator=(FVector4d const& Rhs) = default;
	FVector4d& operator=(FVector4d&& Rhs) = default;

	explicit FVector4d(FVector3d const& Rhs);
	explicit FVector4d(Private::TVector<float, 4> const& Rhs);
	explicit FVector4d(float const X, float const Y, float const Z, float const W);
	explicit FVector4d(float const Rhs);

	FVector4d const operator*(float const Rhs) const;

	float const& operator[](std::size_t const Rhs) const;
	float& operator[](std::size_t const Rhs);

	FVector4d const static Zero;
	FVector4d const static One;
	Private::TVector<float, 4> Vector{};
};
