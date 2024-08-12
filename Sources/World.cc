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

#include "World.hh"

FWorld::~FWorld()
{
}

void FWorld::Draw() const
{
	// need to draw a grid in a specific vector space (i.e 2d or 3d)

	// where we can visualize objects moving into that space

	// example : showing how a point lerp from A to B

	// displaying this information through opengl rendering pipeline

	// imply : A - converting any object from Model-Space (Object local coordinate), View-space (Camera view coordinate/frustrum),

	// Clip-space (NDC - normalize device coordinate), Screen-space (Monitor 2d space - orthographic projection or perspective projection)

	// maybe what I want is :

	// draw the basis in world space (x, y, z)

	// define a camera with set position, rotation according to this point in space
}
