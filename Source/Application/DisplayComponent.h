#pragma once

#include "../Direct3D9/Direct3D9.h"
#include <list>

using Direct3D::v9::resource::primitive::vertex::VertexWithColor;

namespace Application
{
	class DisplayComponent
	{
	public:
		DisplayComponent();
		DisplayComponent(std::list<struct VertexWithColor> vertexList);

		//获得平移/旋转等变换之后的VertexList
		//这是个虚函数，为继承的各个显示组件提供默认实现
		virtual std::list<struct VertexWithColor> getOutputVertexList();

		//设置平移
		virtual void setTranslation(D3DXVECTOR3 translation);
		//设置旋转
		virtual void setRotation(D3DXVECTOR3 rotation);
		//设置缩放
		virtual void setScale(D3DXVECTOR3 scale);
	private:
		D3DXVECTOR3 center; 

		D3DXVECTOR3 translation;//平移
		D3DXVECTOR3 rotation;//旋转
		D3DXVECTOR3 scale;//缩放

		std::list<struct VertexWithColor> vertexList;
	};
}