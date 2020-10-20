#include <vector>
#include <string>
#include "CImg.h"
#include <cmath>
#include <dirent.h>
#define cimg_use_jpeg 1
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;
using namespace cimg_library;
using namespace std;


class Nodo{
public:
    vector<float> vc;
     char* nombre; //large
     string name; //corto
public:
    vector<float>  Vectorizar(CImg<float> & img ){
	     vector<float> R;
	     cimg_forXY(img,x,y){
		  R.push_back( (img(x,y,0) + img(x,y,1) +  img(x,y,2))/3);
	      }
	     return R;
	}

    Nodo(string nom, string ruta){
	strcpy(this->nombre, ruta.c_str());
	CImg<float>   A(nombre);
	A.resize(512, 512);
	CImg<float>   B =  A.haar(false,3);
	CImg<float>   c  = B.crop(0,0,63,63);
	vector<float> vec = Vectorizar(c);
	this->vc = vec;
	this->name=nom;
    }
};

class Arista{
private:
    Nodo* A;
    Nodo* B;
    float distancia;
public:
    Arista(Nodo* n1 , Nodo* n2){
	A=n1;
	B= n2;
	distancia= Distancia(A,B);
    }
    float Distancia(Nodo* v1, Nodo* v2){
	float total = 0;

	for(int i=0; i< v1->vc.size(); ++i){
	    total += pow( (v1->vc)[i]-(v2->vc)[i] , 2);
	}

	total = sqrt(total);
	return total;
    }

};

int main()
{
/*
//find /home/cristian/ -type f -name "*.jpg"
   CImg<float>   A("cara_p.jpg");
   A.resize(512, 512);
   CImg<float>   B =  A.haar(false,3);
   CImg<float>   c  = B.crop(0,0,63,63);

   vector<float> vc = Vectorizar(c);

   //A.display();
   //B.display();
   c.display();
*/

    vector<Nodo*> vecN;
    vector<Arista> vecA;
    std::string path("/home/yoncho/Documents/Cursos_UTEC/Ciclo_6/EDA/faces/faces94");
    std::string ext(".jpg");
    cout<<"llegue aqui";
    for (auto &p : fs::recursive_directory_iterator(path))
    {
        if (p.path().extension() == ext){
            //std::cout << p.path().string() << '\n';
	    vecN.push_back(new Nodo(p.path().string(), p.path().stem().string()) );
	}
    }
    int i,j;
    for (i=0; i< vecN.size()-1; i++){
	for(j=1; i< vecN.size(); j++){
	    Arista AR(vecN[i], vecN[j]);
	    vecA.push_back(AR);
	}
    }
    //p.path().stem().string()
    return 0;
}
