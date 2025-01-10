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

#include <stdio.h>

#include "imgui.h"
#include "glad/glad.h"
#include "SDL3/SDL.h"

#include "Object.hh"
#include "Utilities/OpenGlUtils.hh"

extern FStackAllocator gStackAllocator;

UDemoExpression::UDemoExpression()
{
	FObject Cube;
	FMemoryBlock const Payload = FMemory::Malloc({ &gStackAllocator, sizeof(FObject) }, &Cube);
	DemoCube = reinterpret_cast<FObject*>(Payload.Payload);

	GLuint VertextShaderID;
	GLuint FramentShaderID;

	{
		char const* File = "";
		char const* VertextShader = "#version 330 core\nlayout (location = 0) in vec3 aPos;\nvoid main()\n{\n\tgl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n}";
		FOpenGlUtils::SetupVertexShader(&DemoCube->VBO, &VertextShaderID, VertextShader, sizeof(DemoCube->Mesh), FOpenGlUtils::LoadVertices(File, DemoCube->Mesh), GL_STATIC_DRAW);
	}

	{
		char const* FragmentShader = "#version 330 core\nout vec4 fragColor;\nvoid main()\n{\n\tfragColor = vec4(0.5, 0.5, 0.5, 1.0);\n}";
		FOpenGlUtils::SetupFragmentShader(&FramentShaderID, FragmentShader);
	}

	{
		FOpenGlUtils::SetupShaderProgram(&DemoCube->ShaderProgramID, VertextShaderID, FramentShaderID);
	}
}

UDemoExpression::~UDemoExpression()
{
	// TODO Fix problem here. We deallocate in main when escaping the factory method.
	if (DemoCube == nullptr) { return; }
	FMemory::Free(&gStackAllocator, FMemoryBlock{ sizeof(FObject), DemoCube });
}

std::size_t UDemoExpression::Size() const
{
	return sizeof(UDemoExpression);
}

void UDemoExpression::Draw()
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
	glUseProgram(DemoCube->ShaderProgramID);
}
