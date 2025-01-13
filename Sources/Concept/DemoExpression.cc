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
	FOpenGlUtils::RunVAO(DemoCube->ShaderProgramID, DemoCube->VAO);
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

	GLuint VertexShaderID;
	GLuint FragmentShaderID;

	{
		FOpenGlUtils::SetupVertexArrayObject(&DemoCube->VAO);
		FOpenGlUtils::SetupVertexAttributePointer(0, 3 /*count*/, 3 * sizeof(float) /*stride*/, NULL/*offset*/);
	}

	{
		char const* File = "";
		char const* VertexShader = "#version 330 core\nlayout (location = 0) in vec3 aPos;\nvoid main()\n{\n\tgl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n}";
		// ISA reordering require non-inline call to FOpenGlUtils::LoadVertices so DemoCube->Size has correct value
		assert(FOpenGlUtils::LoadVertices(File, DemoCube->Mesh, DemoCube->Size) == true);
		FOpenGlUtils::SetupVertexShader(&DemoCube->VBO, &VertexShaderID, VertexShader, DemoCube->Mesh, DemoCube->Size/*size*/, GL_STATIC_DRAW);
	}

	{
		char const* FragmentShader = "#version 330 core\nout vec4 fragColor;\nvoid main()\n{\n\tfragColor = vec4(0.5, 0.5, 0.5, 1.0);\n}";
		FOpenGlUtils::SetupFragmentShader(&FragmentShaderID, FragmentShader);
	}

	{
		FOpenGlUtils::SetupShaderProgram(&DemoCube->ShaderProgramID, VertexShaderID, FragmentShaderID);
	}
}

void UDemoExpression::Cleanup()
{
	assert(DemoCube != nullptr);

	{
		FOpenGlUtils::Cleanup(&DemoCube->ShaderProgramID, &DemoCube->VBO, &DemoCube->VAO);
	}

	FMemory::Free(&gStackAllocator, FMemoryBlock{ sizeof(FObject), DemoCube });
}
