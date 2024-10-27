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
#include <Utilities/Viewport.hh>

struct FCamera
{
	FCamera() = default;
	FCamera(FCamera const&) = default;
	FCamera(FCamera&&) = default;
	FCamera& operator=(FCamera const&) = default;

	inline FCamera(FTransform const& Transform, float NearPlane, float FarPlane, float Fov)
	{
		this->Transform = Transform;
		this->NearPlane = NearPlane;
		this->FarPlane = FarPlane;
		this->Fov = Fov;
	}

	inline FMatrix4x4 ModelViewMatrix(FTransform const& Object) const
	{
		// TODO double check left-hand side vs right-hand side if weird behaviour occurs
		return this->Transform.Inverse() * Object.ModelMatrix();
	}

	inline FMatrix4x4 ProjectionMatrix(FTransform const& Object)
	{
		float const X = ((2 * Object.Position[0]/*Pw(x)*/) - FViewport::Application.Left) / ((FViewport::Application.Right - FViewport::Application.Left) * Object.Position[2] /*Pw(z)*/ * tan(Fov * 0.5f));
		float const Y = ((2 * Object.Position[1]/*Pw(y)*/) - FViewport::Application.Bottom) / ((FViewport::Application.Top - FViewport::Application.Bottom) * Object.Position[2] /*Pw(z)*/ * tan(Fov * 0.5f));
		float const Z = ((2 * Object.Position[2]/*Pw(z)*/) - FarPlane) / ((NearPlane - FarPlane) * Object.Position[2] /*Pw(z)*/);

		auto const ProjectionMatrix = FMatrix4x4
		{
			Private::TMatrix<float, 4, 4>
			{
				Private::TVector<float, 4>{X,0,0,-((FViewport::Application.Right + FViewport::Application.Left) / (FViewport::Application.Right - FViewport::Application.Left))},
				Private::TVector<float, 4>{0,Y,0,-((FViewport::Application.Top + FViewport::Application.Bottom) / (FViewport::Application.Top - FViewport::Application.Bottom))},
				Private::TVector<float, 4>{0,0,Z,-((FarPlane + NearPlane) / (NearPlane - FarPlane))},
				Private::TVector<float, 4>{0,0,-1,0}
			}
		};

		return ProjectionMatrix * this->ModelViewMatrix(Object);
	}

	void Modify(FTransform const& In);
	void Reset();

	FTransform Transform = FTransform::Default;
	float NearPlane, FarPlane = 0.f;
	float Fov = 0.f;
	FCamera const static Default;
};