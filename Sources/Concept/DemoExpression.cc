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

#include "Concept/DemoExpression.hh"

#include <sstream>

#include "imgui.h"
#include "SDL3/SDL.h"

#include "Mesh.hh"
#include "Object.hh"
#include "../Utilities/Private/OpenGlUtils.hh"

extern FArenaAllocator gArenaAllocator;
extern FStackAllocator gStackAllocator;
extern FPoolAllocator gPoolAllocator;

std::size_t UDemoExpression::Size() const
{
	return sizeof(UDemoExpression);
}

void UDemoExpression::ApplicationDraw()
{
	assert(DemoCube != nullptr);

	FOpenGlUtils::UseProgram(DemoCube->ShaderProgramID);
	for (std::size_t i = 0; i < DemoCube->NumMeshes; ++i)
	{
		FMesh& Mesh = DemoCube->Meshes[i];

		// @gdemers doing it on the cpu side to showcase math implemntation defined for this project.
		// since this project is suppose to prove the comprehension of math principles usefull to game programming,
		// all calculation are done with our custom api and defined on the cpu side instead of gpu.
		// IMPORTANT : This is obviously WAYY less efficient.
		auto const NumVertices = Mesh.Vertices.size();
		for (std::size_t j = 0; j < NumVertices; ++j)
		{
			FVector4d PointInWorld = (DemoCube->Transform.ModelMatrix() * FVector4d { Mesh.Vertices[j].Position });
		}

		FOpenGlUtils::DrawObject(Mesh.VAO, Mesh.Indices.size());
	}
}

void UDemoExpression::ImGuiDraw()
{
	ImGui::Begin("Demo");
	ImGui::Checkbox("Toggle Orthographic View", &bIsOrthographic);
	ImGui::Checkbox("Toggle 3d", &bIs3d);
	ImGui::Checkbox("Toggle Grid", &bDisplayGrid);
	ImGui::End();
}

void UDemoExpression::Tick()
{
	assert(DemoCube != nullptr);

	for (std::size_t i = 0; i < DemoCube->NumMeshes; ++i)
	{
		FMesh& Mesh = DemoCube->Meshes[i];
	}
}

void UDemoExpression::Init()
{
	FMemoryBlock const Payload = FMemory::Malloc(&gStackAllocator, sizeof(FObject));
	DemoCube = reinterpret_cast<FObject*>(Payload.Payload);

	std::stringstream ss;
	ss << SDL_GetCurrentDirectory() << "\\..\\..\\" << "Res/Cube2.gltf";
	FOpenGlUtils::ImportMesh(ss.str().c_str(), DemoCube, &gPoolAllocator);
	ss.clear();

	assert(DemoCube != nullptr && DemoCube->Meshes != nullptr && DemoCube->NumMeshes > 0);

	for (std::size_t i = 0; i < DemoCube->NumMeshes; ++i)
	{
		FMesh& Mesh = DemoCube->Meshes[i];
		FOpenGlUtils::SetupVertexArrayObject(&Mesh.VAO);

		FOpenGlUtils::SetupBufferObject(&Mesh.VBO,
			&Mesh.Vertices[0] /*data*/,
			Mesh.Vertices.size() * sizeof(FVertex) /*size*/,
			GL_ARRAY_BUFFER,
			GL_STATIC_DRAW);

		FOpenGlUtils::SetupBufferObject(&Mesh.EBO,
			&Mesh.Indices[0] /*data*/,
			Mesh.Indices.size() * sizeof(std::size_t) /*size*/,
			GL_ELEMENT_ARRAY_BUFFER,
			GL_STATIC_DRAW);

		// Note to self : VertexAttributePointer are configured based on the currently bound VBO (which is attached to the active VAO context)
		// failing to configure VertexAttributePointer AFTER VBO binding will result in glDrawArrays throwing!
		FOpenGlUtils::SetupVertexAttributePointer(0, 3 /*count*/, 6 * sizeof(float) /*stride*/, NULL/*offset*/);
	}

	{
		char const* VertexShader = "#version 460 core\nlayout (location = 0) in vec3 aPos;\nvoid main()\n{\n\tgl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n}";
		FOpenGlUtils::SetupShader(&DemoCube->VertexProgramID,
			VertexShader,
			GL_VERTEX_SHADER);

		char const* FragmentShader = "#version 460 core\nout vec4 fragColor;\nvoid main()\n{\n\tfragColor = vec4(1.0, 0.0, 0.0, 1.0);\n}";
		FOpenGlUtils::SetupShader(&DemoCube->FragmentProgramID,
			FragmentShader,
			GL_FRAGMENT_SHADER);

		FOpenGlUtils::SetupShaderProgram(&DemoCube->ShaderProgramID,
			DemoCube->VertexProgramID,
			DemoCube->FragmentProgramID);
	}
}

void UDemoExpression::Cleanup()
{
	assert(DemoCube != nullptr && DemoCube->Meshes != nullptr && DemoCube->NumMeshes > 0);

	FOpenGlUtils::CleanupProgram(&DemoCube->ShaderProgramID,
		&DemoCube->VertexProgramID,
		&DemoCube->FragmentProgramID);

	for (std::size_t i = 0; i < DemoCube->NumMeshes; ++i)
	{
		FMesh& Mesh = DemoCube->Meshes[i];

		FOpenGlUtils::CleanupMesh(&Mesh.VAO,
			&Mesh.VBO,
			&Mesh.EBO);

		FMemory::Free(&gPoolAllocator,
			FMemoryBlock{ &Mesh, sizeof(FMesh) });
	}

	FMemory::Free(&gStackAllocator,
		FMemoryBlock{ DemoCube, sizeof(FObject) });
}
