#include <string>
#include <map>
using namespace std;

#ifndef MTLLOADER_H_
#define MTLLOADER_H_

struct mtlTexture
{
	mtlTexture(){}
	mtlTexture(string n):name(n){}


	string name;

	float Ns,Ni,d,Tr,illum;
	float Ka,Kd,Ks,Ke,Tf;

	string map_ka;
	string map_kd;
};

class MtlObj {
private:

	map<string,mtlTexture> mtls;

	string path;

public:

	MtlObj();
	MtlObj(string p);
	~MtlObj();

	void readfile();
	string getTextureByName(string name);

	int size();
};

#endif /* MTLLOADER_H_ */
