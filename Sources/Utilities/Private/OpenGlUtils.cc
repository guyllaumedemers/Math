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

#include "OpenGlUtils.hh"

#include <cassert>
#include <functional>
#include <vector>

#include "assimp/cimport.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "SDL3/SDL.h"

#include "AssimpUtils.hh"
#include "Memory.hh"
#include "Object.hh"
#include "Utilities/Matrix.hh"

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

void FOpenGlUtils::UseProgram(GLuint ShaderProgramId)
{
	glUseProgram(ShaderProgramId);
}

void FOpenGlUtils::SetProjectionMatrix(GLuint ShaderProgramId,
	FMatrix4x4 const& ProjectionMatrix)
{
	GLint Location = glGetUniformLocation(ShaderProgramId, "projMat");
	glUniformMatrix4fv(Location, 1, GL_FALSE, &ProjectionMatrix.Matrix.RowsCols[0][0]);
}

void FOpenGlUtils::DrawObject(GLuint VAO,
	GLsizei Count)
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void FOpenGlUtils::CleanupProgram(GLuint* ShaderProgramId,
	GLuint* VertexShaderId,
	GLuint* FragmentShaderId)
{
	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDetachShader(*ShaderProgramId, *VertexShaderId);
	glDetachShader(*ShaderProgramId, *FragmentShaderId);
	glDeleteProgram(*ShaderProgramId);

	// clear cached variables
	*VertexShaderId = 0;
	*FragmentShaderId = 0;
	*ShaderProgramId = 0;
}

void FOpenGlUtils::CleanupMesh(GLuint* VAOs,
	GLuint* VBOs,
	GLuint* EBOs)
{
	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);
	glDeleteBuffers(1, EBOs);
	*VAOs = 0;
	*VBOs = 0;
	*EBOs = 0;
}

void FOpenGlUtils::ImportMesh(char const* File,
	FObject* Object,
	FAllocator* Alloc)
{
	aiScene const* Scene = aiImportFile(File, 0);
	if (Scene != nullptr && Scene->HasMeshes())
	{
		std::vector<aiMesh const*> OutaiMeshes;

		FAssimpUtils::GetMeshes(Scene, Scene->mRootNode, OutaiMeshes);
		std::vector<FMesh> OutMeshes = FAssimpUtils::ConvertMeshes(OutaiMeshes);

		FMemoryBlock MemBlock = FMemory::Malloc({ Alloc, sizeof(FMesh) * OutMeshes.size() }, &OutMeshes[0]);
		Object->Meshes = reinterpret_cast<FMesh*>(MemBlock.Payload);
		Object->NumMeshes = OutMeshes.size();
	}

	// use the cached importer pimp, on the scene, to clear resources
	aiReleaseImport(Scene);
}
