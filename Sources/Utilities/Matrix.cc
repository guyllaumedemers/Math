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

FMatrix4x4::FMatrix4x4(Private::TMatrix<float, 4, 4> const& Matrix)
{
	this->Matrix = Matrix;
}

FMatrix4x4 FMatrix4x4::operator*(float In) const
{
	return FMatrix4x4{ Matrix * In };
}

FMatrix4x4 FMatrix4x4::operator*(FMatrix4x4 const& In) const
{
	return FMatrix4x4{ Matrix * In.Matrix };
}

FVector4d FMatrix4x4::operator*(FVector4d const& In) const
{
	return FVector4d{ Matrix * In.Vector };
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

FMatrix4x4 FMatrix4x4::Translate(FVector4d const& Translate)
{
	return FMatrix4x4
	{
		Private::TMatrix<float, 4, 4>
		{
			Private::TVector<float, 4>{1,0,0,Translate[0]},
			Private::TVector<float, 4>{0,1,0,Translate[1]},
			Private::TVector<float, 4>{0,0,1,Translate[2]},
			Private::TVector<float, 4>{0,0,0,1}
		}
	};
}

FMatrix4x4 FMatrix4x4::Rotate(FVector4d const& Rotate)
{
	return {};
}

FMatrix4x4 FMatrix4x4::Scale(FVector4d const& Scale)
{
	return FMatrix4x4
	{
		Private::TMatrix<float, 4, 4>
		{
			Private::TVector<float, 4>{Scale[0],0,0,0},
			Private::TVector<float, 4>{0,Scale[1],0,0},
			Private::TVector<float, 4>{0,0,Scale[2],0},
			Private::TVector<float, 4>{0,0,0,1}
		}
	};
}
