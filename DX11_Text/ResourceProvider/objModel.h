#include "objLoader.h"
#include "../demo/dx113dbase.h"
#include <d3dx11effect.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <DXErr.h>
#include <d3dcompiler.h>


#ifndef OBJMODEL_H_
#define OBJMODEL_H_

class ObjModel {


public:
	ObjModel(ModelObj* obj,ID3D11Device* d3d);
	~ObjModel();
	void initVertices();
	void initTextures();
	void render(ID3D11DeviceContext* D3D11DeviceContext_,ID3DX11Effect* effect_);
	void renderSimple();

private:
	//LPD3DXMESH mesh;
	ModelObj* objLoader;
	int mtlNum;
	//LPDIRECT3DTEXTURE9* textures;
	map<string,ID3D11ShaderResourceView*> textures;
	

	ID3D11Device* m_d3d;

	DX113DBase::Vertex* vertices;
	UINT* indices;

	ID3D11Buffer* vertexBuffer_;
	ID3D11Buffer* indexBuffer_;
};

#endif /* OBJMODEL_H_ */
