#pragma once
#include "dx113dbase.h"
#include "../Common/GeometryGens.h"
#include "../Common/Lights.h"

class DX11NormalMappingDemo :
	public DX113DBase
{
public:
	DX11NormalMappingDemo(void);
	~DX11NormalMappingDemo(void);

public:
	virtual bool LoadContent();
	virtual void UnloadContent();

	virtual void Render() ;
	virtual void Update(float dtime) ;

private:
	bool initInputLayout();
	bool initEffect();
	bool initResourceView();
	bool initBuffer();

	GeoGen::MeshData m_floor;

	ID3DX11Effect* effect_;
	ID3DX11EffectTechnique* m_tech;

	ID3D11InputLayout* inputLayout_;

	ID3D11Buffer* vertexBuffer_;
	ID3D11Buffer* indexBuffer_;

	ID3D11ShaderResourceView* m_floorSVR;
	ID3D11ShaderResourceView* m_floorN;
	ID3D11SamplerState* colorMapSampler_;

	Lights::Material m_material;
	Lights::DirLight m_dirLights[3];
};

