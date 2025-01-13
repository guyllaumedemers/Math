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

#include "glad/glad.h"

struct FOpenGlUtils
{
	static void SetupVertexArrayObject(GLuint* BufferId);

	static void SetupVertexAttributePointer(GLuint LocationId,
		GLint Count,
		GLsizei Stride,
		void const* Offset);

	static void SetupVertexShader(GLuint* BufferId,
		GLuint* ShaderId,
		char const* ShaderSrc,
		void const* data,
		GLsizeiptr Size,
		GLenum usage);

	static void SetupFragmentShader(GLuint* ShaderId,
		char const* ShaderSrc);

	static void SetupShaderProgram(GLuint* ShaderProgramId,
		GLuint VertextShaderId,
		GLuint FragmentShaderId);

	static void RunVAO(GLuint ShaderProgramId,
		GLuint VAO);

	static void Cleanup(GLuint* ShaderProgramId,
		GLuint* VBOs,
		GLuint* VAOs);

	static bool LoadVertices(char const* File,
		void*& Dest,
		GLsizeiptr& Size);
};
