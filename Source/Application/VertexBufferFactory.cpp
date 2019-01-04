#include "VertexBufferFactory.h"
#include "../Direct3D9/Direct3D9.h"

namespace Application
{
	namespace vertexBufferFactory
	{
		Direct3D::v9::primitive::buffer::VertexBuffer<struct Direct3D::v9::primitive::vertex::VertexWithColor>
			getVertexBuffer(
			IDirect3DDevice9 *device,
			std::list<Application::DisplayComponent *> componentList
			)
		{
			using Direct3D::v9::primitive::buffer::VertexBuffer;
			using Direct3D::v9::primitive::vertex::VertexWithColor;

			std::list<struct VertexWithColor> allVertexList;

			for(std::list<Application::DisplayComponent *>::iterator displayComponentListIterator = componentList.begin();
				displayComponentListIterator!=componentList.end();displayComponentListIterator++
				)
			{
				std::list<struct VertexWithColor> tempVertexList =
					(*displayComponentListIterator)->getOutputVertexList();
				for(std::list<struct VertexWithColor>::iterator tempVertexIterator = tempVertexList.begin();
					tempVertexIterator!=tempVertexList.end();tempVertexIterator++
					)
				{
					allVertexList.push_back(*tempVertexIterator);
				}
			}

			return VertexBuffer<struct VertexWithColor>(device,allVertexList);
		}
	}
}

