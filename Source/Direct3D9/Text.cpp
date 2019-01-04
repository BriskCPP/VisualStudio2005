#include "Direct3D9.h"

namespace Direct3D
{
	namespace v9
	{
		namespace text
		{
			Text::Text(IDirect3DDevice9 *device,std::string content,std::string font,int weight /* = 200 */)
			{
				//������Ĺ��캯��

				LPWSTR fontName;
				convertStringToWideChar(font,&fontName);
				HFONT hFont = CreateFont(10,5,0,0,weight,0,0,0,DEFAULT_CHARSET,0,0,0,0,(LPCWSTR)fontName);
				delete[] fontName;

				LPWSTR tempContent;
				convertStringToWideChar(content,&tempContent);
				//�һ��������ø���ȥ�����������̨
				//����ɵĸ���


				//��������
				HDC hdc = CreateCompatibleDC(0);//��������
				HFONT originalFont = (HFONT)SelectObject(hdc,hFont);//������ѡ���豸����,������һ�����壬�Ա�����ѡ��
				D3DXCreateText(device,hdc,(LPCWSTR)tempContent,0.000001f,0.000001f,&this->d3dMesh,NULL,NULL);//�����ı�����
				SelectObject(hdc,originalFont);//ѡ��ԭ��������
				DeleteObject(hFont);
				DeleteDC(hdc);

				delete[] tempContent;

				this->pretransform();//Ԥ�任

				//д�㷨����Ƿǳ������Խ�Ĺ�������Ҳ��֪����ʦΪʲô  ��һ��Ҫ������������һЩ�ǳ����ѵ�����
			}

			void Text::pretransform()
			{
				//����Ԥ�任
				//��ê�㰲�ŵ����ֵ�����
				this->bound.rightTop.x = -(1024.0f*1024.0f*1024.0f);
				this->bound.rightTop.y = -(1024.0f*1024.0f*1024.0f);
				this->bound.rightTop.z = -(1024.0f*1024.0f*1024.0f);
				this->bound.leftBottom.x = 1024.0f*1024.0f*1024.0f;
				this->bound.leftBottom.y = 1024.0f*1024.0f*1024.0f;
				this->bound.leftBottom.z = 1024.0f*1024.0f*1024.0f;

				struct Direct3D::v9::text::TextVertex *pVertex = NULL;
				this->d3dMesh->LockVertexBuffer(0,(void **)&pVertex);
				for(DWORD i = 0;i < this->d3dMesh->GetNumVertices();i++)
				{
					this->bound.leftBottom.x = pVertex[i].x<this->bound.leftBottom.x?pVertex[i].x:this->bound.leftBottom.x;
					this->bound.leftBottom.y = pVertex[i].y<this->bound.leftBottom.y?pVertex[i].y:this->bound.leftBottom.y;
					this->bound.leftBottom.z = pVertex[i].z<this->bound.leftBottom.z?pVertex[i].z:this->bound.leftBottom.z;

					this->bound.rightTop.x = pVertex[i].x>this->bound.rightTop.x?pVertex[i].x:this->bound.rightTop.x;
					this->bound.rightTop.y = pVertex[i].y>this->bound.rightTop.y?pVertex[i].y:this->bound.rightTop.y;
					this->bound.rightTop.z = pVertex[i].z>this->bound.rightTop.z?pVertex[i].z:this->bound.rightTop.z;
				}
				//���������֮��Ӧ�ñ任
				D3DXVECTOR3 initialCenter = D3DXVECTOR3(
					(this->bound.rightTop.x + this->bound.leftBottom.x)/2,
					(this->bound.rightTop.y + this->bound.leftBottom.y)/2,
					(this->bound.rightTop.z + this->bound.leftBottom.z)/2
					);//�����ı�����ʱ������
				for(DWORD i = 0;i < this->d3dMesh->GetNumVertices();i++)
				{
					//����ƽ��
					pVertex[i].x -= initialCenter.x;
					pVertex[i].y -= initialCenter.y;
					pVertex[i].z -= initialCenter.z;
				}
				this->d3dMesh->UnlockVertexBuffer();
				this->center = D3DXVECTOR3(0,0,0);
				this->bound.leftBottom.x -= initialCenter.x;
				this->bound.rightTop.x -= initialCenter.x;
				this->bound.leftBottom.y -= initialCenter.y;
				this->bound.rightTop.y -= initialCenter.y;
				this->bound.leftBottom.z -= initialCenter.z;
				this->bound.rightTop.z -= initialCenter.z;

				this->translation = D3DXVECTOR3(0,0,0);
				this->rotation = D3DXVECTOR3(0,0,0);
				this->scale = D3DXVECTOR3(1,1,1);
			}

			ID3DXMesh *Text::getMesh(D3DXVECTOR3 leftBottom,D3DXVECTOR3 rightTop)
			{
				//
				D3DXVECTOR3 translation = rightTop/2 + leftBottom/2;
				D3DXVECTOR3 scale;
				scale.x = (rightTop.x-leftBottom.x)/(this->bound.rightTop.x - this->bound.leftBottom.x);
				scale.y = (rightTop.y-leftBottom.y)/(this->bound.rightTop.y - this->bound.leftBottom.y);
				scale.z = (rightTop.z-leftBottom.z)/(this->bound.rightTop.z - this->bound.leftBottom.z);

				if(scale.x<scale.y) scale.y = scale.x;
				if(scale.x>scale.y) scale.x = scale.y;

				this->setTranslation(translation);
				this->setScale(scale);
				this->setRotation(D3DXVECTOR3(0.0f,0.0f,0.0f));
				//
				return this->getMesh();
			}

			ID3DXMesh *Text::getMesh()
			{
				ID3DXMesh *transformedMesh = this->d3dMesh;
				//�Ժ��Ƕ�������в��������ٸı�ԭʼ��
				//����������setTranslation()ʱ���Ѿ��ı���
				//���Ա����Ƚ���ƽ�Ʋ���
				//0.ƽ��
				struct Direct3D::v9::text::TextVertex *pVertex = NULL;
				transformedMesh->LockVertexBuffer(0,(void **)&pVertex);
				for(DWORD i = 0;i < transformedMesh->GetNumVertices();i++)
				{
					pVertex[i].x += this->translation.x;
					pVertex[i].y += this->translation.y;
					pVertex[i].z += this->translation.z;
					//��ʱ���ı䶥�㷨������Ч��
				}
				
				//1.��ת(todo)
				//��ת��ʱ��Ҫ�ı䷨���������ԾͱȽϸ��ӣ���ʱ����
				//emmmmmm����������һ�³���Ӧ��û�����˰�
				for(DWORD i = 0;i < transformedMesh->GetNumVertices();i++)
				{
					D3DXVECTOR3 relative;
					relative.x = pVertex[i].x - this->center.x;
					relative.y = pVertex[i].y - this->center.y;
					relative.z = pVertex[i].z - this->center.z;

					D3DXVECTOR3 rotated = Direct3D::v9::vector::rotate(relative,this->rotation);
					pVertex[i].x = this->center.x + rotated.x;
					pVertex[i].y = this->center.y + rotated.y;
					pVertex[i].z = this->center.z + rotated.z;

					//����Ҫ��ת������
					D3DXVECTOR3 n = D3DXVECTOR3(pVertex[i].nx,pVertex[i].ny,pVertex[i].nz);
					rotated = Direct3D::v9::vector::rotate(n,this->rotation);
					pVertex[i].nx = rotated.x;
					pVertex[i].ny = rotated.y;
					pVertex[i].nz = rotated.z;
				}
				//2.���ţ����Ų��ı�����
				for(DWORD i = 0;i < transformedMesh->GetNumVertices();i++)
				{
					D3DXVECTOR3 relative;
					relative.x = pVertex[i].x - this->center.x;
					relative.y = pVertex[i].y - this->center.y;
					relative.z = pVertex[i].z - this->center.z;
					pVertex[i].x = this->center.x + relative.x*this->scale.x;
					pVertex[i].y = this->center.y + relative.y*this->scale.y;
					pVertex[i].z = this->center.z + relative.z*this->scale.z;
					//��ʱ���ı䶥�㷨��
					//��Ϊ��Ĳ�֪�����㷨�߻�Ӱ��ʲô
				}
				transformedMesh->UnlockVertexBuffer();

				return transformedMesh;
			}


			Text::~Text()
			{
				this->d3dMesh->Release();
			}
			void Text::setTranslation(D3DXVECTOR3 translation)
			{
				this->translation = translation;
				//ƽ��Ҫ�ı����ģ�����Ͳ�����
				this->center.x += this->translation.x;
				this->center.y += this->translation.y;
				this->center.z += this->translation.z;
				//������ҲҪ�ı�
				this->bound.leftBottom.x += this->translation.x;
				this->bound.leftBottom.y += this->translation.y;
				this->bound.leftBottom.z += this->translation.z;
				this->bound.rightTop.x += this->translation.x;
				this->bound.rightTop.y += this->translation.y;
				this->bound.rightTop.z += this->translation.z;
			}
			void Text::setRotation(D3DXVECTOR3 rotation)
			{
				this->rotation = rotation;

				//�ı����±߽�
				this->bound.leftBottom = Direct3D::v9::vector::rotate((this->bound.leftBottom - this->center),this->rotation);
				this->bound.rightTop = Direct3D::v9::vector::rotate((this->bound.rightTop - this->center),this->rotation);
			}
			void Text::setScale(D3DXVECTOR3 scale)
			{
				this->scale = scale;

				//����Ҫ�ı����±߽�
				D3DXVECTOR3 relative;
				//�ȴ����±߽�
				relative.x = this->bound.leftBottom.x - this->center.x;
				relative.y = this->bound.leftBottom.y - this->center.y;
				relative.z = this->bound.leftBottom.z - this->center.z;
				this->bound.leftBottom.x = this->center.x + this->scale.x*relative.x;
				this->bound.leftBottom.y = this->center.y + this->scale.y*relative.y;
				this->bound.leftBottom.z = this->center.z + this->scale.z*relative.z;
				//�ٴ����ϱ߽�
				relative.x = this->bound.rightTop.x - this->center.x;
				relative.y = this->bound.rightTop.y - this->center.y;
				relative.z = this->bound.rightTop.z - this->center.z;
				this->bound.rightTop.x = this->center.x + this->scale.x*relative.x;
				this->bound.rightTop.y = this->center.y + this->scale.y*relative.y;
				this->bound.rightTop.z = this->center.z + this->scale.z*relative.z;
			}

			D3DXVECTOR3 Text::getLeftBottom()
			{
				return this->bound.leftBottom;
			}
			D3DXVECTOR3 Text::getRightTop()
			{
				return this->bound.rightTop;
			}

			//����ת������
			int Text::convertStringToWideChar(std::string toConvert,LPWSTR *result)
			{
				return convertMultiByteToWideChar(toConvert.c_str(),result);
			}
			int Text::convertMultiByteToWideChar(const char *toConvert,LPWSTR *result)
			{
				int resultLength = MultiByteToWideChar(CP_ACP,0,toConvert,-1,NULL,0);
				*result = (wchar_t*)malloc( resultLength*sizeof(wchar_t) );
				MultiByteToWideChar(CP_ACP,0, toConvert, -1, *result, resultLength);
				return resultLength;
			}
		}
	}
}