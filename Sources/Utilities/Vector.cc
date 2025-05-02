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

FVector2d::FVector2d(float X, float Y)
{
	this->Vector[0] = X;
	this->Vector[1] = Y;
}

FVector2d::FVector2d(float In)
{
	this->Vector[0] = In;
	this->Vector[1] = In;
}

FVector3d::FVector3d(FVector2d const& In)
{
	this->Vector[0] = In.Vector[0];
	this->Vector[1] = In.Vector[1];
	this->Vector[2] = 1.f;
}

FVector3d::FVector3d(float X, float Y, float Z)
{
	this->Vector[0] = X;
	this->Vector[1] = Y;
	this->Vector[2] = Z;
}

FVector3d::FVector3d(float In)
{
	this->Vector[0] = In;
	this->Vector[1] = In;
	this->Vector[2] = In;
}

FVector4d::FVector4d(Private::TVector<float, 4> const& Vector)
{
	this->Vector = Vector;
}

FVector4d::FVector4d(float X, float Y, float Z, float W)
{
	this->Vector[0] = X;
	this->Vector[1] = Y;
	this->Vector[2] = Z;
	this->Vector[3] = W;
}

FVector4d::FVector4d(FVector3d const& In)
{
	this->Vector[0] = In.Vector[0];
	this->Vector[1] = In.Vector[1];
	this->Vector[2] = In.Vector[2];
	this->Vector[3] = 1.f;
}

FVector4d FVector4d::operator*(float In) const
{
	// @gdemers we can argue on if we want allow scaling of the W component later
	return FVector4d
	{
		Vector[0] * In,
		Vector[1] * In,
		Vector[2] * In,
		1.f,
	};
}

float const& FVector4d::operator[](std::size_t Index) const
{
	return Vector[Index];
}

float& FVector4d::operator[](std::size_t Index)
{
	return Vector[Index];
}
