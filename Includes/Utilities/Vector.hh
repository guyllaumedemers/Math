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
		inline T& operator[](std::size_t Index)
		{
			return Components[Index];
		}

		// right-hand side of operator=
		inline T const& operator[](std::size_t Index) const
		{
			return Components[Index];
		}

		inline TVector& operator=(TVector<T, N> const& Rhs)
		{
			this->Components = Rhs.Components;
			return *this;
		}

		inline TVector operator+(TVector<T, N> const& Rhs) const
		{
			TVector<T, N> Result{};

			for (std::size_t i = 0; i < GetRows(); ++i)
			{
				Result[i] = (Components[i] + Rhs[i]);
			}

			return Result;
		}

		inline TVector& operator+=(TVector<T, N> const& Rhs)
		{
			*this = (*this + Rhs);
			return *this;
		}

		inline TVector operator-(TVector<T, N> const& Rhs) const
		{
			TVector<T, N> Result{};

			for (std::size_t i = 0; i < GetRows(); ++i)
			{
				Result[i] = (Components[i] - Rhs[i]);
			}

			return Result;
		}

		inline TVector& operator-=(TVector<T, N> const& Rhs)
		{
			*this = (*this - Rhs);
			return *this;
		}

		inline bool operator==(TVector<T, N> const& Rhs) const
		{
			return (this->Components == Rhs.Components);
		}

		inline T DotProduct(TVector<T, N> const& In) const
		{
			return FMath::DotProduct<T, N>(Components, In.Components);
		}

		inline TVector CrossProduct(TVector<T, N> const& In) const
		{
			return TVector{ FMath::CrossProduct<T, N>(Components, In.Components) };
		}

		inline TVector Projection(TVector<T, N> const& In) const
		{
			return TVector{ FMath::Projection<T, N>(Components, In.Components) };
		}

		inline TVector Rejection(TVector<T, N> const& In) const
		{
			return TVector{ FMath::Rejection<T, N>(Components, In.Components) };
		}

		inline TVector Normalize() const
		{
			return TVector{ FMath::Normalize<T, N>(Components) };
		}

		inline T SquaredMagnitude() const
		{
			return FMath::SquaredMagnitude<T, N>(Components);
		}

		inline T Magnitude() const
		{
			return FMath::Magnitude<T, N>(Components);
		}

		inline std::size_t GetRows() const
		{
			return Components.size();
		}

		std::array<T, N> Components{};
	};
}

struct FVector2d
{
	FVector2d() = default;
	FVector2d(FVector2d const&) = default;
	FVector2d(FVector2d&&) = default;
	FVector2d& operator=(FVector2d const&) = default;

	inline FVector2d(float In)
	{
		this->Vector[0] = In;
		this->Vector[1] = In;
	}

	inline FVector2d(float X, float Y)
	{
		this->Vector[0] = X;
		this->Vector[1] = Y;
	}

	FVector2d const static Zero;
	FVector2d const static One;
	Private::TVector<float, 2> Vector{};
};

struct FVector3d
{
	FVector3d() = default;
	FVector3d(FVector3d const&) = default;
	FVector3d(FVector3d&&) = default;
	FVector3d& operator=(FVector3d const&) = default;

	inline FVector3d(float In)
	{
		this->Vector[0] = In;
		this->Vector[1] = In;
		this->Vector[2] = In;
	}

	inline FVector3d(float X, float Y, float Z)
	{
		this->Vector[0] = X;
		this->Vector[1] = Y;
		this->Vector[2] = Z;
	}

	inline FVector3d(FVector2d const& In)
	{
		this->Vector[0] = In.Vector[0];
		this->Vector[1] = In.Vector[1];
		this->Vector[2] = 1.f;
	}

	FVector3d const static Zero;
	FVector3d const static One;
	Private::TVector<float, 3> Vector{};
};

struct FVector4d
{
	FVector4d() = default;
	FVector4d(FVector4d const&) = default;
	FVector4d(FVector4d&&) = default;
	FVector4d& operator=(FVector4d const&) = default;

	FVector4d operator*(float In) const
	{
		return FVector4d{};
	}

	float& operator[](std::size_t Index)
	{
		return Vector[Index];
	}

	float const& operator[](std::size_t Index) const
	{
		return Vector[Index];
	}

	FVector4d(float X, float Y, float Z, float W)
	{
		this->Vector[0] = X;
		this->Vector[1] = Y;
		this->Vector[2] = Z;
		this->Vector[3] = W;
	}

	FVector4d(FVector3d const& In)
	{
		this->Vector[0] = In.Vector[0];
		this->Vector[1] = In.Vector[1];
		this->Vector[2] = In.Vector[2];
		this->Vector[3] = 1.f;
	}

	Private::TVector<float, 4> Vector{};
};
