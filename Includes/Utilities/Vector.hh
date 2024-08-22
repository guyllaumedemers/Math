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
	template<typename T, std::size_t N, std::enable_if_t<std::is_floating_point_v<T>, T> = true>
	struct TVector
	{
		static_assert(N != 0, "TVector size is ill format");

		inline float DotProduct(TVector<T, N> const& In) const
		{
			return FMath::DotProduct<T, N>(*this, In);
		}

		inline float Normalize() const
		{
			return FMath::Normalize<T, N>(Components);
		}

		inline float Magnitude() const
		{
			return FMath::Magnitude<T, N>(Components);
		}

		std::array<T, N> Components;
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
		this->X = In;
		this->Y = In;
	}

	inline FVector2d(float X, float Y)
	{
		this->X = X;
		this->Y = Y;
	}

	float X = 0.f;
	float Y = 0.f;
	FVector2d const static Zero;
	FVector2d const static One;
};

struct FVector3d
{
	FVector3d() = default;
	FVector3d(FVector3d const&) = default;
	FVector3d(FVector3d&&) = default;
	FVector3d& operator=(FVector3d const&) = default;

	inline FVector3d(float In)
	{
		this->X = In;
		this->Y = In;
		this->Z = In;
	}

	inline FVector3d(float X, float Y, float Z)
	{
		this->X = X;
		this->Y = Y;
		this->Z = Z;
	}

	inline FVector3d(FVector2d const& In)
	{
		this->X = In.X;
		this->Y = In.Y;
		this->Z = 0.f;
	}

	float X = 0.f;
	float Y = 0.f;
	float Z = 0.f;
	FVector3d const static Zero;
	FVector3d const static One;
};

struct FVector4d
{
	FVector4d() = default;
	FVector4d(FVector4d const&) = default;
	FVector4d(FVector4d&&) = default;
	FVector4d& operator=(FVector4d const&) = default;

	inline FVector4d operator*(float In) const
	{
		return FVector4d{ X * In, Y * In, Z * In, W * In };
	}

	inline FVector4d(float X, float Y, float Z, float W)
	{
		this->X = X;
		this->Y = Y;
		this->Z = Z;
		this->W = W;
	}

	inline FVector4d(FVector3d const& In)
	{
		this->X = In.X;
		this->Y = In.Y;
		this->Z = In.Z;
		this->W = 1.f;
	}

	float X = 0.f;
	float Y = 0.f;
	float Z = 0.f;
	float W = 1.f;
};
