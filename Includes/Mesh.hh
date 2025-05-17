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

#include <cstdint>
#include <cstddef>
#include <vector>

#include "glad/glad.h"

#include "Utilities/Vector.hh"

// POD Class. Represent a single Vertex object.
struct FVertex
{
	FVector3d Position = FVector3d::Zero;
};

// POD Class. Represent a single Mesh object.
struct FMesh
{
	// buffer ids
	GLuint VAO = UINT64_MAX;
	GLuint VBO = UINT64_MAX;
	GLuint EBO = UINT64_MAX;

	// how triangles are built based on indexed drawing
	std::vector<unsigned int> Indices;

	// object space data (or local space)
	std::vector<FVertex> Vertices;
};