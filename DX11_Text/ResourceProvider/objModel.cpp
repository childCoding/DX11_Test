#include "objModel.h"

inline void ConvertVertex(NormalTexVertex& from,DX113DBase::Vertex& to)
{
	to.pos =  from.pos;
	to.tex =  from.tex;
}

ObjModel::ObjModel(ModelObj* obj,ID3D11Device* d3d) {
	objLoader=obj;
	m_d3d=d3d;

	initVertices();
	initTextures();
}

ObjModel::~ObjModel() {
	for(auto i:textures) {
		i.second->Release();
	}
	
	if (vertexBuffer_) vertexBuffer_->Release();
	if( indexBuffer_ ) indexBuffer_->Release( );

	delete[] vertices;
	delete[] indices;
	delete objLoader;
}

void ObjModel::initVertices() {

	int vcount = 0,icount = 0;
	for (auto it:objLoader->meshs)
	{
		vcount += it.vertices.size();
		icount += it.indices.size();
	}
	
	int vcur = 0 ,icur = 0;
	vertices = new DX113DBase::Vertex[vcount];
	indices = new UINT[icount];
	for(auto& im:objLoader->meshs )
	{	
		im.verticeStart = vcur;
		for(auto it:im.vertices)
		{
			ConvertVertex(it,vertices[vcur]);			
			vcur++;
		}

		im.indiceStart = icur;
		for(auto it:im.indices)
		{
			indices[icur] = it;		
			icur++;
		}
	}

	//加载顶点数据
	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory( &vertexDesc, sizeof( vertexDesc ) );
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof( DX113DBase::Vertex ) * vcount;
	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory( &resourceData, sizeof( resourceData ) );
	resourceData.pSysMem = vertices;
	HRESULT d3dResult = m_d3d->CreateBuffer( &vertexDesc, &resourceData, &vertexBuffer_ );
	if( FAILED( d3dResult ) )
	{
		DXTRACE_MSG( "Failed to create vertex buffer!" );
		return ;
	}
	
	//加载索引数据
	D3D11_BUFFER_DESC indexDesc;
	ZeroMemory( &indexDesc, sizeof( indexDesc ) );
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.ByteWidth = sizeof( UINT ) * icount;
	indexDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA ibData;
	ZeroMemory( &ibData, sizeof( ibData ) );
	ibData.pSysMem = indices;

	d3dResult = m_d3d->CreateBuffer( &indexDesc, &ibData, &indexBuffer_ );
	if( FAILED( d3dResult ) )
	{
		DXTRACE_MSG( "Failed to create index buffer!" );
		return ;
	}
	
}

void ObjModel::initTextures() {
	
	mtlNum = objLoader->mtl->size();

	for(auto it:objLoader->meshs) {
		string texFile = objLoader->mtl->getTextureByName(it.material) ;
		textures.insert(pair<string,ID3D11ShaderResourceView*>(it.material,nullptr));

		HRESULT d3dResult;
		d3dResult = D3DX11CreateShaderResourceViewFromFile( m_d3d, texFile.c_str(), 0, 0,&textures[it.material], 0 );
		if( FAILED( d3dResult ) )
		{
			string out ="\"" + texFile + "\"" + " Failed to load the texture image!";
			
			DXTRACE_MSG( out.c_str() );
			continue;
		}
	}
}

void ObjModel::render(ID3D11DeviceContext* D3D11DeviceContext_,ID3DX11Effect* effect_) {


	unsigned int stride = sizeof(DX113DBase::Vertex);
	unsigned int offset = 0;
	D3D11DeviceContext_->IASetVertexBuffers(0,1,&vertexBuffer_,&stride,&offset);
	D3D11DeviceContext_->IASetIndexBuffer(indexBuffer_,DXGI_FORMAT_R32_UINT,0);
	D3D11DeviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for(DWORD i=0;i<(DWORD)objLoader->meshs.size();i++) {

		
		auto it = textures.find(objLoader->meshs[i].material) ;
		if (it == textures.end())
		{
			DXTRACE_MSG("find not textures" );
		}

		ID3DX11EffectShaderResourceVariable* colorMap;
		colorMap = effect_->GetVariableByName( "colorMap_" )->AsShaderResource( );
		colorMap->SetResource( it->second );

		ID3DX11EffectTechnique* colorInvTechnique;
		colorInvTechnique = effect_->GetTechniqueByName( "ColorShift" );
		D3DX11_TECHNIQUE_DESC techDesc;
		colorInvTechnique->GetDesc( &techDesc );
		for( unsigned int p = 0; p < techDesc.Passes; p++ )
		{
			ID3DX11EffectPass* pass = colorInvTechnique->GetPassByIndex( p );
			if( pass != 0 )
			{
				pass->Apply( 0, D3D11DeviceContext_ );
				//D3D11DeviceContext_->Draw( objLoader->meshs[i].vertices.size(), objLoader->meshs[i].verticeStart );
				
				D3D11DeviceContext_->DrawIndexed( objLoader->meshs[i].vertices.size(),objLoader->meshs[i].indiceStart, objLoader->meshs[i].verticeStart);
			}
		}
	}
}

void ObjModel::renderSimple() {
	/*
	for(DWORD i=0;i<(DWORD)objLoader->groupNum;i++)
		mesh->DrawSubset(i);
		*/
}

