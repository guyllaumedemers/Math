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

#include "Utilities/Transform.hh"
#include "Utilities/Viewport.hh"

// define the view frustum of a camera
struct FAxisAlignBoundingBox
{
	FAxisAlignBoundingBox() = default;

	explicit FAxisAlignBoundingBox(float const Left, float const Right, float const Bottom, float const Top, float const Near, float const Far);

	inline FMatrix4x4 CanonicalViewVolume() const;

	// @gdemers properties that define the bounds of the view volume
	float Left, Right, Top, Bottom, Near, Far = 0.f;
};

// define a target from which we can see the world. Allow conversion from 3d space to 2d
struct FCamera
{
	FCamera() = default;
	FCamera(FCamera const&) = default;
	FCamera(FCamera&&) = default;
	FCamera& operator=(FCamera const&) = default;

	explicit FCamera(FTransform const& Transform, FAxisAlignBoundingBox const& ViewVolume, float FieldOfView);

	inline FMatrix4x4 ModelViewMatrix(FTransform const& Object) const;
	inline FMatrix4x4 OrthographicProjection(FTransform const& Object) const;
	inline FMatrix4x4 PerspectiveProjection(FTransform const& Object) const;
	inline FMatrix4x4 PerspectiveDivide(float const Far, float const Near) const;

	FTransform Transform = FTransform::Default;
	FAxisAlignBoundingBox ViewVolume;

	// @gdemers eye opening, distance to image plane, image plane aspect ratio
	float FieldOfView, FocalLength, FilmGateRatio = 0.f;

	FCamera const static Default;
};