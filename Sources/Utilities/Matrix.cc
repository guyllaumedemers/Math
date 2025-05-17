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

#include "Utilities/Matrix.hh"

FMatrix4x4::FMatrix4x4(Private::TMatrix<float, 4, 4> const& Rhs)
{
	this->Matrix = Rhs;
}

FMatrix4x4 FMatrix4x4::operator*(float const Rhs) const
{
	return FMatrix4x4{ Matrix * Rhs };
}

FMatrix4x4 FMatrix4x4::operator*(FMatrix4x4 const& Rhs) const
{
	return FMatrix4x4{ Matrix * Rhs.Matrix };
}

FMatrix4x4& FMatrix4x4::operator*=(FMatrix4x4 const& Rhs)
{
	this->Matrix = this->Matrix * Rhs.Matrix;
	return *this;
}

FVector4d FMatrix4x4::operator*(FVector4d const& Rhs) const
{
	return FVector4d{ Matrix * Rhs.Vector };
}

FMatrix4x4 FMatrix4x4::Adjugate() const
{
	return FMatrix4x4{ Matrix.CalculateAdjugate() };
}

float FMatrix4x4::Determinant() const
{
	return Matrix.CalculateDeterminant();
}

FMatrix4x4 FMatrix4x4::Zero()
{
	return FMatrix4x4
	{
		Private::TMatrix<float, 4, 4>{}
	};
}

FMatrix4x4 FMatrix4x4::Identity()
{
	return FMatrix4x4
	{
		Private::TMatrix<float, 4, 4>
		{
			Private::TVector<float, 4>{1,0,0,0},
			Private::TVector<float, 4>{0,1,0,0},
			Private::TVector<float, 4>{0,0,1,0},
			Private::TVector<float, 4>{0,0,0,1}
		}
	};
}

FMatrix4x4 FMatrix4x4::Translate(FVector3d const& Rhs)
{
	return FMatrix4x4
	{
		Private::TMatrix<float, 4, 4>
		{
			Private::TVector<float, 4>{1,0,0,Rhs[0]},
			Private::TVector<float, 4>{0,1,0,Rhs[1]},
			Private::TVector<float, 4>{0,0,1,Rhs[2]},
			Private::TVector<float, 4>{0,0,0,1}
		}
	};
}

FMatrix4x4 FMatrix4x4::Rotate(FVector3d const& Rhs)
{
	return {};
}

FMatrix4x4 FMatrix4x4::Scale(FVector3d const& Rhs)
{
	return FMatrix4x4
	{
		Private::TMatrix<float, 4, 4>
		{
			Private::TVector<float, 4>{Rhs[0],0,0,0},
			Private::TVector<float, 4>{0,Rhs[1],0,0},
			Private::TVector<float, 4>{0,0,Rhs[2],0},
			Private::TVector<float, 4>{0,0,0,1}
		}
	};
}
