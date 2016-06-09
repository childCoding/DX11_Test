#pragma once
#include "DX11FontDemo.h"
#include <iostream>

//freetype library
#include  <ft2build.h>
#include FT_FREETYPE_H

class DX11FreeTypeFontDemo
	:public DX11FontDemo
{
public:
	DX11FreeTypeFontDemo(std::string pathname);
	~DX11FreeTypeFontDemo(void);

	void Rasterise(const void* buffer,UINT iWidth,UINT iHeight);
	void DrawGlyphToBitmap(const void* buffer,UINT iWidth,UINT iHeight);

	virtual bool LoadContent();
	virtual void UnloadContent();

	virtual void Render() ;
	virtual void Update(float dtime) ;

private:
	FT_Library library;
	FT_Face face;

	ID3D11Texture2D* fonttex_;
};

