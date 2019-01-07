#include "./AttitudeIndicator.h"

#include <list>
#include <cstdlib>
#include <string>

#include <cmath>

namespace Application
{
	namespace HUD
	{
		void AttitudeIndicator::render(IDirect3DDevice9 *device)
		{
			//��Ⱦ
			//0. ����ˮƽ��
			//ÿһ���ڿռ��еĳ���
			double lengthPerDegree = (double)this->R/(double)this->fieldAngle;
			double pitch = this->pitch*lengthPerDegree;//��ø������ڿռ��еĳ���

			struct {INT16 upper,lower;} divBound;

			divBound.lower = pitch-this->R<0
				?(INT16)((pitch-(double)this->R)/(lengthPerDegree*(double)this->degreePerDivision))
				:(INT16)((pitch-(double)this->R)/(lengthPerDegree*(double)this->degreePerDivision)+1);
			divBound.upper = pitch+this->R>0
				?(INT16)((pitch+(double)this->R)/(lengthPerDegree*(double)this->degreePerDivision))
				:(INT16)((pitch+(double)this->R)/(lengthPerDegree*(double)this->degreePerDivision)-1);

			//����߽��������������ʱ�����������
			if(((pitch-(double)this->R)/(lengthPerDegree*(double)this->degreePerDivision)) == 
				(INT16)((pitch-(double)this->R)/(lengthPerDegree*(double)this->degreePerDivision))
				)
			{
				divBound.lower++;
			}
			if(((pitch+(double)this->R)/(lengthPerDegree*(double)this->degreePerDivision)) == 
				(INT16)((pitch+(double)this->R)/(lengthPerDegree*(double)this->degreePerDivision))
				)
			{
				divBound.upper--;
			}

			using Direct3D::v9::resource::primitive::vertex::VertexWithColor;


			std::list<std::pair<struct VertexWithColor,struct VertexWithColor>> horizontalLineVertexList;
			for(INT16 divIndex = divBound.lower;divIndex <= divBound.upper;divIndex++)
			{
				//�������ɸ���ˮƽ��
				double halfLength = sqrt(
					(double)this->R*(double)this->R-
					pow((pitch - (double)divIndex*(double)(lengthPerDegree*this->degreePerDivision)),2)
					);

				//��ƽ��Ҫ��һЩ,�볤��ռ�ܰ뾶��60%
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
				struct VertexWithColor vertexArray[4];//������ջ�Ͽ��ٵģ��Զ��������Բ����ͷ�

				vertexArray[0].x = (float)(-halfLength);
				vertexArray[1].x = (float)(-0.2)*(float)this->R;
				vertexArray[2].x = (float)0.2*(float)this->R;
				vertexArray[3].x = (float)(halfLength);

				//��ֻ����Ĳ��������ˣ����˶���
				for(UINT8 aaaaaaaaa = 0;aaaaaaaaa <4;aaaaaaaaa++)
				{
					vertexArray[aaaaaaaaa].color = D3DCOLOR_XRGB(0,0xff,0);
					vertexArray[aaaaaaaaa].y = (float)(-pitch + divIndex*(double)(lengthPerDegree*this->degreePerDivision));
					vertexArray[aaaaaaaaa].z = (float)this->Z;
				}

				horizontalLineVertexList.push_back(std::pair<struct VertexWithColor,struct VertexWithColor>(vertexArray[0],vertexArray[1]));
				horizontalLineVertexList.push_back(std::pair<struct VertexWithColor,struct VertexWithColor>(vertexArray[2],vertexArray[3]));

				///////////////////////////////////////////////
				//the following part is aimed at generating and rendering Text Mesh

				//������������;

				std::string currentDegreeText("");
				
				char charArray[64];
				sprintf_s(charArray,"%d",(int)(divIndex * this->degreePerDivision));
				currentDegreeText.append(charArray);



				Direct3D::v9::resource::mesh::text::TextMesh currentDegreeTextMesh = 
					Direct3D::v9::resource::mesh::MeshFactory::createTextMesh(device,currentDegreeText,std::string("Microsoft YaHei"));
				
				//��ΪTextMeshû�������̬��ʾ�ǵĵ����Ż������Ա��뵥���趨Translation   ��ʵҲ����
				D3DXVECTOR3 translationWithoutRotation;
				translationWithoutRotation.x = (float)(-0.1*(float)this->R);
				translationWithoutRotation.y = (float)(-pitch + divIndex*(float)(lengthPerDegree*this->degreePerDivision));
				translationWithoutRotation.z = (float)this->Z;

				currentDegreeTextMesh.setTranslation(Direct3D::v9::resource::vector::rotate(
					translationWithoutRotation, D3DXVECTOR3(0.0f,0.0f,((float)this->roll/180)*D3DX_PI)));
				currentDegreeTextMesh.setScale(D3DXVECTOR3(100,100,0.001f));
				currentDegreeTextMesh.setRotation(D3DXVECTOR3(0.0f,0.0f,((float)this->roll/180)*D3DX_PI));
				//��û����������
				currentDegreeTextMesh.render();
			}


			Direct3D::v9::resource::primitive::LineList<struct VertexWithColor> horizontalLineList = 
				Direct3D::v9::resource::primitive::LineList<struct VertexWithColor>(device,horizontalLineVertexList);
			horizontalLineList.setRotation(D3DXVECTOR3(0,0,((float)this->roll/180)*D3DX_PI));
			horizontalLineList.render();

			//�м��+Ҫ�������ɲ���Ⱦ�����򡭡�����������Ź�תһ����ת
			struct VertexWithColor vertexArray[4];//������ջ�Ͽ��ٵģ��Զ��������Բ����ͷ�
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

			//��ֻ����Ĳ��������ˣ����˶���
			for(UINT8 aaaaaaaaa = 0;aaaaaaaaa <4;aaaaaaaaa++)
			{
				vertexArray[aaaaaaaaa].z = (float)this->Z;
				vertexArray[aaaaaaaaa].color = D3DCOLOR_XRGB(0,0xff,0);
			}

			std::list<std::pair<struct VertexWithColor,struct VertexWithColor>> aaaaaaaaaaaaaa;
			//û����������������
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