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

#include "Vector.hh"

#include <atomic>

namespace Private
{
	// extern utils
	extern float CalculateDeterminant(std::size_t Size, float** Matrix);
	extern float** CalculateAdjugate(std::size_t Size, float** Matrix);
}

// homogeneous coordinate
struct FMatrix4x4
{
	FMatrix4x4()					= default;
	FMatrix4x4(FMatrix4x4 const&)	= default;
	FMatrix4x4(FMatrix4x4&&)		= default;

	inline FMatrix4x4 operator*(FMatrix4x4 const& In) const
	{
		// TODO double check left-hand side vs right-hand side if weird behaviour occurs
		return FMatrix4x4
		{
			FVector4d{ *this * In.Rows[0] },
			FVector4d{ *this * In.Rows[1] },
			FVector4d{ *this * In.Rows[2] },
			FVector4d{ *this * In.Rows[3] }
		};
	}

	inline FVector4d operator*(FVector4d const& In) const
	{
		// TODO handle differently register load/access, this is un-optimized
		return FVector4d
		{
			(Rows[0].X * In.X) + (Rows[1].X * In.X) + (Rows[2].X * In.X) + (Rows[3].X * In.X),
			(Rows[0].Y * In.Y) + (Rows[1].Y * In.Y) + (Rows[2].Y * In.Y) + (Rows[3].Y * In.Y),
			(Rows[0].Z * In.Z) + (Rows[1].Z * In.Z) + (Rows[2].Z * In.Z) + (Rows[3].Z * In.Z),
			(Rows[0].W * In.W) + (Rows[1].W * In.W) + (Rows[2].W * In.W) + (Rows[3].W * In.W)
		};
	}

	// TODO verify that truncation is a valid operation since matrix mult
	// required num col x row to be equal and output row x col for result
	inline FVector3d operator*(FVector3d const& In) const
	{
		// TODO handle differently register load/access, this is un-optimized
		return FVector3d
		{
			(Rows[0].X * In.X) + (Rows[1].X * In.X) + (Rows[2].X * In.X) + (Rows[3].X * In.X),
			(Rows[0].Y * In.Y) + (Rows[1].Y * In.Y) + (Rows[2].Y * In.Y) + (Rows[3].Y * In.Y),
			(Rows[0].Z * In.Z) + (Rows[1].Z * In.Z) + (Rows[2].Z * In.Z) + (Rows[3].Z * In.Z)
		};
	}

	inline FMatrix4x4 operator*(float In) const
	{
		return FMatrix4x4
		{
			FVector4d{Rows[0] * In},
			FVector4d{Rows[1] * In},
			FVector4d{Rows[2] * In},
			FVector4d{Rows[3] * In}
		};
	}

	inline float Determinant() const
	{
		return Private::CalculateDeterminant(4, this->ToArray());
	}

	inline FMatrix4x4 Adjugate() const
	{
		float** Result = Private::CalculateAdjugate(4, this->ToArray());
		return FromArray(Result);
	}

	inline float** ToArray() const
	{
		// TODO fix this! cannot return a ptr-to-ptr without allocating on the heap
		// otherwise the ptr-to-ptr will return undef behaviour
		float** Array = nullptr;
		return Array;
	}

	inline FMatrix4x4 FromArray(float** In) const
	{
		return FMatrix4x4();
	}

	// custom matrix function exposing target mutation applied in vector space
	static FMatrix4x4 Zero();
	static FMatrix4x4 Identity();
	static FMatrix4x4 Translate(FVector4d const& Translate);
	static FMatrix4x4 Rotate(FVector4d const& Rotate);
	static FMatrix4x4 Scale(FVector4d const& Scale);

	FVector4d Rows[4];
};
