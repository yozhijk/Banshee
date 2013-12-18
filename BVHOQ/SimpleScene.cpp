//
//  SimpleScene.cpp
//  BVHOQ
//
//  Created by dmitryk on 08.10.13.
//  Copyright (c) 2013 Dmitry Kozlov. All rights reserved.
//

#include "SimpleScene.h"

std::shared_ptr<SimpleScene> SimpleScene::CreateFromObj(std::string const& fileName)
{
	auto Mesh = Mesh::CreateFromFile(fileName);
	
	return std::make_shared<SimpleScene>(Mesh);
}

std::vector<Mesh::Vertex> const& SimpleScene::GetVertices() const
{
	return vertices_;
}

std::vector<unsigned int> const& SimpleScene::GetIndices() const
{
	return indices_;
}

SimpleScene::~SimpleScene()
{
}

SimpleScene::SimpleScene(std::shared_ptr<Mesh> mesh_ptr)
{
	Mesh::Vertex const* vertexData = mesh_ptr->GetVertexArrayPtr();
	unsigned const* indexData = mesh_ptr->GetIndexArrayPtr();
	vertices_.resize(mesh_ptr->GetVertexCount());
	indices_.resize(mesh_ptr->GetIndexCount());
	
	BBox b = BBox(vertexData[0].position);
	for (int i = 0; i < mesh_ptr->GetVertexCount(); ++i)
	{
		vertices_[i] = vertexData[i];
		b = BBoxUnion(b, vertexData[i].position);
	}
	std::copy(indexData, indexData + mesh_ptr->GetIndexCount(), indices_.begin());

	MeshDesc md = {b, Sphere(), 0, indices_.size()};
	meshes_.push_back(md);
}

std::vector<SimpleScene::MeshDesc> const& SimpleScene::GetMeshes() const
{
	return meshes_;
}
