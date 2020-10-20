#include "BNode.h"
#include <stdlib.h>
#include <bits/stdc++.h>
#include <math.h> 
#include <algorithm>

template<typename T>
bool ValueCmp (BNode<T> * const a, BNode<T> *const b);

template<typename T>
class BinomialHeap {
private:
	int size {0};
	list<BNode<T> *> heap_list;
	BNode<T> *min = nullptr;
  unordered_map<int, BNode<T> *> grados;
public:
	BinomialHeap () {};

	void Insert (BNode<T> *temp) {
		if (!min || min->key >= temp->key)
			min = temp;

		heap_list.push_front (temp);
    
		size++;
    
  
		//Compress ();
    


  }
	
	T GetMin () {
		return min->key;
	}
public:


	void DeleteMin () {
    /*
    cout << "a" <<endl;
    min->Print();
    cout << "\na" <<endl;
    */
    
		heap_list.remove (min);
		auto aux = min->children;
		size--;
		for (auto it : aux){
      it->parent = nullptr;
			Insert (it);
    }
    min->Reset();
		min = nullptr;
		T min_key = INT_MAX;
		Compress ();
    for (auto it : heap_list) {
			if ((it)->key < min_key) {
				min_key = (it)->key;
				min = it;
			}
		}
    
	}

  BNode<T> * Search(BNode<T> * node, T value){
    if(node->key == value){
      return node;
    }
    BNode<T> *temp;
    for(auto it:node->children){
      temp = Search(it, value);
      if (temp)
        return temp;
    }
    return nullptr;
  }


  BNode<T> * Find(T value){
      for(auto node : heap_list){
        auto ret = Search(node, value);
        if(ret){
          //cout << "ret" << endl;
          return ret;
        }
      }

      return nullptr;
  }

	void DecreaseKey (BNode<T> *node, T value) {

    //paso 1
      node->key = value;

    //CASE 1) todo sale bien
    if(node->parent.key < value){
      if(min->key > value){
        min = node;
      }
    }else{
      //CASE 2) no cumple propiedad y padre no está marcado
      if(!node->parent.black){

          node->parent.black = true;
          
          auto actual = node;
          auto aux = node->parent;
          while(aux->parent != nullptr){
              //recorremos hasta el root del arbol del parent
              aux->grado -= actual->grado;

              aux = aux->parent;
              actual = actual->parent;

          }

          node->parent = nullptr;


      }





    }




	}
private:
	void Merge (BinomialHeap<T> H) {
		for (auto it : H.heap_list) 
			Insert (*it);
	}
	
	BNode<T> *Union (BNode<T> *node1, BNode<T> *node2) {
		BNode<T> *new_root;
    if (node1->grado != node2->grado)
			return nullptr;
		if (node1->key < node2->key) {
			node1->children.push_back (node2);
			node2->parent = node1;
			node1->grado++;
			heap_list.remove (node2);
      new_root = node1;
		}
		else {
			node2->children.push_back (node1);
			node1->parent = node2;
			node2->grado++;
			heap_list.remove (node1);
      new_root = node2;
		}
		size--;
    return new_root;
	}

	void Compress () {

    grados.clear();

		auto it = heap_list.begin ();
    auto it2 = it;

		while (it2 != heap_list.end ()) {
      it2 = next (it, 1);
			if (grados.find((*it)->grado) == grados.end()) {
        grados.insert({(*it)->grado, *it});
			}else{
        auto auxGrado = (*it)->grado;
        auto newArbol = Union(*it, grados[(*it)->grado]);
        grados.erase(auxGrado);

        while (grados.find (newArbol->grado) != grados.end ()) {
          int prev_grado = newArbol->grado;
          newArbol = Union (newArbol, grados[newArbol->grado]);
          grados.erase (prev_grado);
        }

        grados.insert({newArbol->grado, newArbol});
      }
			it = it2;
		}
	}
public:
	void Print () {
		for (auto it : heap_list) {
			cout << "node" << endl;
			it->Print ();
			cout << endl;
		}
    cout << "--------------------------" << endl;
	}

	void Delete (BNode<T> *node) {
		DecreaseKey (node, INT_MIN);
    DeleteMin ();
	}

	void GenerateGraph () {
		ofstream ofile;
		ofile.open ("graph.gv", ios::trunc);

		ofile << "digraph BinomialHeap {" << endl;
		ofile << "node [shape=circle];" << endl << "rankdir=LR;" << endl;
		for (auto it = heap_list.begin (); it != heap_list.end (); it++) {
			ofile << (*it)->key << " [color=red];" << endl;
			if (next (it, 1) != heap_list.end ())
				ofile << (*it)->key << " -> " << (*(next (it, 1)))->key << ";" << endl;
			(*it)->GenerateGraph (&ofile);
		}
		ofile << "}";
	}

	void InputData (string filename) {
		ifstream ifile;
		//Completar
	}
};

template<typename T>
bool ValueCmp (BNode<T> * const a, BNode<T> *const b) {
	return a->GetGrado () < b->GetGrado ();
}
