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

#include "Mesh.hh"

void FAssimpUtils::GetMeshes(aiScene const* Scene,
	aiNode const* Node,
	std::vector<aiMesh const*>& Out)
{
	if (Scene == nullptr || Node == nullptr)
	{
		return;
	}

	for (std::size_t i = 0; i < Node->mNumMeshes; ++i)
	{
		int const MeshIndex = *(Node->mMeshes + i);
		Out.push_back(*(Scene->mMeshes + MeshIndex));
	}

	for (std::size_t j = 0; j < Node->mNumChildren; ++j)
	{
		GetMeshes(Scene, *(Node->mChildren + j), Out);
	}
}

void FAssimpUtils::ConvertMeshes(std::vector<aiMesh const*> const& Meshes,
	void*& MemoryBlock)
{
	auto const PushVertex = [](aiMesh const* Target,
		std::size_t i)
		{
			auto aiVertex = *(Target->mVertices + i);
			auto aiNormal = *(Target->mNormals + i);
			auto aiTangeant = *(Target->mTangents + i);

			FVertex Vertex;
			Vertex.Positions.push_back(FVector3d(aiVertex.x, aiVertex.y, aiVertex.z));
			Vertex.Normals.push_back(FVector3d(aiNormal.x, aiNormal.y, aiNormal.z));
			Vertex.Tangeant.push_back(FVector3d(aiTangeant.x, aiTangeant.y, aiTangeant.z));
			return Vertex;
		};

	auto const PushFace = [](aiMesh const* Target,
		std::size_t i)
		{
			return *(Target->mFaces + i)->mIndices;
		};

	for (std::size_t i = 0; i < Meshes.size(); ++i)
	{
		aiMesh const* Target = Meshes[i];
		FMesh Mesh;

		for (std::size_t i = 0; i < Target->mNumVertices; ++i)
		{
			Mesh.Vertices.push_back(PushVertex(Target, i));
		};

		for (std::size_t i = 0; i < Target->mNumFaces; ++i)
		{
			Mesh.Indices.push_back(PushFace(Target, i));
		};
	}

	// TODO allocate memory on a pool allocator
	MemoryBlock = nullptr;
}
