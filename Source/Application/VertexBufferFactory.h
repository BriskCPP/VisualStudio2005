#pragma once
#include "../Direct3D9/Direct3D9.h"
#include "./DisplayComponent.h"

#include <list>

namespace Application
{
	namespace vertexBufferFactory
	{
		Direct3D::v9::resource::primitive::buffer::VertexBuffer<struct Direct3D::v9::resource::primitive::vertex::VertexWithColor> getVertexBuffer(
			IDirect3DDevice9 *device,
			std::list<Application::DisplayComponent *> componentList
			);
	}
}