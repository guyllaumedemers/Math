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

#include "imgui.h"
#include "glad/glad.h"
#include "SDL3/SDL.h"

#include "Object.hh"
#include "Utilities/OpenGlUtils.hh"

extern FStackAllocator gStackAllocator;

std::size_t UDemoExpression::Size() const
{
	return sizeof(UDemoExpression);
}

void UDemoExpression::ApplicationDraw()
{
	assert(DemoCube != nullptr);
	FOpenGlUtils::UseProgram(DemoCube->ShaderProgramID, DemoCube->VAO);
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
	// TODO do more here if required!
}

void UDemoExpression::Init()
{
	FObject Cube;
	FMemoryBlock const Payload = FMemory::Malloc({ &gStackAllocator, sizeof(FObject) }, &Cube);
	DemoCube = reinterpret_cast<FObject*>(Payload.Payload);

	assert(DemoCube != nullptr);

	{
		FOpenGlUtils::SetupVertexArrayObject(&DemoCube->VAO);
	}

	{
		char const* File = "";
		assert(FOpenGlUtils::LoadVertices(File,
			DemoCube->Data,
			DemoCube->MemBlockSize) == true);

		FOpenGlUtils::SetupVertexBufferObject(&DemoCube->VBO,
			DemoCube->Data /*data*/,
			DemoCube->MemBlockSize /*size*/,
			GL_STATIC_DRAW);
	}

	{
		// Note to self : VertexAttributePointer are configured based on the currently bound VBO (which is attached to the active VAO context bound)
		// failing to configure VertexAttributePointer AFTER VBO binding will result in glDrawArrays throwing!
		FOpenGlUtils::SetupVertexAttributePointer(0, 3 /*count*/, 3 * sizeof(float) /*stride*/, NULL/*offset*/);
	}

	{
		char const* VertexShader = "#version 460 core\nlayout (location = 0) in vec3 aPos;\nvoid main()\n{\n\tgl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n}";
		FOpenGlUtils::SetupShader(&DemoCube->VertexProgramID,
			VertexShader,
			GL_VERTEX_SHADER);
	}

	{
		char const* FragmentShader = "#version 460 core\nout vec4 fragColor;\nvoid main()\n{\n\tfragColor = vec4(1.0, 0.0, 0.0, 1.0);\n}";
		FOpenGlUtils::SetupShader(&DemoCube->FragmentProgramID,
			FragmentShader,
			GL_FRAGMENT_SHADER);
	}

	{
		FOpenGlUtils::SetupShaderProgram(&DemoCube->ShaderProgramID,
			DemoCube->VertexProgramID,
			DemoCube->FragmentProgramID);
	}
}

void UDemoExpression::Cleanup()
{
	assert(DemoCube != nullptr);

	{
		FOpenGlUtils::Cleanup(&DemoCube->ShaderProgramID,
			&DemoCube->VertexProgramID,
			&DemoCube->FragmentProgramID,
			&DemoCube->VBO,
			&DemoCube->VAO);
	}

	FMemory::Free(&gStackAllocator, FMemoryBlock{ DemoCube->MemBlockSize, DemoCube->Data });
	FMemory::Free(&gStackAllocator, FMemoryBlock{ sizeof(FObject), DemoCube });
}
