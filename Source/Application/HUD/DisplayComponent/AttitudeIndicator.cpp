#include "AttitudeIndicator.h"

#include <cmath>

namespace Application
{
	namespace HUD
	{
		namespace DisplayComponent
		{
			AttitudeIndicator::AttitudeIndicator(double pitch,double roll,UINT R /* = 600 */,UINT Z /* = 1000 */)
			{
				this->pitch = pitch;
				this->roll = roll;
				this->R = R;
				this->Z = Z;
			}

			std::list<struct Direct3D::v9::resource::primitive::vertex::VertexWithColor> AttitudeIndicator::getOutputVertexList()
			{
				std::list<struct Direct3D::v9::resource::primitive::vertex::VertexWithColor> vertexList;
				double p = this->pitch*((float)this->R/(D3DX_PI/9));
				//pָ����pitchת��Ϊ�ռ��еĳ��ȣ����ﶨ����������-20�㵽+20��
				float d = ((float)this->R)/4;//dָ������������ƽ�����ڿռ��еľ���

				//Ҫ��-R<p+id<R;;��Ҫ��(-R-p)/d<i<(R-p)/d
				INT8 iMin = p<-(float)this->R?(INT8)((-(float)this->R-p)/d)+1:(INT8)((-(float)this->R-p)/d);
				INT8 iMax = p>(float)this->R?(INT8)(((float)this->R-p)/d)-1:(INT8)(((float)this->R-p)/d);
				//i�������ޱ���������ۣ�����Ǳ߽�����

				for(INT8 i = iMin;i<=iMax;i++)
				{
					double h = abs(p+i*d);
					double halfL = sqrt((float)this->R*(float)this->R-h*h);


					if(i==0)
					{
						//����δ��Ҫ�ĳɶ�i=0��i!=0��������
						if(0.2*(float)this->R<halfL&&halfL<0.8*(float)this->R)
						{
							//��Բ�ε�Mask���ڵ�
							//ƽ���ߵĳ���ֻ��ȡΪ2*halfL
							struct VertexWithColor currentVertex;
							currentVertex.color = D3DCOLOR_XRGB(0,0xff,0);
							currentVertex.x = (float)((p+i*d)*sin(this->roll)-halfL*cos(this->roll));
							currentVertex.y = (float)((p+i*d)*cos(this->roll)+halfL*sin(this->roll));
							currentVertex.z = (float)this->Z;
							vertexList.push_back(currentVertex);
							//һ�ߵ����˵�

							currentVertex.x = (float)((p+i*d)*sin(this->roll)-0.2*(float)this->R*cos(this->roll));
							currentVertex.y = (float)((p+i*d)*cos(this->roll)+0.2*(float)this->R*sin(this->roll));
							currentVertex.z = (float)this->Z;
							vertexList.push_back(currentVertex);

							currentVertex.x = (float)((p+i*d)*sin(this->roll)+0.2*(float)this->R*cos(this->roll));
							currentVertex.y = (float)((p+i*d)*cos(this->roll)-0.2*(float)this->R*sin(this->roll));
							currentVertex.z = (float)this->Z;
							vertexList.push_back(currentVertex);


							currentVertex.x = (float)((p+i*d)*sin(this->roll)+halfL*cos(this->roll));
							currentVertex.y = (float)((p+i*d)*cos(this->roll)-halfL*sin(this->roll));
							currentVertex.z = (float)this->Z;
							vertexList.push_back(currentVertex);
							//��һ�ߵ����˵�
						}
						if(halfL>=0.8*(float)this->R)
						{
							//û�б�Բ�ε�Mask���ڵ�
							//����ȡΪ0.4
							struct VertexWithColor currentVertex;
							currentVertex.color = D3DCOLOR_XRGB(0,0xff,0);
							currentVertex.x = (float)((p+i*d)*sin(this->roll)-0.8*(float)this->R*cos(this->roll));
							currentVertex.y = (float)((p+i*d)*cos(this->roll)+0.8*(float)this->R*sin(this->roll));
							currentVertex.z = (float)this->Z;
							vertexList.push_back(currentVertex);

							currentVertex.x = (float)((p+i*d)*sin(this->roll)-0.2*(float)this->R*cos(this->roll));
							currentVertex.y = (float)((p+i*d)*cos(this->roll)+0.2*(float)this->R*sin(this->roll));
							currentVertex.z = (float)this->Z;
							vertexList.push_back(currentVertex);

							currentVertex.x = (float)((p+i*d)*sin(this->roll)+0.2*(float)this->R*cos(this->roll));
							currentVertex.y = (float)((p+i*d)*cos(this->roll)-0.2*(float)this->R*sin(this->roll));
							currentVertex.z = (float)this->Z;
							vertexList.push_back(currentVertex);

							currentVertex.x = (float)((p+i*d)*sin(this->roll)+0.8*(float)this->R*cos(this->roll));
							currentVertex.y = (float)((p+i*d)*cos(this->roll)-0.8*(float)this->R*sin(this->roll));
							currentVertex.z = (float)this->Z;
							vertexList.push_back(currentVertex);
						}
					}


					if(i!=0)
					{
						//����δ��Ҫ�ĳɶ�i=0��i!=0��������
						if(0.2*(float)this->R<halfL&&halfL<0.5*(float)this->R)
						{
							//��Բ�ε�Mask���ڵ�
							//ƽ���ߵĳ���ֻ��ȡΪ2*halfL
							struct VertexWithColor currentVertex;
							currentVertex.color = D3DCOLOR_XRGB(0,0xff,0);
							currentVertex.x = (float)((p+i*d)*sin(this->roll)-halfL*cos(this->roll));
							currentVertex.y = (float)((p+i*d)*cos(this->roll)+halfL*sin(this->roll));
							currentVertex.z = (float)this->Z;
							vertexList.push_back(currentVertex);
							//һ�ߵ����˵�

							currentVertex.x = (float)((p+i*d)*sin(this->roll)-0.2*(float)this->R*cos(this->roll));
							currentVertex.y = (float)((p+i*d)*cos(this->roll)+0.2*(float)this->R*sin(this->roll));
							currentVertex.z = (float)this->Z;
							vertexList.push_back(currentVertex);

							currentVertex.x = (float)((p+i*d)*sin(this->roll)+0.2*(float)this->R*cos(this->roll));
							currentVertex.y = (float)((p+i*d)*cos(this->roll)-0.2*(float)this->R*sin(this->roll));
							currentVertex.z = (float)this->Z;
							vertexList.push_back(currentVertex);


							currentVertex.x = (float)((p+i*d)*sin(this->roll)+halfL*cos(this->roll));
							currentVertex.y = (float)((p+i*d)*cos(this->roll)-halfL*sin(this->roll));
							currentVertex.z = (float)this->Z;
							vertexList.push_back(currentVertex);
							//��һ�ߵ����˵�
						}
						if(halfL>=0.5*(float)this->R)
						{
							//û�б�Բ�ε�Mask���ڵ�
							//����ȡΪ0.4
							struct VertexWithColor currentVertex;
							currentVertex.color = D3DCOLOR_XRGB(0,0xff,0);
							currentVertex.x = (float)((p+i*d)*sin(this->roll)-0.5*(float)this->R*cos(this->roll));
							currentVertex.y = (float)((p+i*d)*cos(this->roll)+0.5*(float)this->R*sin(this->roll));
							currentVertex.z = (float)this->Z;
							vertexList.push_back(currentVertex);

							currentVertex.x = (float)((p+i*d)*sin(this->roll)-0.2*(float)this->R*cos(this->roll));
							currentVertex.y = (float)((p+i*d)*cos(this->roll)+0.2*(float)this->R*sin(this->roll));
							currentVertex.z = (float)this->Z;
							vertexList.push_back(currentVertex);

							currentVertex.x = (float)((p+i*d)*sin(this->roll)+0.2*(float)this->R*cos(this->roll));
							currentVertex.y = (float)((p+i*d)*cos(this->roll)-0.2*(float)this->R*sin(this->roll));
							currentVertex.z = (float)this->Z;
							vertexList.push_back(currentVertex);

							currentVertex.x = (float)((p+i*d)*sin(this->roll)+0.5*(float)this->R*cos(this->roll));
							currentVertex.y = (float)((p+i*d)*cos(this->roll)-0.5*(float)this->R*sin(this->roll));
							currentVertex.z = (float)this->Z;
							vertexList.push_back(currentVertex);
						}
					}
				}


				return vertexList;
			}
		}
	}
}