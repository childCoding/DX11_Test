#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "objLoader.h"
using namespace std;

inline bool to3Int(string str,string flag,int& f1,int& f2,int& f3)
{
	
	size_t last = 0;  
	size_t index = str.find_first_of(flag,last);  

	string s1 = str.substr(last,index - last);  
	last = index + 1;  

	index = str.find_first_of(flag,last);  
	string s2 = str.substr(last,index - last);  
	last = index + 1;  
	
	index = str.find_first_of(flag,last);  
	string s3 = str.substr(last,index - last);  
	last = index + 1;  

	istringstream o1(s1),o2(s2),o3(s3);
	o1>>f1;
	o2>>f2;
	o3>>f3;

	return true;
}

ModelObj::ModelObj(string p):mtl(nullptr) {
	
	pathfile = p;
	size_t index = pathfile.find_last_of("\\");
	path = pathfile.substr(0,index);


	readfile();
}

ModelObj::~ModelObj() {
	
	delete mtl;
	cout<<"delModel"<<endl;
}


void ModelObj::readfile()
{
	ifstream infile(pathfile.c_str());
	string sline;//每一行


	std::string s1;
	float f2,f3,f4;
	MeshData mesh;
	int index = 0;
	//std::vector<XMFLOAT3>  vs;  //顶点
	//std::vector<XMFLOAT3>  vns; //法线
	//std::vector<XMFLOAT2>  vts; //纹理映射
	while(getline(infile,sline)) {
		string first;
		istringstream in(sline);
		in>>first;
		if (first == "v")
		{
			istringstream ins(sline);
			ins>>s1>>f2>>f3>>f4;
			m_vs.push_back(XMFLOAT3(f2,f3,f4));
		}else if(first == "vn")
		{
			istringstream ins(sline);
			ins>>s1>>f2>>f3>>f4;
			m_vns.push_back(XMFLOAT3(f2,f3,f4));
		}else if(first == "vt")
		{
			istringstream ins(sline);
			ins>>s1>>f2>>f3>>f4;
			m_vts.push_back(XMFLOAT2(1.f-f2,1.f-f3));
		}else if(first == "g")
		{
			if (!mesh.name.empty())
			{
				meshs.push_back(mesh);	//推一个拷贝进去
				mesh.vertices.clear();	//然后清除掉
				mesh.indices.clear();	//然后清除掉
				index = 0;
			}
			istringstream ins(sline);
			ins>>s1>>mesh.name;

		}else if(first == "usemtl")
		{
			istringstream ins(sline);
			ins>>s1>>mesh.material;
		}else if(first == "f")
		{
			string v1,v2,v3;
			int i1,i2,i3;
			istringstream ins(sline);
			ins>>s1>>v1>>v2>>v3;
			to3Int(v1,"/",i1,i2,i3);
			mesh.indices.push_back(index++);
			mesh.vertices.push_back( NormalTexVertex( m_vs[i1-1],m_vns[i3-1],m_vts[i2-1]));
			to3Int(v2,"/",i1,i2,i3);
			mesh.indices.push_back(index++);
			mesh.vertices.push_back( NormalTexVertex( m_vs[i1-1],m_vns[i3-1],m_vts[i2-1]));
			to3Int(v3,"/",i1,i2,i3);
			mesh.indices.push_back(index++);
			mesh.vertices.push_back( NormalTexVertex( m_vs[i1-1],m_vns[i3-1],m_vts[i2-1]));
		}else if (first == "mtllib")
		{
			string s2;
			istringstream ins(sline);
			ins>>s1>>s2;
			if (!mtl)
			{
				mtl = new MtlObj(path + "\\" + s2);
				mtl->readfile();
			}

		}
	}

	meshs.push_back(mesh);	//推一个拷贝进去
	mesh.vertices.clear();	//然后清除掉
	mesh.indices.clear();	//然后清除掉

	infile.close();
}