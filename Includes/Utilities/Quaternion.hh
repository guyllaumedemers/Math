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

// @gdemers brief recap on quaternion
// q = a + bi + cj + dk
// https://www.youtube.com/watch?v=jlskQDR8-bY&ab_channel=Mathoma (defining a quaternion)
// https://en.wikipedia.org/wiki/Complex_number (introduction to complex numbers, i^2 = j^2 = k^2 = -1)
// https://www.youtube.com/watch?v=3Ki14CsP_9k&ab_channel=Mathoma (quaternion multiplication)
// https://www.youtube.com/watch?v=ln3vI4JEArc&ab_channel=Mathoma (extracting the dot product and cross product from quaternion)
// matrix form for quaternion multiplication : q1 * q2 = (a + bi + cj + dk) * (e + fi + gj + hk)
// a -b -c -d
// b  a -d  c	* (e + fi + gj + hk)
// c  d  a -b
// d -c  b  a
// https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation (understanding the real part of a quaternion)
// https://www.youtube.com/watch?v=_kmSiU0Ckyg&ab_channel=UofMIntroductiontoComputerGraphics-COMP3490 (other explanation to quaternion)
struct FQuaternion
{
	FQuaternion() = default;
	FQuaternion(FQuaternion const& Rhs) = default;
	FQuaternion(FQuaternion&& Rhs) = default;
	FQuaternion& operator=(FQuaternion const& Rhs) = default;
	FQuaternion& operator=(FQuaternion&& Rhs) = default;

	explicit FQuaternion(FVector4d const& Rhs);

	FQuaternion const operator*(FQuaternion const& Rhs) const;

	FQuaternion const Conjugate() const;
	float const Magnitude() const;
	void Normalize();

	/**
	 * @gdemers im missing pieces of the puzzle here, like how would i provide the angle by which
	 * the quaternion : q1 = cos(angle/2) + sin(angle/2) * (v1)
	 * 
	 * from my understanding, the above unit vector v1 represent the axis by which we want to rotate.
	 * to ensure that the object is rotated with imgui debugger, i would have to use the cached value in FEulerRotation and chain rotation to get a result that calculate the current
	 * rotation for all 3 axis.
	 * 
	 * maybe renaming FEulerRotation to FEulerAngles would make more sense ?
	 * 
	 * would simply providing my object position vector be enought if normalized ?
	 * 
	 * instead of doing : v' = (q1)q2(q1*)
	 * 
	 * q1 = cos(angle/2) + sin(angle/2) * (v1)
	 * q2 = <0, v2>
	 */

	float const& operator[](std::size_t const Rhs) const;
	float& operator[](std::size_t const Rhs);

	FQuaternion const static Zero;
	FQuaternion const static One;

	FVector4d Components = FVector4d::Zero;
};