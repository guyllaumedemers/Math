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

#include "Utilities/OpenGlUtils.hh"

#include <cstddef>

#include "SDL3/SDL.h"

void FOpenGlUtils::SetupVertexShader(GLuint* BufferId,
	GLuint* ShaderId,
	char const* ShaderSrc,
	GLsizeiptr size,
	void const* data,
	GLenum usage)
{
	glGenBuffers(1, BufferId);
	glBindBuffer(GL_ARRAY_BUFFER, *BufferId);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);

	*ShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(*ShaderId, 1, &ShaderSrc, NULL);
	glCompileShader(*ShaderId);

	int CompilationResult = 0;
	glGetShaderiv(*ShaderId, GL_COMPILE_STATUS, &CompilationResult);

	if (!CompilationResult)
	{
		char Output[512];
		glGetShaderInfoLog(*ShaderId, 512, NULL, Output);
		SDL_Log("Shader Compilation Failed: %s", Output);
	}
}

void FOpenGlUtils::SetupFragmentShader(GLuint* ShaderId,
	char const* ShaderSrc)
{
	*ShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(*ShaderId, 1, &ShaderSrc, NULL);
	glCompileShader(*ShaderId);

	int CompilationResult = 0;
	glGetShaderiv(*ShaderId, GL_COMPILE_STATUS, &CompilationResult);

	if (!CompilationResult)
	{
		char Output[512];
		glGetShaderInfoLog(*ShaderId, 512, NULL, Output);
		SDL_Log("Shader Compilation Failed: %s", Output);
	}
}

void FOpenGlUtils::SetupShaderProgram(GLuint* ShaderProgramId,
	GLuint VertextShaderId,
	GLuint FragmentShaderId)
{
	*ShaderProgramId = glCreateProgram();
	glAttachShader(*ShaderProgramId, VertextShaderId);
	glAttachShader(*ShaderProgramId, FragmentShaderId);
	glLinkProgram(*ShaderProgramId);

	int CompilationResult = 0;
	glGetProgramiv(*ShaderProgramId, GL_LINK_STATUS, &CompilationResult);

	if (!CompilationResult)
	{
		char Output[512];
		glGetShaderInfoLog(*ShaderProgramId, 512, NULL, Output);
		SDL_Log("Shader Program Linkage Failed: %s", Output);
	}

	glDeleteShader(VertextShaderId);
	glDeleteShader(FragmentShaderId);
}

void* FOpenGlUtils::LoadVertices(char const* File, void* Dest)
{
	// TODO make real fopen function to read from file
	float static Temp[]
	{
		-0.5, -0.5, 1.0,
		 0.5, -0.5, 1.0,
		 0.0,  0.5, 1.0
	};

	return static_cast<void*>(Temp);
}
