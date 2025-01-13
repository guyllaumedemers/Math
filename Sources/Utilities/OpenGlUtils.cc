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

#include "Memory.hh"

void FOpenGlUtils::SetupVertexArrayObject(GLuint* BufferId)
{
	glGenVertexArrays(1, BufferId);
	glBindVertexArray(*BufferId);
}

void FOpenGlUtils::SetupVertexAttributePointer(GLuint AttributeId,
	GLuint Count,
	GLuint Stride,
	void const* Offset)
{
	glVertexAttribPointer(AttributeId, Count, GL_FLOAT, GL_FALSE, Stride, Offset);
	glEnableVertexAttribArray(AttributeId);
}

void FOpenGlUtils::SetupVertexShader(GLuint* BufferId,
	GLuint* ShaderId,
	char const* ShaderSrc,
	void const* Data,
	GLuint Size,
	GLenum Usage)
{
	glGenBuffers(1, BufferId);
	glBindBuffer(GL_ARRAY_BUFFER, *BufferId);
	glBufferData(GL_ARRAY_BUFFER, Size, Data, Usage);

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

void FOpenGlUtils::RunVAO(GLuint ShaderProgramId, GLuint VAO)
{
	glUseProgram(ShaderProgramId);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void FOpenGlUtils::Cleanup(GLuint* ShaderProgramId, GLuint* VBOs, GLuint* VAOs)
{
	glUseProgram(0);
	glDisableVertexAttribArray(0);
	glDeleteProgram(*ShaderProgramId);
	glDeleteBuffers(1, VBOs);
	glDeleteVertexArrays(1, VAOs);

	// clear cached variables
	*ShaderProgramId = 0;
	*VBOs = 0;
	*VAOs = 0;
}

void* FOpenGlUtils::LoadVertices(char const* File, void* Dest)
{
	// TODO make real fopen function to read from file
	float static Temp[]
	{
		-0.5f, -0.5f, 1.0f,
		 0.5f, -0.5f, 1.0f,
		 0.0f,  0.5f, 1.0f
	};

	Dest = Temp;
	return Dest;
}
