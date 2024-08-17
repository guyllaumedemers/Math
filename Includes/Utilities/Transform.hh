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

#include "Matrix.hh"
#include "Vector.hh"

struct FTransform
{
	FTransform()					= default;
	FTransform(FTransform const&)	= default;
	FTransform(FTransform &&)		= default;

	inline FTransform(FVector3d const& Position, FVector3d const& Rotation, FVector3d const& Scale)
	{
		this->Position	= Position;
		this->Rotation	= Rotation;
		this->Scale		= Scale;
	}

	inline FTransform& operator*=(FTransform const& In)
	{
		Position	= FMatrix4x4::Translate(In.Position)	* Position;
		Rotation	= FMatrix4x4::Rotate(In.Rotation)		* Rotation;
		Scale		= FMatrix4x4::Scale(In.Scale)			* Scale;
		return *this;
	}

	inline FMatrix4x4 ModelMatrix() const
	{
		// TODO double check left-hand side vs right-hand side if weird behaviour occurs
		return (FMatrix4x4::Translate(Position) * (FMatrix4x4::Rotate(Rotation) * FMatrix4x4::Scale(Scale)));
	}

	inline FMatrix4x4 Inverse() const
	{
		FMatrix4x4 const& Matrix = ModelMatrix();
		return Matrix.Adjuvent() * (1.f / Matrix.Determinant());
	}

	FVector3d Position	= FVector3d::Zero;
	// TODO update for quaternions later, if you ever understand them
	FVector3d Rotation	= FVector3d::Zero;
	FVector3d Scale		= FVector3d::One;
	FTransform const static Identity;
};