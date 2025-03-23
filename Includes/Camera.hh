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

	explicit FAxisAlignBoundingBox(float const Left, float const Right, float const Bottom, float const Top, float const Near, float const Far)
	{
		this->Left = Left;
		this->Right = Right;
		this->Bottom = Bottom;
		this->Top = Top;
		this->Near = Near;
		this->Far = Far;
	};

	inline FMatrix4x4 CanonicalViewVolume() const
	{
		// @gdemers we follow opengl standard here with the cannonical view [-1,1]
		// and not directx [0,1]
		float const X = (2 / (Right - Left));
		float const Y = (2 / (Top - Bottom));
		float const Z = ((2 * Far * Near) / (Far - Near));
		float const Xt = ((Right + Left) / (Right - Left));
		float const Yt = ((Top + Bottom) / (Top - Bottom));
		float const Zt = ((Far + Near) / (Far - Near));

		// @gdemers be aware that the canonical view undergo perspective division during the perspective projection process
		// which imply that w=-Pwz and will be converted from homogeneous coordinate to carthesien when exiting the vertex shader
		return FMatrix4x4
		{
			Private::TMatrix<float, 4, 4>
			{
				Private::TVector<float, 4>{X,0,Xt,0},
				Private::TVector<float, 4>{0,Y,Yt,0},
				Private::TVector<float, 4>{0,0,Zt,Z},
				Private::TVector<float, 4>{0,0,0,1}
			}
		};
	}

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

	inline FCamera(FTransform const& Transform, FAxisAlignBoundingBox const& ViewVolume, float FieldOfView)
	{
		this->Transform = Transform;
		this->ViewVolume = ViewVolume;
		this->FieldOfView = FieldOfView;
	}

	inline FMatrix4x4 ModelViewMatrix(FTransform const& Object) const
	{
		return this->Transform.Inverse() * Object.ModelMatrix();
	}

	inline FMatrix4x4 OrthographicProjection(FTransform const& Object) const
	{
		return FMatrix4x4{} *this->ModelViewMatrix(Object);
	}

	inline FMatrix4x4 PerspectiveProjection(FTransform const& Object) const
	{
		// @gdemers remember that matrix multiplication goes right-to-left as we are in column-major form
		return this->ViewVolume.CanonicalViewVolume() /*convert 2d point into clip space [-1,1]*/
			* this->PerspectiveDivide() /*project 3d point onto the image plane*/
			* this->ModelViewMatrix(Object) /*put 3d point in camera space (or view space)*/;
	}

	inline FMatrix4x4 PerspectiveDivide() const
	{
		// @gdemers we set r4,z = -1 due to the perspective division.
		// Doing so allow the gpu to convert from-homogeneous-to-carthesien during perspective division run in the vertext shader using it's w component (which is set to -PwZ) and
		// flip our thrid column sign to the correct value.
		return FMatrix4x4
		{
			Private::TMatrix<float, 4, 4>
			{
				Private::TVector<float, 4>{1,0,0,0},
				Private::TVector<float, 4>{0,1,0,0},
				Private::TVector<float, 4>{0,0,1,0},
				Private::TVector<float, 4>{0,0,-1,0}
			}
		};
	}

	FTransform Transform = FTransform::Default;
	FAxisAlignBoundingBox ViewVolume;

	// @gdemers eye opening, distance to image plane, image plane aspect ratio
	float FieldOfView, FocalLength, FilmGateRatio = 0.f;

	FCamera const static Default;
};