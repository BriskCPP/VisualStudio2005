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
				//p指的是pitch转换为空间中的长度，这里定义了视域在-20°到+20°
				float d = ((float)this->R)/4;//d指的是相邻两条平行线在空间中的距离

				//要求-R<p+id<R;;即要求(-R-p)/d<i<(R-p)/d
				INT8 iMin = p<-(float)this->R?(INT8)((-(float)this->R-p)/d)+1:(INT8)((-(float)this->R-p)/d);
				INT8 iMax = p>(float)this->R?(INT8)(((float)this->R-p)/d)-1:(INT8)(((float)this->R-p)/d);
				//i的上下限必须分类讨论，这就是边界问题

				for(INT8 i = iMin;i<=iMax;i++)
				{
					double h = abs(p+i*d);
					double halfL = sqrt((float)this->R*(float)this->R-h*h);


					if(i==0)
					{
						//这里未来要改成对i=0和i!=0单独处理
						if(0.2*(float)this->R<halfL&&halfL<0.8*(float)this->R)
						{
							//被圆形的Mask所遮挡
							//平行线的长度只能取为2*halfL
							struct VertexWithColor currentVertex;
							currentVertex.color = D3DCOLOR_XRGB(0,0xff,0);
							currentVertex.x = (float)((p+i*d)*sin(this->roll)-halfL*cos(this->roll));
							currentVertex.y = (float)((p+i*d)*cos(this->roll)+halfL*sin(this->roll));
							currentVertex.z = (float)this->Z;
							vertexList.push_back(currentVertex);
							//一边的外侧端点

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
							//另一边的外侧端点
						}
						if(halfL>=0.8*(float)this->R)
						{
							//没有被圆形的Mask所遮挡
							//可以取为0.4
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
						//这里未来要改成对i=0和i!=0单独处理
						if(0.2*(float)this->R<halfL&&halfL<0.5*(float)this->R)
						{
							//被圆形的Mask所遮挡
							//平行线的长度只能取为2*halfL
							struct VertexWithColor currentVertex;
							currentVertex.color = D3DCOLOR_XRGB(0,0xff,0);
							currentVertex.x = (float)((p+i*d)*sin(this->roll)-halfL*cos(this->roll));
							currentVertex.y = (float)((p+i*d)*cos(this->roll)+halfL*sin(this->roll));
							currentVertex.z = (float)this->Z;
							vertexList.push_back(currentVertex);
							//一边的外侧端点

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
							//另一边的外侧端点
						}
						if(halfL>=0.5*(float)this->R)
						{
							//没有被圆形的Mask所遮挡
							//可以取为0.4
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