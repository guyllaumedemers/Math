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

#include "Utilities/Matrix.hh"
#include "Utilities/Transform.hh"

// define the view frustum of a camera
struct FAxisAlignBoundingBox
{
	FAxisAlignBoundingBox() = default;

	explicit FAxisAlignBoundingBox(float const aLeft,
		float const aRight,
		float const aBottom,
		float const aTop,
		float const aNear,
		float const aFar);

	FMatrix4x4 const CanonicalViewVolume() const;

	// @gdemers properties that define the bounds of the view volume
	float Left, Right, Top, Bottom, Near, Far = 0.f;
};

// define a target from which we can see the world. Allow conversion from 3d space to 2d
struct FCamera
{
	FCamera() = default;
	FCamera(FCamera const& Rhs) = default;
	FCamera(FCamera&& Rhs) = default;
	FCamera& operator=(FCamera const& Rhs) = default;

	explicit FCamera(FTransform const& aTransform,
		FAxisAlignBoundingBox const& aViewVolume,
		float const aFieldOfView,
		float const aFocalLength,
		float const aFilmGateRatio);

	FMatrix4x4 const ModelViewMatrix(FTransform const& Object) const;
	FMatrix4x4 const OrthographicProjection() const;
	FMatrix4x4 const PerspectiveProjection() const;
	FMatrix4x4 const PerspectiveDivide(float const Far, float const Near) const;

	FTransform Transform = FTransform::Default;
	FAxisAlignBoundingBox ViewVolume;

	// @gdemers eye opening, distance to image plane, image plane aspect ratio
	float FieldOfView, FocalLength, FilmGateRatio = 0.f;

	FCamera const static Default;
};