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

FMatrix4x4 FMatrix4x4::Identity()
{
	return FMatrix4x4
	{
		FVector4d(1.f, 0.f, 0.f, 0.f),
		FVector4d(0.f, 1.f, 0.f, 0.f),
		FVector4d(0.f, 0.f, 1.f, 0.f),
		FVector4d(0.f, 0.f, 0.f, 1.f)
	};
}

FMatrix4x4 FMatrix4x4::Translate(FVector4d const& Translate)
{
	return FMatrix4x4
	{
		FVector4d(1.f, 0.f, 0.f, 0.f),
		FVector4d(0.f, 1.f, 0.f, 0.f),
		FVector4d(0.f, 0.f, 1.f, 0.f),
		FVector4d(Translate.X, Translate.Y, Translate.Z, 1.f)
	};
}

FMatrix4x4 FMatrix4x4::Rotate(FVector4d const& Rotate)
{
	// TODO impl rotation matrix
	return FMatrix4x4::Identity();
}

FMatrix4x4 FMatrix4x4::Scale(FVector4d const& Scale)
{
	return FMatrix4x4
	{
		FVector4d(Scale.X, 0.f, 0.f, 0.f),
		FVector4d(0.f, Scale.Y, 0.f, 0.f),
		FVector4d(0.f, 0.f, Scale.Z, 0.f),
		FVector4d(0.f, 0.f, 0.f, 1.f)
	};
}
