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

#include "Concept/ImGui/ImGuiBuilder.hh"

#include "imgui.h"

#include "Camera.hh"
#include "Utilities/Transform.hh"

// static
FImGuiBuilder FImGuiBuilder::Builder;

FImGuiProperties::FImGuiProperties(char const* const PropertyTitle,
	float const Min,
	float const Max) :
	Title(PropertyTitle),
	MinValue(Min),
	MaxValue(Max)
{
}

void FImGuiBuilder::AxisAlignedBoundingBox(FImGuiProperties const& Properties,
	FAxisAlignBoundingBox& OutViewVolume)
{
	ImGui::Text(Properties.Title);
	ImGui::Separator();

	ImGui::BeginGroup();

	static char const* const lTitle = "left";
	static char const* const rTitle = "right";
	static char const* const bTitle = "bottom";
	static char const* const tTitle = "top";
	static char const* const nTitle = "near";
	static char const* const fTitle = "far";
	ImGui::InputFloat(lTitle, &OutViewVolume.Left);
	ImGui::InputFloat(rTitle, &OutViewVolume.Right);
	ImGui::InputFloat(bTitle, &OutViewVolume.Bottom);
	ImGui::InputFloat(tTitle, &OutViewVolume.Top);
	ImGui::InputFloat(nTitle, &OutViewVolume.Near);
	ImGui::InputFloat(fTitle, &OutViewVolume.Far);

	ImGui::EndGroup();

	static const char* const ResetTitle = "Reset";
	if (ImGui::Button(ResetTitle, { ImGui::GetContentRegionAvail().x , 0 }))
	{
		OutViewVolume = FAxisAlignBoundingBox(-10.f, 10.f, -10.f, 10.f, -10.f, 10.f);
	}
}

void FImGuiBuilder::Translation(FImGuiProperties const& Properties,
	FTransform& OutTransform)
{
	ImGui::Text(Properties.Title);
	ImGui::Separator();

	bool static bCanLinkScale = false;
	if (bCanLinkScale)
	{
		OutTransform.Position[2] = OutTransform.Position[1] = OutTransform.Position[0];
	}

	{
		ImGui::BeginGroup();

		static char const* const xTitle = "tX";
		ImGui::SliderFloat(xTitle, &OutTransform.Position[0], Properties.MinValue, Properties.MaxValue);

		ImGui::EndGroup();
	}

	{
		ImGui::BeginGroup();

		static char const* const yTitle = "tY";
		ImGui::SliderFloat(yTitle, &OutTransform.Position[1], Properties.MinValue, Properties.MaxValue);

		ImGui::EndGroup();
	}

	{
		ImGui::BeginGroup();

		static char const* const zTitle = "tZ";
		ImGui::SliderFloat(zTitle, &OutTransform.Position[2], Properties.MinValue, Properties.MaxValue);

		ImGui::EndGroup();
	}

	{
		ImGui::BeginGroup();

		static const char* const ResetTitle = "Reset Position";
		if (ImGui::Button(ResetTitle, ImVec2{ ImGui::GetContentRegionAvail().x , 0 }))
		{
			OutTransform.Position = FVector3d::Zero;
		}

		static const char* const LinkTitle = "Link Position";
		ImGui::Checkbox(LinkTitle, &bCanLinkScale);

		ImGui::EndGroup();
	}

	ImGui::NewLine();
}

void FImGuiBuilder::Rotation(FImGuiProperties const& Properties,
	FTransform& OutTransform)
{
	ImGui::Text(Properties.Title);
	ImGui::Separator();

	bool static bCanLinkScale = false;
	if (bCanLinkScale)
	{
		OutTransform.Rotation[2] = OutTransform.Rotation[1] = OutTransform.Rotation[0];
	}

	{
		ImGui::BeginGroup();

		static char const* const xTitle = "rX";
		ImGui::SliderFloat(xTitle, &OutTransform.Rotation[0], Properties.MinValue, Properties.MaxValue);

		ImGui::EndGroup();
	}

	{
		ImGui::BeginGroup();

		static char const* const yTitle = "rY";
		ImGui::SliderFloat(yTitle, &OutTransform.Rotation[1], Properties.MinValue, Properties.MaxValue);

		ImGui::EndGroup();
	}

	{
		ImGui::BeginGroup();

		static char const* const zTitle = "rZ";
		ImGui::SliderFloat(zTitle, &OutTransform.Rotation[2], Properties.MinValue, Properties.MaxValue);

		ImGui::EndGroup();
	}

	{
		ImGui::BeginGroup();

		static const char* const ResetTitle = "Reset Rotation";
		if (ImGui::Button(ResetTitle, ImVec2{ ImGui::GetContentRegionAvail().x , 0 }))
		{
			OutTransform.Rotation = FQuaternion::Zero;
		}

		static const char* const LinkTitle = "Link Rotation";
		ImGui::Checkbox(LinkTitle, &bCanLinkScale);

		ImGui::EndGroup();
	}

	ImGui::NewLine();
}

void FImGuiBuilder::Scale(FImGuiProperties const& Properties,
	FTransform& OutTransform)
{
	ImGui::Text(Properties.Title);
	ImGui::Separator();

	bool static bCanLinkScale = true;
	if (bCanLinkScale)
	{
		OutTransform.Scale[2] = OutTransform.Scale[1] = OutTransform.Scale[0];
	}

	{
		ImGui::BeginGroup();

		static char const* const xTitle = "sX";
		ImGui::SliderFloat(xTitle, &OutTransform.Scale[0], Properties.MinValue, Properties.MaxValue);

		ImGui::EndGroup();
	}

	{
		ImGui::BeginGroup();

		static char const* const yTitle = "sY";
		ImGui::SliderFloat(yTitle, &OutTransform.Scale[1], Properties.MinValue, Properties.MaxValue);

		ImGui::EndGroup();
	}

	{
		ImGui::BeginGroup();

		static char const* const zTitle = "sZ";
		ImGui::SliderFloat(zTitle, &OutTransform.Scale[2], Properties.MinValue, Properties.MaxValue);

		ImGui::EndGroup();
	}

	{
		ImGui::BeginGroup();

		static const char* const ResetTitle = "Reset Scale";
		if (ImGui::Button(ResetTitle, ImVec2{ ImGui::GetContentRegionAvail().x , 0 }))
		{
			OutTransform.Scale = FVector3d::One;
		}

		static const char* const LinkTitle = "Link Scale";
		ImGui::Checkbox(LinkTitle, &bCanLinkScale);

		ImGui::EndGroup();
	}

	ImGui::NewLine();
}
