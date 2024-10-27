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

#include "gtest/gtest.h"

#include "Utilities/Vector.hh"

class TestTVector : public testing::Test
{
protected:
	virtual void SetUp() override
	{
		Vector2d = { 3,4 };
		Vector3d = { 2,3, 6 };
		DotProductVector = { 1,2, 3 };
		CrossProductVector = { 1,2, 3 };
		SegmentVector = { 3,1 };

		// careful about handed-ness
		BoundingBoxSegmentA = { 2,0, 0 };
		BoundingBoxSegmentB = { 0, 2, 0 };
		BoundingBoxSegmentC = { 0,0, 2 };
	}

	virtual void TearDown() override
	{
		// stack allocation, will be released when going out-of-scope
	}

	// target properties
	Private::TVector<float, 2> Vector2d{};
	Private::TVector<float, 3> Vector3d{};
	Private::TVector<float, 3> DotProductVector{};
	Private::TVector<float, 3> CrossProductVector{};
	Private::TVector<float, 2> SegmentVector{};
	Private::TVector<float, 3> BoundingBoxSegmentA{};
	Private::TVector<float, 3> BoundingBoxSegmentB{};
	Private::TVector<float, 3> BoundingBoxSegmentC{};
};

/**
 *	Impl already handle compile-time assertion on non-floating point types.
 *
 *	Vector operation are only possible on types of identical size, otherwise will
 *	fail compilation.
 */

TEST_F(TestTVector, VectorAddition)
{
	// copy
	auto const& Addition2d = (Vector2d + Vector2d);
	for (std::size_t i = 0; i < Addition2d.GetRows(); ++i)
	{
		EXPECT_FLOAT_EQ(Addition2d[i], Vector2d[i] + Vector2d[i]);
	}

	// cache initial value
	auto const CopyVector2d = Vector2d;

	// update by ref
	Vector2d += Vector2d;

	for (std::size_t i = 0; i < CopyVector2d.GetRows(); ++i)
	{
		EXPECT_FLOAT_EQ(Vector2d[i], CopyVector2d[i] + CopyVector2d[i]);
	}

	// copy
	auto const& Addition3d = (Vector3d + Vector3d);
	for (std::size_t i = 0; i < Addition3d.GetRows(); ++i)
	{
		EXPECT_FLOAT_EQ(Addition3d[i], Vector3d[i] + Vector3d[i]);
	}

	// cache initial value
	auto const CopyVector3d = Vector3d;

	// update by ref
	Vector3d += Vector3d;

	for (std::size_t i = 0; i < CopyVector3d.GetRows(); ++i)
	{
		EXPECT_FLOAT_EQ(Vector3d[i], CopyVector3d[i] + CopyVector3d[i]);
	}
}

TEST_F(TestTVector, VectorSubstraction)
{
	// copy
	auto const& Subtraction2d = (Vector2d - Vector2d);
	for (std::size_t i = 0; i < Subtraction2d.GetRows(); ++i)
	{
		EXPECT_FLOAT_EQ(Subtraction2d[i], Vector2d[i] - Vector2d[i]);
	}

	// cache initial value
	auto const CopyVector2d = Vector2d;

	// update by ref
	Vector2d -= Vector2d;

	for (std::size_t i = 0; i < CopyVector2d.GetRows(); ++i)
	{
		EXPECT_FLOAT_EQ(Vector2d[i], CopyVector2d[i] - CopyVector2d[i]);
	}

	// copy
	auto const& Subtraction3d = (Vector3d - Vector3d);
	for (std::size_t i = 0; i < Subtraction3d.GetRows(); ++i)
	{
		EXPECT_FLOAT_EQ(Subtraction3d[i], Vector3d[i] - Vector3d[i]);
	}

	// cache initial value
	auto const CopyVector3d = Vector3d;

	// update by ref
	Vector3d -= Vector3d;

	for (std::size_t i = 0; i < CopyVector3d.GetRows(); ++i)
	{
		EXPECT_FLOAT_EQ(Vector3d[i], CopyVector3d[i] - CopyVector3d[i]);
	}
}

TEST_F(TestTVector, VectorSquaredMagnitude)
{
	EXPECT_FLOAT_EQ(Vector2d.SquaredMagnitude(), 25.f);
	EXPECT_FLOAT_EQ(Vector3d.SquaredMagnitude(), 49.f);
}

TEST_F(TestTVector, VectorMagnitude)
{
	EXPECT_FLOAT_EQ(Vector2d.Magnitude(), 5.f);
	EXPECT_FLOAT_EQ(Vector3d.Magnitude(), 7.f);
}

TEST_F(TestTVector, VectorNormalize)
{
	auto const& Normalize2d = Vector2d.Normalize();
	float const Magnitude2d = Vector2d.Magnitude();

	for (std::size_t i = 0; i < Normalize2d.GetRows(); ++i)
	{
		EXPECT_FLOAT_EQ(Vector2d[i], Normalize2d[i] * Magnitude2d);
	}

	auto const& Normalize3d = Vector3d.Normalize();
	float const Magnitude3d = Vector3d.Magnitude();

	for (std::size_t i = 0; i < Normalize3d.GetRows(); ++i)
	{
		EXPECT_FLOAT_EQ(Vector3d[i], Normalize3d[i] * Magnitude3d);
	}
}

TEST_F(TestTVector, DotProduct)
{
	// also known : scalar product
	EXPECT_FLOAT_EQ(Vector2d.DotProduct(Vector2d), Vector2d.SquaredMagnitude());
	EXPECT_FLOAT_EQ(Vector3d.DotProduct(Vector3d), Vector3d.SquaredMagnitude());
	EXPECT_FLOAT_EQ(Vector3d.DotProduct(DotProductVector), 26.f);
}

TEST_F(TestTVector, CrossProduct)
{
	// also known : vector product
	auto const& ZeroVector = Vector3d.CrossProduct(Vector3d);
	for (std::size_t i = 0; i < ZeroVector.GetRows(); ++i)
	{
		EXPECT_FLOAT_EQ(ZeroVector[i], 0.f);
	}

	auto const& CrossVector = Vector3d.CrossProduct(CrossProductVector);
	EXPECT_FLOAT_EQ(CrossVector[0], -3.f);
	EXPECT_FLOAT_EQ(CrossVector[1], 0.f);
	EXPECT_FLOAT_EQ(CrossVector[2], 1.f);
}

TEST_F(TestTVector, VectorProjection)
{
	auto const& IdentityVector = Vector2d.Projection(Vector2d);
	for (std::size_t i = 0; i < IdentityVector.GetRows(); ++i)
	{
		EXPECT_FLOAT_EQ(IdentityVector[i], Vector2d[i]);
	}

	auto const& ProjectionVector = Vector2d.Projection(SegmentVector);
	EXPECT_FLOAT_EQ(ProjectionVector[0], 3.9f);
	EXPECT_FLOAT_EQ(ProjectionVector[1], 1.3f);
}

TEST_F(TestTVector, VectorRejection)
{
	auto const& ZeroVector = Vector2d.Rejection(Vector2d);
	for (std::size_t i = 0; i < ZeroVector.GetRows(); ++i)
	{
		EXPECT_FLOAT_EQ(ZeroVector[i], 0.f);
	}

	auto const& RejectionVector = Vector2d.Rejection(SegmentVector);
	EXPECT_FLOAT_EQ(RejectionVector[0], -0.9f);
	EXPECT_FLOAT_EQ(RejectionVector[1], 2.7f);
}

TEST_F(TestTVector, ScalarTripleProduct)
{
	float const Volume = FMath::ScalarTripleProduct<float, 3>(
		BoundingBoxSegmentA.Components,
		BoundingBoxSegmentB.Components,
		BoundingBoxSegmentC.Components);

	EXPECT_FLOAT_EQ(Volume, 8.f);
}

TEST_F(TestTVector, VectorTripleProduct)
{
	/*auto const& ResultVector = FMath::VectorTripleProduct<float, 3>(
		BoundingBoxSegmentA.Components,
		BoundingBoxSegmentB.Components,
		BoundingBoxSegmentC.Components);

	for (std::size_t i = 0; i < ResultVector.size(); ++i)
	{
		EXPECT_FLOAT_EQ(ResultVector[i], 8.f);
	}*/
}

