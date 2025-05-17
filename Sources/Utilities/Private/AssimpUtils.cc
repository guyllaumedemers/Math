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

#include "AssimpUtils.hh"

#include <cassert>

#include "assimp/cimport.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"

#include "Memory.hh"

void FAssimpUtils::GetMeshes(aiScene const* Scene, aiNode const* Node, std::vector<aiMesh const*>& Out)
{
	if (Scene == nullptr || Node == nullptr)
	{
		return;
	}

	for (std::size_t i = 0; i < Node->mNumMeshes; ++i)
	{
		unsigned int const MeshIndex = *(Node->mMeshes + i);
		Out.push_back(*(Scene->mMeshes + MeshIndex));
	}

	for (std::size_t j = 0; j < Node->mNumChildren; ++j)
	{
		GetMeshes(Scene, *(Node->mChildren + j), Out);
	}
}

std::vector<FMesh> FAssimpUtils::ConvertMeshes(std::vector<aiMesh const*> const& Meshes)
{
	std::vector<FMesh> OutResult;
	for (std::size_t i = 0; i < Meshes.size(); ++i)
	{
		aiMesh const* Target = Meshes[i];
		FMesh Mesh;

		for (std::size_t j = 0; j < Target->mNumVertices; ++j)
		{
			aiVector3D const& aiVertex = *(Target->mVertices + j);
			FVertex const MeshVertex = { FVector3d{ aiVertex.x, aiVertex.y, aiVertex.z } };
			Mesh.Vertices.push_back(MeshVertex);
		};

		for (std::size_t k = 0; k < Target->mNumFaces; ++k)
		{
			aiFace const* Face = (Target->mFaces + k);
			for (std::size_t m = 0; m < Face->mNumIndices; ++m)
			{
				unsigned int const FaceIndex = Face->mIndices[m];
				Mesh.Indices.push_back(FaceIndex);
			}
		};

		OutResult.push_back(Mesh);
	}

	return OutResult;
}
