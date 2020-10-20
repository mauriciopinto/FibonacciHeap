#include <list>
#include <iostream>
#include <fstream>

using namespace std;
template<typename T>
class BNode {
protected:
	int grado = 0;
	T key;
	BNode<T> *parent = nullptr;
  bool black = false;
private:
	list<BNode<T> *> children;
public:
	BNode (T k) : key {k} {};

	int GetGrado () {
		return grado;
	}

	void Print () {
		cout << key << " grado " << grado << " ";

		for (auto it : children) {
			cout << endl;
			it->Print ();
		}
	}

  void Reset(){
    grado = 0;
    parent = nullptr;
    children.clear();
  }
	void GenerateGraph (ofstream *ofile) {
		for (auto it = children.begin (); it != children.end (); it++) {
			*ofile << key << " -> " << (*it)->key << ";" << endl;
			(*it)->GenerateGraph (ofile);
		}
	}

	template<typename U> friend class BinomialHeap;
};
