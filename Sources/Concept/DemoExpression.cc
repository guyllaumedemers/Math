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

#include <fstream>
#include <sstream>

#include "imgui.h"
#include "SDL3/SDL.h"

#include "Camera.hh"
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

void UDemoExpression::ApplicationDraw(FViewport const& Viewport, FCamera const& Pov /*User*/)
{
	assert(DemoCube != nullptr);

	FOpenGlUtils::UseProgram(DemoCube->ShaderProgramID);

	// get our parent object in View space (or Camera space)
	FMatrix4x4 const ModelViewMatrix = Pov.ModelViewMatrix(DemoCube->Transform);

	// get out parent object Projection matrix
	FMatrix4x4 const ProjectionMatrix = Pov.ProjectionMatrix(Viewport, DemoCube->Transform);

	for (std::size_t i = 0; i < DemoCube->NumMeshes; ++i)
	{
		FMesh& Mesh = DemoCube->Meshes[i];

		// @gdemers doing 3d point conversion to 2d here to showcase math implemntation defined for this project. However,
		// implementation cannot forward converted vertices to the gpu as the VBO would have to be written to with the updated data, imply targeting
		// GL_DYNAMIC_DRAW.
		// 
		// - This is purely for educational purpose! -
		// 
		//	auto const NumVertices = Mesh.Vertices.size();
		//	for (std::size_t j = 0; j < NumVertices; ++j)
		//	{
		//		// convert object space vertice (or local space) into Camera space
		//		FVector4d PointInCameraSpace = (ModelViewMatrix * FVector4d{ Mesh.Vertices[j].Position });

		//		// project point from Camera space onto the screen using frustum projection calculation
		//		FVector4d PointOnScreen = (ProjectionMatrix * PointInCameraSpace);
		// 
		//		// now depending on the projection type used, we can either do Orthographic or Perspective projection
		//		// note however that since we use opengl, perspective division is handled on the gpu so we dont have control
		//		// over it.
		//	}

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

	{
		std::stringstream ss;
		ss << SDL_GetCurrentDirectory() << "\\..\\..\\" << "Res/Cube2.gltf";
		FOpenGlUtils::ImportMesh(ss.str().c_str(), DemoCube, &gPoolAllocator);
	}

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
		std::stringstream filedir;
		filedir << SDL_GetCurrentDirectory() << "\\..\\..\\" << "Res/DemoExpression.vshader";

		std::ifstream fs;
		fs.open(filedir.str().c_str());

		std::stringstream vertshader;
		vertshader << fs.rdbuf();

		FOpenGlUtils::SetupShader(&DemoCube->VertexProgramID,
			vertshader.str().c_str(),
			GL_VERTEX_SHADER);

		fs.close();
	}

	{
		std::stringstream filedir;
		filedir << SDL_GetCurrentDirectory() << "\\..\\..\\" << "Res/DemoExpression.fshader";

		std::ifstream fs;
		fs.open(filedir.str().c_str());

		std::stringstream fragshader;
		fragshader << fs.rdbuf();

		FOpenGlUtils::SetupShader(&DemoCube->FragmentProgramID,
			fragshader.str().c_str(),
			GL_FRAGMENT_SHADER);

		fs.close();
	}

	FOpenGlUtils::SetupShaderProgram(&DemoCube->ShaderProgramID,
		DemoCube->VertexProgramID,
		DemoCube->FragmentProgramID);
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
