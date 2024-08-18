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

struct FCamera
{
	FCamera()				= default;
	FCamera(FCamera const&) = default;
	FCamera(FCamera&&)		= default;
	FCamera& operator=(FCamera const&) = default;

	inline FCamera(FTransform const& Transform, float NearPlane, float FarPlane, float Fov)
	{
		this->Transform = Transform;
		this->NearPlane = NearPlane;
		this->FarPlane = FarPlane;
		this->Fov = Fov;
	}

	inline FMatrix4x4 ModelViewMatrix(FTransform const& In) const
	{
		// TODO double check left-hand side vs right-hand side if weird behaviour occurs
		return Transform.Inverse() * In.ModelMatrix();
	}

	void Modify(FTransform const& In);
	void Reset();

	FTransform Transform = FTransform::Default;
	float NearPlane, FarPlane = 0.f;
	float Fov = 0.f;
	FCamera const static Default;
};