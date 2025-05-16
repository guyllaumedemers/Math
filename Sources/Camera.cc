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

#include "Camera.hh"

// static
FCamera const FCamera::Default = FCamera
{
	FTransform{ FVector3d(0.f), FQuaternion(), FVector3d(1.f)},
	FAxisAlignBoundingBox(),
	0.f
};

FAxisAlignBoundingBox::FAxisAlignBoundingBox(float const Left, float const Right, float const Bottom, float const Top, float const Near, float const Far)
{
	this->Left = Left;
	this->Right = Right;
	this->Bottom = Bottom;
	this->Top = Top;
	this->Near = Near;
	this->Far = Far;
}

FMatrix4x4 FAxisAlignBoundingBox::CanonicalViewVolume() const
{
	// @gdemers here, we are remapping our view volume defined using (l, r, b, t, f, n) into our canonical view volume.
	// we follow opengl standard here with the cannonical view [-1,1] and not directx [0,1].
	float const X = (2 / (Right - Left));
	float const Y = (2 / (Top - Bottom));
	float const Z = (2 / (Far - Near));
	float const Xt = ((Right + Left) / (Right - Left));
	float const Yt = ((Top + Bottom) / (Top - Bottom));
	float const Zt = ((Far + Near) / (Far - Near));

	return FMatrix4x4
	{
		Private::TMatrix<float, 4, 4>
		{
			Private::TVector<float, 4>{X,0,0,Xt},
			Private::TVector<float, 4>{0,Y,0,Yt},
			Private::TVector<float, 4>{0,0,Z,Zt},
			Private::TVector<float, 4>{0,0,0,1}
		}
	};
}

FCamera::FCamera(FTransform const& Transform, FAxisAlignBoundingBox const& ViewVolume, float FieldOfView)
{
	this->Transform = Transform;
	this->ViewVolume = ViewVolume;
	this->FieldOfView = FieldOfView;
}

FMatrix4x4 FCamera::ModelViewMatrix(FTransform const& Object) const
{
	return this->Transform.Inverse() * Object.getModelMatrix();
}

FMatrix4x4 FCamera::OrthographicProjection(FTransform const& Object) const
{
	// @gdemers #2 and when you think about it, it makes sense as we expect this projection type to keep true scale.
	// while we can visuallize our perspective projection frustum as a pyramid, our orthographic view volume is instead a box, whose shape is defined
	// by the user, and in the end remapped to the canonical view [-1,1].
	return this->ViewVolume.CanonicalViewVolume()
		* this->ModelViewMatrix(Object);
}

FMatrix4x4 FCamera::PerspectiveProjection(FTransform const& Object) const
{
	// @gdemers something troubling in my initial understanding of projection as a concept is how the mathematical process from which we remapped
	// our view volume for both orthographic and perspective differ.
	// and in reality, they don't!
	// our perspective projection build on top of the process from which we remap our othrographic view volume to the canonical one.
	// the difference here lay at the matrix level which encapsulate perspective division through the expression stated in the function below.
	// our goal here is merely to emulate points converging from 3d space toward a user point of view (something orthographic projection doesn't do)
	// and create a sense of depth with objects in our fictional world.
	// goto #2
	return this->ViewVolume.CanonicalViewVolume()
		* this->PerspectiveDivide(this->ViewVolume.Far, this->ViewVolume.Near)
		* this->ModelViewMatrix(Object);
}

FMatrix4x4 FCamera::PerspectiveDivide(float const Far, float const Near) const
{
	// TODO double check math again, your matrix multiplication may not be right in the end. tbd!

	// @gdemers using our formula for perspective division, we can generate our matrix
	// for the following expression : Psx = (g * Pwx) / -Pwz & Psy = (g * Pwy) / -Pwz.
	// additionally, our depth information has to remain in order to run early z-depth calculation
	// in the fragment shader and handle fragment priority.
	// furthermore, by setting M[3][2] = -1, our world point w-component will become -Pwz during our matrix-vector multiplication
	// which confirm the above expression.
	// however, our world point z-component will lose it's property and z-depth calculation won't be possible as
	// perspective division will be executed in the vertex shader output.
	// to account for this, our world point z-component has to equal z^2 which imply that the equation for our two unknowns (A & B)
	// is as follow : Az + Bw = z^2. like Obi-Wan kenobi says: Hello there! (it's a quadratic equation)
	// to solve this equation, we have to setup rule from which we stipulate : z=-n, z=-f;
	// note : that the following realization raise the problem of "perspective correctness", from which we state that points laying elsewhere, other than
	// the near and far plane are non-linear.
	// 
	// reminder that w-component equals 1 unit here.
	// A*-f + B*w = -f^2 (smallest value)
	// A*-n + B*w = -n^2 (biggest  value)
	// after solving for both: A= (f - n), B= -(fn)
	const float A = (Far - Near);
	const float B = -(Far * Near);
	return FMatrix4x4
	{
		Private::TMatrix<float, 4, 4>
		{
			Private::TVector<float, 4>{FocalLength /*distance to image plane*/,0,0,0},
			Private::TVector<float, 4>{0,FocalLength,0,0},
			Private::TVector<float, 4>{0,0,A,B},
			Private::TVector<float, 4>{0,0,-1,0}
		}
	};
}
