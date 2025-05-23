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

#include "Math.hh"
#include "Matrix.hh"

// @gdemers building intuition for understanding rotation in R2/R3.
// https://www.youtube.com/watch?v=lPWfIq5DzqI&ab_channel=KhanAcademy (deriving rotation matrix from the identity matrix and trigonometry)
// https://www.youtube.com/watch?v=R0EQg9vgbQw&ab_channel=KhanAcademy (angle sum identity)
struct FEulerRotation
{
	FEulerRotation() = default;
	FEulerRotation(FEulerRotation const& Rhs) = default;
	FEulerRotation(FEulerRotation&& Rhs) = default;
	FEulerRotation& operator=(FEulerRotation const& Rhs) = default;
	FEulerRotation& operator=(FEulerRotation&& Rhs) = default;

	static FMatrix4x4 const RotateX(float const Angle);
	static FMatrix4x4 const RotateY(float const Angle);
	static FMatrix4x4 const RotateZ(float const Angle);

	FMatrix4x4 const EulerRotation() const;

	float const& operator[](std::size_t const Rhs) const;
	float& operator[](std::size_t const Rhs);

	FEulerRotation const static Zero;

	FVector3d EulerAngles = FVector3d::Zero;
};
