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
#include "Quaternion.hh"
#include "Vector.hh"

struct FTransform
{
	FTransform() = default;
	FTransform(FTransform const&) = default;
	FTransform(FTransform&&) = default;
	FTransform& operator=(FTransform const&) = default;

	explicit FTransform(FVector3d const& Position, FQuaternion const& Rotation, FVector3d const& Scale);

	FMatrix4x4 const getModelMatrix() const;
	FMatrix4x4 OrthoNormal() const;
	FMatrix4x4 Inverse() const;

	// @gdemers cache properties as such and rebuild matrix based on data.
	// note : it wouldnt be possible to cache a single matrix and update property fields. matrix multiplication
	// prevents it.
	FQuaternion Rotation = FQuaternion::Zero;
	FVector3d Position = FVector3d::Zero;
	FVector3d Scale = FVector3d::Zero;

	FTransform const static Default;
};