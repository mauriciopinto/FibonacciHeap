#include "FibonacciHeap.h"

int main () {
		BinomialHeap<int> z;

	  BNode<int> *a = new BNode<int>(10);
	  BNode<int> *b = new BNode<int>(2);
	  BNode<int> *c = new BNode<int>(3);
	  BNode<int> *d = new BNode<int>(1);
  	BNode<int> *e = new BNode<int>(5);
  	BNode<int> *f = new BNode<int>(4);
  	BNode<int> *g = new BNode<int>(6);
    
	  BNode<int> *h = new BNode<int>(7);
  	BNode<int> *i = new BNode<int>(0);
  	BNode<int> *j = new BNode<int>(9);
  	BNode<int> *k = new BNode<int>(22);

    //

  	z.Insert(a);
  	z.Insert(b);
  	z.Insert(c);
  	z.Insert(d);
  	z.Insert(e);
  	z.Insert(f);
  	z.Insert(g);
  	z.Insert(h);
  	z.Insert(i);
  	z.Insert(j);
  	z.Insert(k);
  	z.Print();
    z.DeleteMin ();
    z.Print();
    z.DecreaseKey (h, 0);
    z.DecreaseKey(k, -1);
    z.Print();
    //z.Insert(d);
   // z.DeleteMin();
    //z.Print();
   
	
	z.GenerateGraph ();		
}
