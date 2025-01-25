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

#include "SDL3/SDL.h"

#include "Memory.hh"

extern FStackAllocator gStackAllocator;

void FOpenGlUtils::SetupVertexArrayObject(GLuint* BufferId)
{
	glGenVertexArrays(1, BufferId);
	glBindVertexArray(*BufferId);
}

void FOpenGlUtils::SetupVertexAttributePointer(GLuint VertexAttributeId,
	GLint Count,
	GLsizei Stride,
	void const* Offset)
{
	glVertexAttribPointer(VertexAttributeId, Count, GL_FLOAT, GL_FALSE, Stride, Offset);
	glEnableVertexAttribArray(VertexAttributeId);
}

void FOpenGlUtils::SetupBufferObject(GLuint* BufferId,
	void const* Data,
	GLsizeiptr Size,
	GLenum BufferType,
	GLenum Usage)
{
	glGenBuffers(1, BufferId);
	glBindBuffer(BufferType, *BufferId);
	glBufferData(BufferType, Size, Data, Usage);
}

void FOpenGlUtils::SetupShader(GLuint* ShaderId,
	char const* ShaderSrc,
	GLenum ShaderType)
{
	*ShaderId = glCreateShader(ShaderType);
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

void FOpenGlUtils::UseProgram(GLuint ShaderProgramId,
	GLuint VAO,
	GLsizei Count)
{
	glUseProgram(ShaderProgramId);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Count, GL_UNSIGNED_INT, 0);
}

void FOpenGlUtils::Cleanup(GLuint* ShaderProgramId,
	GLuint* VertexShaderId,
	GLuint* FragmentShaderId,
	GLuint* VBOs,
	GLuint* VAOs,
	GLuint* EBOs)
{
	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDetachShader(*ShaderProgramId, *VertexShaderId);
	glDetachShader(*ShaderProgramId, *FragmentShaderId);
	glDeleteProgram(*ShaderProgramId);
	glDeleteBuffers(1, VBOs);
	glDeleteVertexArrays(1, VAOs);

	// clear cached variables
	*VertexShaderId = 0;
	*FragmentShaderId = 0;
	*ShaderProgramId = 0;
	*VBOs = 0;
	*VAOs = 0;
	*EBOs = 0;
}

bool FOpenGlUtils::LoadFile(char const* File,
	void*& Dest,
	std::size_t& MemblockSize)
{
	return true;
}
