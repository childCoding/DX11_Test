#include <string>
#include <vector>
#include <map>
#include <d3dcompiler.h>
#include <xnamath.h>
#include "mtlLoader.h"

#ifndef OBJLOADER_H_
#define OBJLOADER_H_

/*
struct NormalTexVertex
{
	float x,y,z;
	float nx,ny,nz;
	float u,v;
};
*/
//定义一个通用的顶点结构：位置、法线、切线、纹理坐标
struct NormalTexVertex
{
	NormalTexVertex(){}
	NormalTexVertex(const XMFLOAT3 _pos, XMFLOAT3 _normal, XMFLOAT2 _tex):
		pos(_pos),normal(_normal),tex(_tex){}
	NormalTexVertex(const XMFLOAT3 _pos, XMFLOAT3 _normal, XMFLOAT3 _tangent, XMFLOAT2 _tex):
		pos(_pos),normal(_normal),tangent(_tangent),tex(_tex){}

	XMFLOAT3	pos;
	XMFLOAT3	normal;
	XMFLOAT3	tangent;
	XMFLOAT2	tex;
};

//基本网络结构：顶点集合+索引集合
struct MeshData
{
	std::vector<NormalTexVertex> vertices;
	std::vector<UINT>	indices;

	int indiceStart;
	int verticeStart;
	string material;  //纹理名称
	string name;   //名称
};

class ModelObj {
private:

	MtlObj* mtl; //纹理对象、用于存在纹理数据的集合

public:
	typedef std::vector<XMFLOAT3>  VecFloat3;
	typedef std::vector<XMFLOAT2>  VecFloat2;

	std::string pathfile;
	std::string path;
	VecFloat3 m_vs,m_vns;
	VecFloat2 m_vts;
	std::vector<MeshData> meshs;


	ModelObj(std::string p);
	~ModelObj();
	void readfile();
public: 
	int GetMaterialCount(){ return mtl ? mtl->size():0;}
	string GetTextureByName(string name) {return  mtl ? mtl->getTextureByName(name):"";}
};

#endif /* OBJLOADER_H_ */
