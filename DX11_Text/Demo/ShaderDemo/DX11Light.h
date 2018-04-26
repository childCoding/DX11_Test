#pragma once
#include "../dx113dbase.h"
#include "../../ResourceProvider/objModel.h"

class DX11Light:
	public DX113DBase
{
public:
	DX11Light(void);
	~DX11Light(void);

	virtual bool LoadContent();
	virtual void UnloadContent();

	virtual void Render() ;
	virtual void Update(float dtime) ;

private:
	ID3DX11Effect* effect_;

	ID3D11InputLayout* inputLayout_;

	ID3D11SamplerState* colorMapSampler_;

	ObjModel* model_;
};

