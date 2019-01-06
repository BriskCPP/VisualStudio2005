#include "Direct3D9.h"

namespace Direct3D
{
	namespace v9
	{
		namespace resource
		{
			OneFrameLifecycleResource *OneFrameLifecycleResource::instance = NULL;
			OneFrameLifecycleResource::OneFrameLifecycleResource()
			{
				this->mesh.clear();
			}
			OneFrameLifecycleResource::~OneFrameLifecycleResource()
			{
				if(instance != NULL)
				{
					delete instance;
					instance = NULL;
				}
			}
			OneFrameLifecycleResource *OneFrameLifecycleResource::getInstance()
			{
				if(instance == NULL) instance = new OneFrameLifecycleResource;
				while(instance == NULL);//等待加载
				return instance;
			}
			void OneFrameLifecycleResource::clear(IDirect3DDevice9 *device)
			{
				//release all the mesh
				std::map<IDirect3DDevice9 *,std::list<ID3DXMesh *>>::iterator meshMapIterator = this->mesh.find(device);
				if(meshMapIterator != this->mesh.end())
				{
					//std::list<ID3DXMesh *> meshList = meshMapIterator->second;
					for(std::list<ID3DXMesh *>::iterator meshListIterator = meshMapIterator->second.begin();
						meshListIterator != meshMapIterator->second.end();meshListIterator++)
					{
						//ID3DXMesh *currentMesh = (ID3DXMesh *)*meshListIterator;
						(*meshListIterator)->Release();
					}
					meshMapIterator->second.clear();

						
				}
			}

			UINT64 OneFrameLifecycleResource::addMesh(IDirect3DDevice9 *device,ID3DXMesh *mesh)
			{
				
				std::map<IDirect3DDevice9 *,std::list<ID3DXMesh *>>::iterator meshMapIterator = this->mesh.find(device);
				if(meshMapIterator == this->mesh.end())
				{
					//现在在map中暂时木有，所以要创建
					std::list<ID3DXMesh *> meshList;
					meshList.push_back(mesh);

					this->mesh[device] = meshList;
					return (UINT64)meshList.size();
				}
				else
				{
					//列表里已经有了
					meshMapIterator->second.push_back(mesh);
					return (UINT64)meshMapIterator->second.size();
				}
			}
		}
	}
}