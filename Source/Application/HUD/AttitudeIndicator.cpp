#include "./AttitudeIndicator.h"

#include <list>
#include <cmath>

namespace Application
{
	namespace HUD
	{
		void AttitudeIndicator::render(IDirect3DDevice9 *device)
		{
			//渲染
			//0. 生成水平线
			//每一度在空间中的长度
			double lengthPerDegree = (double)this->R/(double)this->fieldAngle;
			double pitch = this->pitch*lengthPerDegree;//获得俯仰角在空间中的长度

			struct {INT16 upper,lower;} divBound;

			divBound.lower = pitch-this->R<0
				?(INT16)((pitch-(double)this->R)/(lengthPerDegree*(double)this->degreePerDivision))
				:(INT16)((pitch-(double)this->R)/(lengthPerDegree*(double)this->degreePerDivision)+1);
			divBound.upper = pitch+this->R>0
				?(INT16)((pitch+(double)this->R)/(lengthPerDegree*(double)this->degreePerDivision))
				:(INT16)((pitch+(double)this->R)/(lengthPerDegree*(double)this->degreePerDivision)-1);

			using Direct3D::v9::resource::primitive::vertex::VertexWithColor;


			std::list<std::pair<struct VertexWithColor,struct VertexWithColor>> horizontalLineVertexList;
			for(INT16 divIndex = divBound.lower;divIndex <= divBound.upper;divIndex++)
			{
				//遍历生成各条水平线
				double halfLength = sqrt(
					(double)this->R*(double)this->R-
					pow((pitch - (double)divIndex*(double)(lengthPerDegree*this->degreePerDivision)),2)
					);

				//地平线要长一些,半长度占总半径的60%
				if(divIndex == 0) halfLength = halfLength < 0.6*this->R?halfLength:0.6*this->R;
				else halfLength = halfLength < 0.4*this->R?halfLength:0.4*this->R;

				/*
				struct VertexWithColor left,right;
				left.color = D3DCOLOR_XRGB(0,0xff,0);
				left.x = (float)(-halfLength);
				left.y = (float)(-pitch + divIndex*(double)(lengthPerDegree*this->degreePerDivision));
				left.z = (float)this->Z;

				right.color = D3DCOLOR_XRGB(0,0xff,0);
				right.x = (float)halfLength;
				right.y = left.y;
				right.z = (float)this->Z;
				*/
				struct VertexWithColor vertexArray[4];//这是在栈上开辟的，自动管理，所以不用释放

				vertexArray[0].x = (float)(-halfLength);
				vertexArray[1].x = (float)(-0.2)*(float)this->R;
				vertexArray[2].x = (float)0.2*(float)this->R;
				vertexArray[3].x = (float)(halfLength);

				//我只是真的不想命名了，仅此而已
				for(UINT8 aaaaaaaaa = 0;aaaaaaaaa <4;aaaaaaaaa++)
				{
					vertexArray[aaaaaaaaa].color = D3DCOLOR_XRGB(0,0xff,0);
					vertexArray[aaaaaaaaa].y = (float)(-pitch + divIndex*(double)(lengthPerDegree*this->degreePerDivision));
					vertexArray[aaaaaaaaa].z = (float)this->Z;
				}

				horizontalLineVertexList.push_back(std::pair<struct VertexWithColor,struct VertexWithColor>(vertexArray[0],vertexArray[1]));
				horizontalLineVertexList.push_back(std::pair<struct VertexWithColor,struct VertexWithColor>(vertexArray[2],vertexArray[3]));
			}


			Direct3D::v9::resource::primitive::LineList<struct VertexWithColor> horizontalLineList = 
				Direct3D::v9::resource::primitive::LineList<struct VertexWithColor>(device,horizontalLineVertexList);
			horizontalLineList.setRotation(D3DXVECTOR3(0,0,((float)this->roll/180)*D3DX_PI));
			horizontalLineList.render();

			//中间的+要单独生成并渲染，否则…………会跟随着滚转一起旋转
			struct VertexWithColor vertexArray[4];//这是在栈上开辟的，自动管理，所以不用释放
			vertexArray[0].color = D3DCOLOR_XRGB(0,0xff,0);
			vertexArray[1].color = D3DCOLOR_XRGB(0,0xff,0);
			vertexArray[2].color = D3DCOLOR_XRGB(0,0xff,0);
			vertexArray[3].color = D3DCOLOR_XRGB(0,0xff,0);

			vertexArray[0].x = (float)(-0.05*(float)this->R);
			vertexArray[1].x = (float)(0.05*(float)this->R);
			vertexArray[2].y = (float)(-0.05*(float)this->R);
			vertexArray[3].y = (float)(0.05*(float)this->R);

			vertexArray[0].y = 0;
			vertexArray[1].y = 0;
			vertexArray[2].x = 0;
			vertexArray[3].x = 0;

			//我只是真的不想命名了，仅此而已
			for(UINT8 aaaaaaaaa = 0;aaaaaaaaa <4;aaaaaaaaa++)
			{
				vertexArray[aaaaaaaaa].z = (float)this->Z;
				vertexArray[aaaaaaaaa].color = D3DCOLOR_XRGB(0,0xff,0);
			}

			std::list<std::pair<struct VertexWithColor,struct VertexWithColor>> aaaaaaaaaaaaaa;
			//没错，我又懒得命名了
			aaaaaaaaaaaaaa.clear();
			aaaaaaaaaaaaaa.push_back(std::pair<struct VertexWithColor,struct VertexWithColor>(vertexArray[0],vertexArray[1]));
			aaaaaaaaaaaaaa.push_back(std::pair<struct VertexWithColor,struct VertexWithColor>(vertexArray[2],vertexArray[3]));

			Direct3D::v9::resource::primitive::LineList<struct VertexWithColor>(device,aaaaaaaaaaaaaa).render();

		}
		//the following methods are setters
		AttitudeIndicator::AttitudeIndicator(UINT8 fieldAngle, UINT8 degreePerDivision, UINT R /* = 600 */,UINT Z /* = 1000 */)
		{
			this->fieldAngle = fieldAngle;
			this->degreePerDivision = degreePerDivision;
			this->R = R;
			this->Z = Z;

			this->pitch = 0;
			this->roll = 0;
		}
		void AttitudeIndicator::setPitch(double pitch)
		{
			this->pitch = pitch;
		}
		void AttitudeIndicator::setRoll(double roll)
		{
			this->roll = roll;
		}
	}
}