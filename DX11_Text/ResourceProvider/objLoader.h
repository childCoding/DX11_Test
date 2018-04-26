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
//����һ��ͨ�õĶ���ṹ��λ�á����ߡ����ߡ���������
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

//��������ṹ�����㼯��+��������
struct MeshData
{
	std::vector<NormalTexVertex> vertices;
	std::vector<UINT>	indices;

	int indiceStart;
	int verticeStart;
	string material;  //��������
	string name;   //����
};

class ModelObj {
private:

	MtlObj* mtl; //����������ڴ����������ݵļ���

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
