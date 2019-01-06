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

		//���ƽ��/��ת�ȱ任֮���VertexList
		//���Ǹ��麯����Ϊ�̳еĸ�����ʾ����ṩĬ��ʵ��
		virtual std::list<struct VertexWithColor> getOutputVertexList();

		//����ƽ��
		virtual void setTranslation(D3DXVECTOR3 translation);
		//������ת
		virtual void setRotation(D3DXVECTOR3 rotation);
		//��������
		virtual void setScale(D3DXVECTOR3 scale);
	private:
		D3DXVECTOR3 center; 

		D3DXVECTOR3 translation;//ƽ��
		D3DXVECTOR3 rotation;//��ת
		D3DXVECTOR3 scale;//����

		std::list<struct VertexWithColor> vertexList;
	};
}