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

#include "Utilities/Euler.hh"

// static
FEulerRotation FEulerRotation::Zero;

FMatrix4x4 const FEulerRotation::EulerRotation() const
{
	return FEulerRotation::RotateZ(EulerAngles[2]) * FEulerRotation::RotateY(EulerAngles[1]) * FEulerRotation::RotateX(EulerAngles[0]);
}

FMatrix4x4 const FEulerRotation::RotateX(float const Angle)
{
	return FMatrix4x4
	{
		Private::TMatrix<float, 4, 4>
		{
			Private::TVector<float, 4>{1,0,0,0},
			Private::TVector<float, 4>{0,FMath::Cos(Angle),-FMath::Sin(Angle),0},
			Private::TVector<float, 4>{0,FMath::Sin(Angle),FMath::Cos(Angle),0},
			Private::TVector<float, 4>{0,0,0,1}
		}
	};
}

FMatrix4x4 const FEulerRotation::RotateY(float const Angle)
{
	return FMatrix4x4
	{
		Private::TMatrix<float, 4, 4>
		{
			Private::TVector<float, 4>{FMath::Cos(Angle),0,FMath::Sin(Angle),0},
			Private::TVector<float, 4>{0,1,0,0},
			Private::TVector<float, 4>{-FMath::Sin(Angle),0,FMath::Cos(Angle),0},
			Private::TVector<float, 4>{0,0,0,1}
		}
	};
}

FMatrix4x4 const FEulerRotation::RotateZ(float const Angle)
{
	return FMatrix4x4
	{
		Private::TMatrix<float, 4, 4>
		{
			Private::TVector<float, 4>{FMath::Cos(Angle),-FMath::Sin(Angle),0,0},
			Private::TVector<float, 4>{FMath::Sin(Angle),FMath::Cos(Angle),0,0},
			Private::TVector<float, 4>{0,0,1,0},
			Private::TVector<float, 4>{0,0,0,1}
		}
	};
}

float const& FEulerRotation::operator[](std::size_t const Rhs) const
{
	return EulerAngles[Rhs];
}

float& FEulerRotation::operator[](std::size_t const Rhs)
{
	return EulerAngles[Rhs];
}
