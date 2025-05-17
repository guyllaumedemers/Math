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

#include "Utilities/Vector.hh"

// static
FVector2d const FVector2d::Zero = FVector2d(0.f);
FVector2d const FVector2d::One = FVector2d(1.f);
FVector3d const FVector3d::Zero = FVector3d(0.f);
FVector3d const FVector3d::One = FVector3d(1.f);

FVector2d::FVector2d(float const X, float const Y) :
	Vector(Private::TVector<float, 2>{X, Y})
{
}

FVector2d::FVector2d(float const Rhs) :
	Vector(Private::TVector<float, 2>{Rhs, Rhs})
{
}

FVector3d::FVector3d(FVector2d const& Rhs) :
	Vector(Private::TVector<float, 3>{Rhs.Vector[0], Rhs.Vector[1], 1.f})
{
}

FVector3d::FVector3d(float const X, float const Y, float const Z) :
	Vector(Private::TVector<float, 3>{X, Y, Z})
{
}

FVector3d::FVector3d(float const Rhs) :
	Vector(Private::TVector<float, 3>{Rhs, Rhs, Rhs})
{
}

FVector3d& FVector3d::operator+=(FVector3d const& Rhs)
{
	this->Vector[0] += Rhs[0];
	this->Vector[1] += Rhs[1];
	this->Vector[2] += Rhs[2];
	return *this;
}

float const& FVector3d::operator[](std::size_t const Rhs) const
{
	return Vector[Rhs];
}

float& FVector3d::operator[](std::size_t const Rhs)
{
	return Vector[Rhs];
}

FVector4d::FVector4d(Private::TVector<float, 4> const& Rhs) :
	Vector(Rhs)
{
}

FVector4d::FVector4d(float const X, float const Y, float const Z, float const W) :
	Vector(Private::TVector<float, 4>{X, Y, Z, W})
{
}

FVector4d::FVector4d(FVector3d const& Rhs) :
	Vector(Private::TVector<float, 4>{Rhs.Vector[0], Rhs.Vector[1], Rhs.Vector[2], 1.f})
{
}

FVector4d const FVector4d::operator*(float const Rhs) const
{
	// @gdemers we can argue on if we want allow scaling of the W component later
	return FVector4d
	{
		Vector[0] * Rhs,
		Vector[1] * Rhs,
		Vector[2] * Rhs,
		1.f,
	};
}

float const& FVector4d::operator[](std::size_t const Rhs) const
{
	return Vector[Rhs];
}

float& FVector4d::operator[](std::size_t const Rhs)
{
	return Vector[Rhs];
}
