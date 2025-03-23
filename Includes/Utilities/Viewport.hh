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

#include "Math.hh"
#include "Vector.hh"

// define a set of properties specific to a space on screen. each player can own their viewport
struct FViewport
{
	FViewport() = default;

	explicit FViewport(float const Width, float const Height)
	{
		this->Width = Width;
		this->Height = Height;
		this->ResolutionGateRatio = (Width / Height);
	};

	inline FVector2d ViewportTransform(FVector2d const& Ps /*Point in screen space*/) const
	{
		// @gdemers convert point defined in canonical view [-1,1] to NDC [0,1].
		float const X = (1 + Ps.Vector[0]) * 0.5f; // x
		float const Y = (1 - Ps.Vector[1]) * 0.5f; // y (0 start at top-left - like CRT tv emitting photon during raster scanning)
		// @gdemers convert NDC point to raster space
		return FVector2d{ FMath::Floor(X * this->Width), FMath::Floor(Y * this->Height) };
	}

	// @gdemers data specific to the raster target (display)
	float Width, Height, ResolutionGateRatio = 0.f;

	// @gdemers handle aspect ratio mismatch between image plane and raster target (display)
	enum class EFitResolutionGate { Fill, Overscan } FitResolutionGate = EFitResolutionGate::Overscan;

	FViewport static Default;
};