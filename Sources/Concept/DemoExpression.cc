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

#include <functional>
#include <fstream>
#include <sstream>

#include "imgui.h"
#include "SDL3/SDL.h"

#include "Camera.hh"
#include "Mesh.hh"
#include "Object.hh"
#include "Utilities/Matrix.hh"
#include "Utilities/Transform.hh"
#include "Utilities/Viewport.hh"
#include "Concept/ImGui/ImGuiBuilder.hh"
#include "../Utilities/Private/OpenGlUtils.hh"

extern FArenaAllocator gArenaAllocator;
extern FStackAllocator gStackAllocator;
extern FPoolAllocator gPoolAllocator;

std::size_t const UDemoExpression::Size() const
{
	return sizeof(UDemoExpression);
}

void UDemoExpression::ApplicationDraw(FViewport const& Viewport, FCamera const& Camera)
{
	assert(DemoCube != nullptr);

	// @gdemers update opengl state-machine with the program id we target.
	GLuint const ShaderProgramId = DemoCube->ShaderProgramID;
	FMatrix4x4 const ProjectionMatrix = Camera.PerspectiveProjection();/*project points in camera space and normalize the AABB (+Pw) for clipping*/
	FMatrix4x4 const ModelViewMatrix = Camera.ModelViewMatrix(DemoCube->Transform);

	static char const* const ProjMat = "projMat";
	static char const* const ModelViewMat = "modelviewMat";
	FOpenGlUtils::UseProgram(ShaderProgramId);
	FOpenGlUtils::SetUniformMat4(ShaderProgramId, ProjectionMatrix, ProjMat);
	FOpenGlUtils::SetUniformMat4(ShaderProgramId, ModelViewMatrix, ModelViewMat);

	for (std::size_t i = 0; i < DemoCube->NumMeshes; ++i)
	{
		FMesh& Mesh = DemoCube->Meshes[i];

		// @gdemers draw object vertices by sending each position to the vertex shader (programable pipeline)
		// note : the VAO has the object space (local space) positions cached in the VBO buffer thats bound. using these local space positions, and the above
		// model-view matrix, we can calculate vertices in regard to the camera coordinate system (view space).
		// using the projection matrix, we can project points, creating perspective if using perspective divide, onto the image plane which goes from [-inf,inf] and normalize our points using the AABB bounds.
		// last, we can discard points, during the clipping stage, that are outside the bounds defined by the AABB. (being normalize prior to executing the clipping stage makes things easier for the pipeline)
		FOpenGlUtils::DrawObject(Mesh.VAO, Mesh.Indices.size());
	}
}

void UDemoExpression::ImGuiDraw(FCamera* const Camera)
{
	assert(DemoCube != nullptr);

	ImGui::Begin("Demo");
	ImGui::BeginTabBar("Tab");

	if (ImGui::BeginTabItem("World"))
	{
		auto const static AABBProperties = FImGuiProperties("Axis-Aligned Bounding Box", 0.f, 1920.f);
		FImGuiBuilder::Builder.AxisAlignedBoundingBox(AABBProperties, Camera->ViewVolume);

		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Camera"))
	{
		auto const static TranslationProperties = FImGuiProperties("Translation", -10, 10);
		FImGuiBuilder::Builder.Translation(TranslationProperties, Camera->Transform);

		auto const static RotationProperties = FImGuiProperties("Rotation", -360.f, 360.f);
		FImGuiBuilder::Builder.Rotation(RotationProperties, Camera->Transform);

		ImGui::SliderFloat("Focal Length", &Camera->FocalLength, 1.f, 1000.f);

		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Cube"))
	{
		auto const static TranslationProperties = FImGuiProperties("Translation", -10.f, 10.f);
		FImGuiBuilder::Builder.Translation(TranslationProperties, DemoCube->Transform);

		auto const static RotationProperties = FImGuiProperties("Rotation", -360.f, 360.f);
		FImGuiBuilder::Builder.Rotation(RotationProperties, DemoCube->Transform);

		auto const static ScaleProperties = FImGuiProperties("Scale", -10.f, 10.f);
		FImGuiBuilder::Builder.Scale(ScaleProperties, DemoCube->Transform);

		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();
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
	// @gdemers adding missing explicit call to  default constructor of the newly allocated resource.
	new (DemoCube) FObject;

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
			Mesh.Indices.size() * sizeof(unsigned int) /*size*/,
			GL_ELEMENT_ARRAY_BUFFER,
			GL_STATIC_DRAW);

		// note : VertexAttributePointer are configured based on the currently bound VBO (which is attached to the active VAO context)
		// failing to configure VertexAttributePointer AFTER VBO binding will result in glDrawArrays throwing!
		FOpenGlUtils::SetupVertexAttributePointer(0,
			3 /*count*/,
			sizeof(FVertex) /*stride*/,
			NULL/*offset*/);
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
