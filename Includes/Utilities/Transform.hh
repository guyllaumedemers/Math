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

#include <assert.h>

#include "Math.hh"
#include "Matrix.hh"
#include "Vector.hh"
#include "Quaternion.hh"

struct FTransform
{
	FTransform() = default;
	FTransform(FTransform const&) = default;
	FTransform(FTransform&&) = default;
	FTransform& operator=(FTransform const&) = default;

	FTransform(FVector3d const& Position, FVector3d const& Rotation, FVector3d const& Scale)
	{
		this->Position = Position;
		this->Rotation = Rotation;
		this->Scale = Scale;
	}

	FMatrix4x4 ModelMatrix() const
	{
		// TODO double check left-hand side vs right-hand side if weird behaviour occurs
		return (FMatrix4x4::Translate(Position) * (FMatrix4x4::Rotate(Rotation.ToVector()) * FMatrix4x4::Scale(Scale)));
	}

	FMatrix4x4 OrthoNormal() const
	{
		// we have to provide a vector, which represent the orthogonal vector (i.e basis from which other vector are defered from)

		// a second vector is provided and does vector rejection to become orthogonal from A

		// ... n is repeated

		// we normalize

		// we return our new basis
		return {};
	}

	FMatrix4x4 Inverse() const
	{
		FMatrix4x4 const& Matrix = ModelMatrix();

		float const Determinant = Matrix.Determinant();
		if (FMath::IsNearlyZero(Determinant))
		{
			// TODO determine if assertion is the correct approach as in some case, we may want to actually return and continue
			// the execution
			assert(false);
			return FMatrix4x4::Zero();
		}
		else
		{
			return Matrix.Adjugate() * (1.f / Matrix.Determinant());
		}
	}

	FVector4d Position{};
	// TODO update for quaternions later, if you ever understand them
	FQuaternion Rotation{};
	FVector4d Scale{};
	FTransform const static Default;
};