#include "DisplayComponent.h"

#include <cmath>

namespace Application
{
	DisplayComponent::DisplayComponent()
	{
		//empty default constructor
	}
	DisplayComponent::DisplayComponent(std::list<struct VertexWithColor> vertexList)
	{
		this->vertexList = vertexList;

		this->center = D3DXVECTOR3(0,0,0);
		this->translation = D3DXVECTOR3(0,0,0);
		this->rotation = D3DXVECTOR3(0,0,0);
		this->scale = D3DXVECTOR3(0,0,0);
	}
	//设置平移
	void DisplayComponent::setTranslation(D3DXVECTOR3 translation)
	{
		this->translation = translation;
	}
	//设置旋转
	void DisplayComponent::setRotation(D3DXVECTOR3 rotation)
	{
		while(rotation.x>=2*D3DX_PI)  rotation.x-=2*D3DX_PI;
		while(rotation.x<=-2*D3DX_PI) rotation.x+=2*D3DX_PI;
		while(rotation.y>=2*D3DX_PI)  rotation.y-=2*D3DX_PI;
		while(rotation.y<=-2*D3DX_PI) rotation.y+=2*D3DX_PI;
		while(rotation.z>=2*D3DX_PI)  rotation.z-=2*D3DX_PI;
		while(rotation.z<=-2*D3DX_PI) rotation.z+=2*D3DX_PI;
		this->rotation = rotation;
	}
	//设置缩放
	void DisplayComponent::setScale(D3DXVECTOR3 scale)
	{
		this->scale = scale;
	}

	std::list<struct VertexWithColor> DisplayComponent::getOutputVertexList()
	{
		std::list<struct VertexWithColor> transformedVertexList = this->vertexList;
		//绕x轴旋转
		if(this->rotation.x!=0)
		{
			for(
				std::list<VertexWithColor>::iterator transformedVertexListIterator=transformedVertexList.begin();
				transformedVertexListIterator!=transformedVertexList.end();transformedVertexListIterator++
				)
			{
				// 相对于所定义的中心的相对y和z值
				// 在绕x轴的变换中,x不会变
				float relativeY = transformedVertexListIterator->y - this->center.y;
				float relativeZ = transformedVertexListIterator->z - this->center.z;

				transformedVertexListIterator->y = 
					this->center.y + relativeY*cos(this->rotation.x)-relativeZ*sin(this->rotation.x);
				transformedVertexListIterator->z = 
					this->center.z + relativeY*sin(this->rotation.x)+relativeZ*cos(this->rotation.x);
			}
		}
		//绕y轴旋转
		if(this->rotation.y!=0)
		{
			for(
				std::list<VertexWithColor>::iterator transformedVertexListIterator=transformedVertexList.begin();
				transformedVertexListIterator!=transformedVertexList.end();transformedVertexListIterator++
				)
			{
				//在绕y轴的旋转中,y不会发生改变
				float relativeX = transformedVertexListIterator->x - this->center.x;
				float relativeZ = transformedVertexListIterator->z - this->center.z;

				transformedVertexListIterator->x = 
					this->center.x + relativeX*cos(this->rotation.y)+relativeZ*sin(this->rotation.y);
				transformedVertexListIterator->z = 
					this->center.z - relativeX*sin(this->rotation.y)+relativeZ*cos(this->rotation.y);
			}
		}
		//绕z轴旋转
		if(this->rotation.z!=0)
		{
			for(
				std::list<VertexWithColor>::iterator transformedVertexListIterator=transformedVertexList.begin();
				transformedVertexListIterator!=transformedVertexList.end();transformedVertexListIterator++
				)
			{
				//在绕z轴的旋转中,z不会发生改变
				float relativeX = transformedVertexListIterator->x - this->center.x;
				float relativeY = transformedVertexListIterator->y - this->center.y;

				transformedVertexListIterator->x = 
					this->center.x + relativeX*cos(this->rotation.z)-relativeY*sin(this->rotation.z);
				transformedVertexListIterator->y = 
					this->center.y + relativeX*sin(this->rotation.z)+relativeY*cos(this->rotation.z);

			}
		}

		//平移  平移时记得保证center也要移动，才能保证旋转和平移的顺序不变
		for(
			std::list<VertexWithColor>::iterator transformedVertexListIterator=transformedVertexList.begin();
			transformedVertexListIterator!=transformedVertexList.end();transformedVertexListIterator++
			)
		{
			transformedVertexListIterator->x+=this->translation.x;
			transformedVertexListIterator->y+=this->translation.y;
			transformedVertexListIterator->z+=this->translation.z;
		}
		this->center.x += this->translation.x;
		this->center.y += this->translation.y;
		this->center.z += this->translation.z;

		//缩放
		//懒得做了，先就这样子吧
		return transformedVertexList;
	}
}
