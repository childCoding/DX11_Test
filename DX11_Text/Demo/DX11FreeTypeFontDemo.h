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

	void creatTextureFromFont(wchar_t* str);
	void Rasterise(const void* buffer,UINT iWidth,UINT iHeight);
	void DrawGlyphToBitmap( UINT* buffer,int buffer_width,FT_GlyphSlot glyph);

	virtual bool LoadContent();
	virtual void UnloadContent();

	virtual void Render() ;
	virtual void Update(float dtime) ;

	bool DrawString(wchar_t* str,float x,float y);
private:
	FT_Library library;
	FT_Face face;

	ID3D11Texture2D* fonttex_;
};

