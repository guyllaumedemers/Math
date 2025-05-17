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

#include "Utilities/Transform.hh"

// static
FTransform const FTransform::Default = FTransform{
	FVector3d(0.f),
	FQuaternion(),
	FVector3d(1.f)
};

FTransform::FTransform(FVector3d const& aPosition,
	FQuaternion const& aRotation,
	FVector3d const& aScale) :
	Position(aPosition),
	Rotation(aRotation),
	Scale(aScale)
{
}

FMatrix4x4 const FTransform::getModelMatrix() const
{
	// TODO @gdemers add missing support for rotation
	//return FMatrix4x4::Scale(Scale) * FQuaternion::ToMatrix(Rotation) * FMatrix4x4::Translate(Position);
	return FMatrix4x4::Scale(Scale) * FMatrix4x4::Translate(Position);
}

FMatrix4x4 const FTransform::OrthoNormal() const
{
	// we have to provide a vector, which represent the orthogonal vector (i.e basis from which other vector are defered from)

	// a second vector is provided and does vector rejection to become orthogonal from A

	// ... n is repeated

	// we normalize

	// we return our new basis
	return {};
}

FMatrix4x4 const FTransform::Inverse() const
{
	FMatrix4x4 const& Matrix = getModelMatrix();

	float const Determinant = Matrix.Determinant();
	if (FMath::IsNearlyZero(Determinant))
	{
		// TODO determine if assertion is the correct approach as in some case, we may want to actually return and continue
		// the execution
		//assert(false);
		return FMatrix4x4::Zero();
	}
	else
	{
		return Matrix.Adjugate() * (1.f / Matrix.Determinant());
	}
}