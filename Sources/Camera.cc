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

// @gdemers
// note : AABB, if two bounds cancel each other, then it's sitting at the origin for this axis.
// scaling still happen, but the view volume doesn't have to execute translation.
// also, note that the scaling will result in the reciprocal of the range * 2.
// example : [-10,10] = 20. reciprocal: 1/20, times 2; 2/20 or 1/10, which is how much we scale our world point by.
// Pw(x) = 5; 5 * 1/10 = 5/10; Ps(s) = 0.5 which is within the bounds of our cannonical view [-1,1].
FCamera const FCamera::Default = FCamera
{
	FTransform{ FVector3d(0.f), FQuaternion(), FVector3d(1.f)},
	FAxisAlignBoundingBox(-10.f, 10.f, -10.f, 10.f, -10.f, 10.f),
	45.f,
	1.f,
	1.f
};

FAxisAlignBoundingBox::FAxisAlignBoundingBox(float const aLeft,
	float const aRight,
	float const aBottom,
	float const aTop,
	float const aNear,
	float const aFar) :
	Left(aLeft),
	Right(aRight),
	Bottom(aBottom),
	Top(aTop),
	Near(aNear),
	Far(aFar)
{
}

FMatrix4x4 const FAxisAlignBoundingBox::CanonicalViewVolume() const
{
	// @gdemers here, we are remapping our view volume defined using (l, r, b, t, f, n) into our canonical view volume.
	// we follow opengl standard here with the cannonical view [-1,1] and not directx [0,1].
	float const X = (2 / (Right - Left));
	float const Y = (2 / (Top - Bottom));
	float const Z = (-2 / (Far - Near));
	float const Xt = (-(Right + Left) / (Right - Left));
	float const Yt = (-(Top + Bottom) / (Top - Bottom));
	float const Zt = (-(Far + Near) / (Far - Near));

	return FMatrix4x4
	{
		Private::TMatrix<float, 4, 4>
		{
			Private::TVector<float, 4>{X,0,0,Xt},
			Private::TVector<float, 4>{0,Y,0,Yt},
			Private::TVector<float, 4>{0,0,Zt,Z},
			Private::TVector<float, 4>{0,0,0,1}
		}
	};
}

FCamera::FCamera(FTransform const& aTransform,
	FAxisAlignBoundingBox const& aViewVolume,
	float const aFieldOfView,
	float const aFocalLength,
	float const aFilmGateRatio) :
	Transform(aTransform),
	ViewVolume(aViewVolume),
	FieldOfView(aFieldOfView),
	FocalLength(aFocalLength),
	FilmGateRatio(aFilmGateRatio)
{
}

FMatrix4x4 const FCamera::ModelViewMatrix(FTransform const& Object) const
{
	// @gdemers both object and camera model matrix are in the same coordinate space (world). to be able to retrieve the vertices information
	// of our object in regard to the camera origin (and not the world), we have to inverse the camera model matrix. doing so will remove any translation, rotation
	// and scale and put the camera model matrix at the origin of the world. matrix multiplication between the returned inverse matrix and the object model matrix
	// will create a new matrix that illustrate the object transforms in regard to the camera coordinate space.
	// note : this doesn't change the world position of our initial object, or of the camera, this simply provide a new set of information useful for calculating vertex
	// positions in regard to a coordinate space.
	return this->Transform.Inverse() * Object.ModelMatrix();
}

FMatrix4x4 const FCamera::OrthographicProjection() const
{
	// @gdemers #2 and when you think about it, it makes sense as we expect this projection type to keep true scale.
	// while we can visuallize our perspective projection frustum as a pyramid, our orthographic view volume is instead a box, whose shape is defined
	// by the user, and in the end remapped to the canonical view [-1,1].
	return this->ViewVolume.CanonicalViewVolume();
}

FMatrix4x4 const FCamera::PerspectiveProjection() const
{
	// @gdemers something troubling in my initial understanding of projection as a concept is how the mathematical process from which we remapped
	// our view volume for both orthographic and perspective differ.
	// and in reality, they don't!
	// our perspective projection build on top of the process from which we remap our othrographic view volume to the canonical one.
	// the difference here lay at the matrix level which encapsulate perspective division through the expression stated in the function below.
	// our goal here is merely to emulate points converging from 3d space toward a user point of view (something orthographic projection doesn't do)
	// and create a sense of depth with objects in our fictional world.
	// goto #2
	return this->ViewVolume.CanonicalViewVolume() * this->PerspectiveDivide(this->ViewVolume.Far, this->ViewVolume.Near);
}

FMatrix4x4 const FCamera::PerspectiveDivide(float const Far, float const Near) const
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
