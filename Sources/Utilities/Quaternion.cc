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

#include "Utilities/Quaternion.hh"

// static
FQuaternion const FQuaternion::Zero = FQuaternion{ FVector4d(0.f) };
FQuaternion const FQuaternion::One = FQuaternion{ FVector4d(0.f, 0.f, 0.f, 1.f) };

FQuaternion::FQuaternion(FVector4d const& Rhs) :
	Components(Rhs)
{
}

FQuaternion const FQuaternion::operator*(FQuaternion const& Rhs) const
{
	float const A = Components[3];	// w
	float const B = Components[0];	// i
	float const C = Components[1];	// j
	float const D = Components[2];	// k

	// @gdemers matrix representation of hamilton product
	auto const Lhs = FMatrix4x4
	{
		Private::TMatrix<float, 4, 4>
		{
			Private::TVector<float, 4>{A,-B,-C,-D},
			Private::TVector<float, 4>{B,A,-D,C},
			Private::TVector<float, 4>{C,D,A,-B},
			Private::TVector<float, 4>{D,-C,B,A}
		}
	};

	return FQuaternion(Lhs * Rhs.Components);
}

FQuaternion const FQuaternion::Conjugate() const
{
	return FQuaternion{ FVector4d(-Components[0], -Components[1], -Components[2], Components[3]) };
}

float const FQuaternion::Magnitude() const
{
	FQuaternion const Quaternion = ((*this) * this->Conjugate());
	return FMath::Magnitude(Quaternion.Components.Vector.Components);
}

void FQuaternion::Normalize()
{
	float const Magnitude = (1.f / this->Magnitude());
	for (std::size_t i = 0; i < Components.Vector.GetRows(); ++i)
	{
		Components[i] *= Magnitude;
	}
}

float const& FQuaternion::operator[](std::size_t const Rhs) const
{
	return Components[Rhs];
}

float& FQuaternion::operator[](std::size_t const Rhs)
{
	return Components[Rhs];
}