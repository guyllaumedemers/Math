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

#include <array>
#include <atomic>
#include <math.h>

struct FMath
{
	inline static bool IsNearlyZero(float In)
	{
		// TODO do proper zero check
		return In == 0.f;
	}

	// src : https://en.wikipedia.org/wiki/Dot_product
	// description : the dot product or scalar product is an algebraic operation that takes two equal-length sequences of numbers (usually coordinate vectors), and returns a single number.
	template<typename T, std::size_t N>
	static float DotProduct(std::array<T, N> const& VectorA, std::array<T, N> const& VectorB);

	template<typename T, std::size_t N>
	static std::array<T, N> Normalize(std::array<T, N> const& Vector);

	template <typename T, std::size_t N>
	static T SquaredMagnitude(std::array<T, N> const& Vector);

	template<typename T, std::size_t N>
	static T Magnitude(std::array<T, N> const& Vector);

	template<typename T>
	static T Sqrt(T In);
};

template <typename T, std::size_t N>
float FMath::DotProduct(std::array<T, N> const& VectorA, std::array<T, N> const& VectorB)
{
	static_assert(std::is_floating_point_v<T>, "FMath ill format, can only accept floating point types");

	T Result{};
	for (std::size_t i = 0; i < VectorA.size(); ++i)
	{
		Result += (VectorA[i] * VectorB[i]);
	}

	return Result;
}

template <typename T, std::size_t N>
std::array<T, N> FMath::Normalize(std::array<T, N> const& Vector)
{
	static_assert(std::is_floating_point_v<T>, "FMath ill format, can only accept floating point types");

	std::array<T, N> Result{};
	T const Magnitude = (1.f / FMath::Magnitude<T, N>(Vector));

	for (std::size_t i = 0; i < Vector.size(); ++i)
	{
		Result[i] = (Vector[i] * Magnitude);
	}

	return Result;
}

template <typename T, std::size_t N>
T FMath::SquaredMagnitude(std::array<T, N> const& Vector)
{
	static_assert(std::is_floating_point_v<T>, "FMath ill format, can only accept floating point types");

	T Result{};
	for (std::size_t i = 0; i < Vector.size(); ++i)
	{
		Result += (Vector[i] * Vector[i]);
	}

	return Result;
}

template <typename T, std::size_t N>
T FMath::Magnitude(std::array<T, N> const& Vector)
{
	static_assert(std::is_floating_point_v<T>, "FMath ill format, can only accept floating point types");

	return FMath::Sqrt<T>(FMath::SquaredMagnitude<T, N>(Vector));
}

template <typename T>
T FMath::Sqrt(T In)
{
	static_assert(std::is_floating_point_v<T>, "FMath ill format, can only accept floating point types");

	return sqrt(In);
}
