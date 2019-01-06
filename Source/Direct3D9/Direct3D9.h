#pragma once
#include "d3d9.h"
#include "d3dx9.h"

#include <list>
#include <map>
#include <string>

namespace Direct3D
{
	namespace v9
	{
		namespace initialization
		{
			//if the master GPU support,return D3DCREATE_HARDWARE_VERTEXPROCESSING
			//otherwise return D3DCREATE_SOFTWARE_VERTEXPROCESSING
			UINT32 renderByHardwareOrSoftware(IDirect3D9 *direct3D9, D3DDEVTYPE deviceType);

			bool createDevice(IDirect3D9 *direct3D9, D3DDEVTYPE deviceType, HWND hwnd, UINT32 byHardwareOrSoftware,
				D3DPRESENT_PARAMETERS presentParameters, IDirect3DDevice9 **device
				);

			D3DPRESENT_PARAMETERS createPresentParameters(HWND hwnd, UINT16 width, UINT16 height, bool isFullScreen);

			HWND createWindow(HINSTANCE hInstance, WNDPROC wndProc, UINT16 width, UINT16 height, bool isFullScreen);

			LRESULT CALLBACK defaultWindowProc(
				HWND hwnd,
				UINT uMessage,
				WPARAM wParam,
				LPARAM lParam
				);

			bool initialize(HINSTANCE hInstance, WNDPROC wndProc, UINT16 width, UINT16 height, bool isFullScreen, D3DDEVTYPE deviceType, IDirect3DDevice9 **device,HWND *windowHandle = NULL);
		}

		void enterMessageLoop(
			bool (*displayFunction)(double totalTime,double deltaTime,IDirect3DDevice9 *device,HWND windowHandle),
			IDirect3DDevice9 *device,HWND windowHandle
			);

		namespace configuration
		{
			void setStaticCamera(
				IDirect3DDevice9 *device,
				D3DXVECTOR3 position,D3DXVECTOR3 target,
				float angle,UINT16 viewPortWidth,UINT16 viewPortHeight,
				float zMinToRender,float zMaxToRender
				);
		}

		namespace resource
		{
			class OneFrameLifecycleResource
			{
				//������
			private:
				OneFrameLifecycleResource();
				~OneFrameLifecycleResource();
				static OneFrameLifecycleResource *instance;
			public:
				static OneFrameLifecycleResource *getInstance();
				//���ϵĲ�������ʵ�ֵ���ģʽ

				void clear(IDirect3DDevice9 *device);//������һ֡���������

				//�����µĴ�С
				UINT64 addMesh(IDirect3DDevice9 *device,ID3DXMesh *mesh);

			private:
				std::map<IDirect3DDevice9 *,std::list<ID3DXMesh *>> mesh;
				//��Ҫ���VertexBuffer��
			};

			namespace primitive
			{
				namespace vertex
				{
					struct VertexWithColor
					{
						float x,y,z;
						D3DCOLOR color;

						static const DWORD flexibleVectorFormat;
					};
				}
				namespace buffer
				{
					template<typename Vertex>
					class VertexBuffer
					{
					public:
						VertexBuffer(
							IDirect3DDevice9 *device,UINT length,
							DWORD usage = D3DUSAGE_WRITEONLY,
							D3DPOOL pool = D3DPOOL_MANAGED
							)
						{
							length = length>1048576?1048576:length;
							device->CreateVertexBuffer(length*sizeof(Vertex),usage,Vertex::flexibleVectorFormat,pool,&(this->d3dVectorBuffer),0);
							this->length = length;
						}

						VertexBuffer(
							IDirect3DDevice9 *device,std::list<Vertex> vertexList,
							DWORD usage = D3DUSAGE_WRITEONLY,
							D3DPOOL pool = D3DPOOL_MANAGED
							)
						{
							this->length = (UINT)vertexList.size();
							device->CreateVertexBuffer((this->length)*sizeof(Vertex),usage,Vertex::flexibleVectorFormat,pool,&(this->d3dVectorBuffer),0);


							Vertex *vertexPointer = NULL;
							this->d3dVectorBuffer->Lock(0,0,(void **)&vertexPointer,0);

							UINT index = 0;
							for(std::list<Vertex>::iterator vertexListIterator = vertexList.begin();
								vertexListIterator != vertexList.end();vertexListIterator++
								)
							{
								*(vertexPointer+index) = *vertexListIterator;
								index++;
							}
							this->d3dVectorBuffer->Unlock();
						}

						UINT getLength()
						{
							return this->length;
						}

						Vertex *lock()
						{
							Vertex *vertexPointer = NULL;
							this->d3dVectorBuffer->Lock(0,0,(void **)&vertexPointer,0);
							return vertexPointer;
						}
						void unlock()
						{
							this->d3dVectorBuffer->Unlock();
						}

						IDirect3DVertexBuffer9 *getDirect3D9VertexBufferPointer()
						{
							return this->d3dVectorBuffer;
						}

					private:
						IDirect3DVertexBuffer9 *d3dVectorBuffer;
						UINT length;
					};

					template<typename IndexType>
					class IndexBuffer
					{
					public:
						IndexBuffer(
							IDirect3DDevice9 *device,UINT length,
							DWORD usage = D3DUSAGE_WRITEONLY,
							D3DFORMAT format = D3DFMT_INDEX32,
							D3DPOOL pool = D3DPOOL_MANAGED
							)
						{
							length = length>1048576?1048576:length;
							switch(sizeof(IndexType))
							{
							case 2:
								format = D3DFMT_INDEX16;
								device->CreateIndexBuffer(length*sizeof(IndexType),usage,format,pool,&(this->d3dIndexBuffer),0);
								break;
							case 4:
								format = D3DFMT_INDEX32;
								device->CreateIndexBuffer(length*sizeof(IndexType),usage,format,pool,&(this->d3dIndexBuffer),0);
								break;
							default:
								device->CreateIndexBuffer(length*sizeof(IndexType),usage,format,pool,&(this->d3dIndexBuffer),0);
								break;
							}
							this->length = length;
						}
						UINT getLength()
						{
							return this->length;
						}

						IndexType *lock()
						{
							IndexType *indexType;
							this->d3dIndexBuffer->Lock(0,0,(void **)&indexType,0);
							return indexType;
						}
						void unlock()
						{
							this->d3dIndexBuffer->Unlock();
						}

					private:
						IDirect3DIndexBuffer9 *d3dIndexBuffer;
						UINT length;
					};
				}
			}

			namespace vector
			{
				D3DXVECTOR3 rotate(D3DXVECTOR3 vector,D3DXVECTOR3 rotation);
			}


			namespace mesh
			{
				//���� 
				namespace text
				{
					class TextMesh;
				}
				class MeshFactory;

				class Mesh
				{
				public:
					Mesh();//��Visual Studio 2005�в�֧��C++ 11�е�=delete����

					//��ÿ�����Mesh,FVFֱ�Ӱ���ԭFVF
					ID3DXMesh *getClonedMesh();
					//��ֱ�ӻ����Ӽ�
					//virtual HRESULT DrawSubset(DWORD attributeId = 0);

				private:
					//ʹ��Direct 3D���õ�Meshָ�봴��Mesh
					//ֻ������Ԫ�����
					Mesh(ID3DXMesh *d3dxMesh,DWORD FVF,IDirect3DDevice9 *device);

					//MeshFactory must be the friend class of this class
					//so that the FACTORY can generate product
					friend class MeshFactory;

					DWORD flexibleVertexFormat;
					ID3DXMesh *d3dxMesh;
					IDirect3DDevice9 *device;
				};

				class MeshFactory
				{
				public:
					//Mesh�Ĺ����࣬���Դ���Mesh
					//ע�⣬ֻ��MeshFactoryӦ�ñ�����Ϊ��ԭ���ϡ��͡���Ʒ������Ԫ��
					//ӵ����ȫ����Ȩ������������Ʒ��
					//��Ӧ����Mesh�������κ�һ����ƷΪ��Ԫ��
					//�����Ʒ������Ϳ��Է���
					static text::TextMesh createTextMesh(IDirect3DDevice9 *device,std::string content,std::string font,int weight = 200);
				};


				namespace text
				{
					struct TextVertex
					{
						float x,y,z;
						float nx,ny,nz;
						//DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL��Ӧ�Ķ����ʽ
						//֮����֪����Text�л�ȡ���������Ķ����ʽ��
						//���ǵ��������18Ҳ����0x12�����Ծ���D3DFVF_XYZ | D3DFVF_NORMAL
					};

					class TextMesh
					{
					private:
						//ָ��MeshFactoryΪ�����࣬ʹ��MeshFactory���Դ���TextMesh
						friend class Direct3D::v9::resource::mesh::MeshFactory;

						Direct3D::v9::resource::mesh::Mesh mesh;

						//�����������ǣ�center�ǲ���Ҫ����ģ���Ϊrender�ĺ�����Pure��
						//D3DXVECTOR3 center,translation,rotation;
						D3DXVECTOR3 translation,rotation;
						//��Ҫ����һ��������Cube������ʶ��
						//����Ӧ�ý�scale�ͱ߳���ϳ�һ���ڲ��࣬ͳһ����
						class Scale
						{
						private:
							D3DXVECTOR3 scale,sideLength;
						public:
							//Scale() = delete;
							Scale();//Visual Studio 2005��֧��C++ 11���ԣ����Բ����� = delete
							Scale(D3DXVECTOR3 sideLength,D3DXVECTOR3 scale = D3DXVECTOR3(1.0f,1.0f,1.0f));
							D3DXVECTOR3 setByScale(D3DXVECTOR3 scale);
							D3DXVECTOR3 setBySideLength(D3DXVECTOR3 sideLength);
							D3DXVECTOR3 getScale();
							//��������ı߳������Ӧ��scale;
							D3DXVECTOR3 getScale(D3DXVECTOR3 sideLength);
							D3DXVECTOR3 getSideLength();
							//���������scale�����Ӧ�ı߳�
							D3DXVECTOR3 getSideLength(D3DXVECTOR3 scale);
						} scale;
						TextMesh(Direct3D::v9::resource::mesh::Mesh mesh,D3DXVECTOR3 sideLength);
					public:
						~TextMesh();

						static int convertStringToWideChar(std::string toConvert,LPWSTR *result);
						static int convertMultiByteToWideChar(const char *toConvert,LPWSTR *result);

						//todo
						void setTranslation(D3DXVECTOR3 translation);
						void setRotation(D3DXVECTOR3 rotation);
						void setScale(D3DXVECTOR3 scale);
						void setSideLength(D3DXVECTOR3 sideLength,bool keepAspectRatio = true);
						//֮����Ҫ����device������ΪҪ����ԭ�е�Mesh������ԭʼ���ɣ���ƽ�Ƶ�ԭ�㣩��Mesh��ֱ��Draw
						void render(IDirect3DDevice9 *device);//��Ⱦ
					};
				}
			}
		}
	}
}