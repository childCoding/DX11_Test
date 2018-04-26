#pragma once
#include "../dx113dbase.h"
#include "../../ResourceProvider/objModel.h"

class DX11ModelDemo :
	public DX113DBase
{
public:
	DX11ModelDemo(void);
	~DX11ModelDemo(void);
public:

	virtual bool LoadContent();
	virtual void UnloadContent();

	virtual void Render() ;
	virtual void Update(float dtime) ;

private:

	ID3D11InputLayout* inputLayout_;

	ID3D11SamplerState* colorMapSampler_;

	ObjModel* model_;
};

