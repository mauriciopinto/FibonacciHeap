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
class Set;

struct Grafo {
	vector<Nodo*> nodos;
	vector<Arista> aristas;
};

class Nodo{
public:
    vector<float> vc;
    char *nombre; //large
    string name; //corto
	Set *tree;

    vector<float>  Vectorizar(CImg<float> & img ){
	     vector<float> R;
	     cimg_forXY(img,x,y){
		  R.push_back( (img(x,y,0) + img(x,y,1) +  img(x,y,2))/3);
	      }
	     return R;
	}
	Nodo () {};
    Nodo(string nom, string ruta){
	nombre = new char[nom.size ()];
	strcpy(this->nombre, nom.c_str());
	CImg<float>   A(nombre);
	A.resize(512, 512);
	CImg<float>   B =  A.haar(false,2);
	CImg<float>   c  = B.crop(0,0,127,127);
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
	Arista () {};
    Arista(Nodo* n1 , Nodo* n2){
	A=n1;
	B= n2;
	distancia= DistanciaEuclidiana(A,B);
    }
    float DistanciaEuclidiana(Nodo* v1, Nodo* v2){
		float total = 0;

		for(int i=0; i< v1->vc.size(); ++i){
		    total += pow( (v1->vc)[i]-(v2->vc)[i] , 2);
		}

		total = sqrt(total);
		return total;
    }
	
	float DistanciaManhattan(Nodo* v1, Nodo* v2){
		float total = 0;

		for(int i=0; i< v1->vc.size(); ++i){
	    	total += abs((v1->vc)[i]-(v2->vc)[i]);
		}
		return total;
    }

	float DistanciaCoseno(Nodo* v1, Nodo* v2) {
		float totalFact1 = 0;
		float totalFact2 = 0;
		float totalFact3 = 0;
		for (int i = 0; i < v1->vc.size (); i++) {
			totalFact1 += v1->vc[i] * v2->vc[i];
			totalFact2 += pow (v1->vc[i], 2);
			totalFact3 += pow (v2->vc[i], 2);
		}

		return 1 - (totalFact1 / (sqrt (totalFact2) * sqrt (totalFact3)));
	}
};

class Set {
public:
		unordered_map<string, Nodo *> elements;
		Set () {};
		Set (string name, Nodo *n) {
			pair<string, Nodo*> p(name, n);
			elements.insert (p);	
		}

		Set *merge (Set *s) {
			for (auto it = s->elements.begin (); it != s->elements.end (); it++) {
				pair<string, Nodo *> p(it->first, it->second);
				it->second->tree = this;
				elements.insert (p);
			}
			s->elements.clear ();
			return this;
		}
};

Grafo kruskal (Grafo g) {
	Grafo minSpan;
	FibonacciHeap<double> q;
	vector<Arista> aristas = g.aristas;
	
	vector<Set *> sets;
	for (int i = 0; i < g.nodos.size (); i++) {
		g.nodos[i]->tree = new Set(g.nodos[i]->name, g.nodos[i]);
		sets.push_back (g.nodos[i]->tree);
	}

	for (int i = 0; i < aristas.size (); i++) {
		BNode<double> *temp = new BNode<double> (aristas[i].distancia, &aristas[i]);	
		q.Insert (temp);
	}
	
	while (!q.Empty ()) {
		Arista current = * ((Arista *) (q.DeleteMin ()->structure));
			if (current.A->tree == current.B->tree)
				continue;
			
			current.A->tree->merge (current.B->tree);
			minSpan.aristas.push_back (current);
		}
	minSpan.nodos = g.nodos;
	return minSpan;
}

void generarGrafo (Grafo g, string filename) {
	ofstream ofile;
	ofile.open (filename, ios::trunc);
	ofile << "graph Cluster {" << endl;
	for (int i = 0; i < g.nodos.size (); i++) {
		ofile << "\"" << g.nodos[i]->name << "\"" << "[image=\"" << g.nodos[i]->nombre << "\", label=\"\"];" << endl;
	}

	for (int i = 0; i < g.aristas.size () - 7; i++) {
		ofile << "\"" << g.aristas[i].A->name << "\"" << " -- " << "\"" << g.aristas[i].B->name << "\"" << "[ label= " << g.aristas[i].distancia << "];" << endl;
	}

	ofile << "}" << endl;
	ofile.close ();
}

int main()
{
    vector<Nodo*> vecN;
    vector<Arista> vecA;
    std::string path("/home/mauricio/Documents/2020-2/EDA/Fibonnacci_Heap/faces94");
    std::string ext(".jpg");
	int count = 0;
    for (auto &p : fs::recursive_directory_iterator(path))
    {   if (count > 149)
			break;
        if (p.path().extension() == ext){
			count++;
            std::cout << p.path().stem ().string() << " : " << count << '\n';
		Nodo *n = new Nodo(p.path().string(), p.path().stem().string()); 
	    vecN.push_back(n);
		cout << "nodo: " << n->name << endl; 
		}
    }
    int i,j;
    for (i=0; i< vecN.size(); i++){
		for(j=i; j< vecN.size(); j++){
			if (i == j)
					continue;
	    	Arista AR(vecN[i], vecN[j]);
	    	vecA.push_back(AR);
			cout << "arista: " << AR.A->name << "--" << AR.B->name << " peso: " << AR.distancia << endl;
		}
    }
	Grafo g;
	g.nodos = vecN;
	g.aristas = vecA;
	generarGrafo (g, "graph.gv");
	Grafo minSpan = kruskal (g);
	generarGrafo (minSpan, "kruskal.gv");	
    return 0;
}
