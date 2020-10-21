#include "CImg.h"
#include <dirent.h>
#define cimg_use_jpeg 1
#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include "src/FibonacciHeap.h"
#include <bits/stdc++.h>

namespace fs = std::experimental::filesystem;
using namespace cimg_library;
using namespace std;

class Nodo;
class Arista;

struct Grafo {
	vector<Nodo*> nodos;
	vector<Arista> aristas;
};

class Nodo{
public:
    vector<float> vc;
     char *nombre; //large
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
	nombre = new char[nom.size ()];
	strcpy(this->nombre, nom.c_str());
	CImg<float>   A(nombre);
	A.resize(512, 512);
	CImg<float>   B =  A.haar(false,3);
	CImg<float>   c  = B.crop(0,0,63,63);
	vector<float> vec = Vectorizar(c);
	this->vc = vec;
	this->name=ruta;
    }
};

class Arista{
public:
    Nodo* A;
    Nodo* B;
	double distancia;
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

Grafo kruskal (Grafo g) {
	Grafo minSpan;
	FibonacciHeap<double> q;
	unordered_map<string, Nodo *> currentNodes;
	vector<Arista> aristas = g.aristas;
	
	for (int i = 0; i < aristas.size (); i++) {
		BNode<double> *temp = new BNode<double> (aristas[i].distancia, &aristas[i]);	
		q.Insert (temp);
	}
	
	while (!q.Empty ()) {
		Arista current = * ((Arista *) (q.DeleteMin ()->structure));
		if (currentNodes.find (current.A->name) != currentNodes.end () && currentNodes.find (current.B->name) != currentNodes.end ())
				continue;
		else {
			if (currentNodes.find (current.A->name) == currentNodes.end ()) {
			    pair<string, Nodo *> p(current.A->name, current.A);
				currentNodes.insert (p);
			}
			if (currentNodes.find (current.B->name) == currentNodes.end ())	{
			    pair<string, Nodo *> p(current.B->name, current.B);
				currentNodes.insert (p);
			}
			minSpan.aristas.push_back (current);
		}
	}
	minSpan.nodos = g.nodos;
	return minSpan;
}

void generarGrafo (Grafo g) {
	ofstream ofile;
	ofile.open ("graph.gv", ios::trunc);
	ofile << "graph Cluster {" << endl;
	for (int i = 0; i < g.aristas.size (); i++) {
		ofile << g.aristas[i].A->name << " -- " << g.aristas[i].B->name << ";" << endl;
	}
	ofile << "}" << endl;
	ofile.close ();
}

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
    std::string path("/home/mauricio/Documents/2020-2/EDA/Fibonnacci_Heap/faces94");
    std::string ext(".jpg");
	int count = 0;
    for (auto &p : fs::recursive_directory_iterator(path))
    {   if (count > 10 )
			break;
        if (p.path().extension() == ext){
			count++;
            std::cout << p.path().stem ().string() << " : " << count << '\n';
	    vecN.push_back(new Nodo(p.path().string(), p.path().stem().string()) );
		}
    }
    int i,j;
    for (i=0; i< vecN.size()-1; i++){
		for(j=1; j< vecN.size(); j++){
	    	Arista AR(vecN[i], vecN[j]);
	    	vecA.push_back(AR);
		}
    }
	Grafo g;
	g.nodos = vecN;
	g.aristas = vecA;
	Grafo minSpan = kruskal (g);
	generarGrafo (minSpan);
    //p.path().stem().string()
    return 0;
}
