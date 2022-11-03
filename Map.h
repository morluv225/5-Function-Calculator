// Map.h           CPTG 244
//     author's Map ADT Interface from code fragment 9.2, 9.4
//     based on an underlying doubly-linked list implementation

#ifndef MAP_H
#define MAP_H

//#include "RuntimeException.h"
#include <string>  // for RuntimeException
#include <iostream>
#include <sstream>           // to use ostringstream
#include <list>
using namespace std;

class RuntimeException {
 private:
  string errorMsg;
 public:
  RuntimeException(const string& err)
    {
      errorMsg = err;
    }

  string getMessage() const 
  {
    return errorMsg;
  }
};

class NonexistentElement : public RuntimeException {
 public:
  NonexistentElement(const string& err) : RuntimeException(err)
    {
      // err is passed to the RuntimeException constructor
    }
};

template <typename K, typename V>
class Map {
public:

  class Entry {
  public:
    // constructor (default) ... calls K() and V() constructors as default
    Entry(const K& k = K(), const V& v = V()) 
      : _key(k), _value(v)
      {  }         // all done in : section above

      const K& key() const   { return _key; }
      const V& value() const { return _value; }
      
      void setKey(const K& k)   { _key = k; }
      void setValue(const V& v) { _value = v; }
	  friend class Map<K, V>;
      
  private:
      K _key;
      V _value;
  }; // end of Entry class

private:
  struct Node {
    typename Map<K,V>::Entry entry;
                       Node* prev;
                       Node* next;
  };

public:
  class Iterator {
  public:
    Entry& operator*();
    bool operator==(const Map<K,V>::Iterator& p) const;
    bool operator!=(const Map<K,V>::Iterator& p) const;
    Iterator& operator++(); // prefix
    Iterator& operator--(); // prefix
    Iterator& operator=(const Map<K,V>::Iterator& p);
    friend class Map<K,V>;
  private:
    Node* v;
    Iterator(Node* u);  // constructor
  };
  
  // ------ rest of Map functions -------

  // --- constructor
  Map();  // default ... see housekeeping functions for copy constructor

  // --- accessor functions
  int size() const;
  bool empty() const;
  //  Iterator find(const K& k) const; // would need a const_Iterator here
  Iterator find(const K& k) ;
  Iterator begin();
  Iterator end();

  // --- mutator functions
  Iterator put(const K& k, const V& v);
  void erase (const K& k)
    throw (NonexistentElement);
  void erase (const Iterator& p);

  // --- housekeeping functions
  ~Map();                                // destructor
  Map<K,V>& operator=(Map<K,V>& other);  // asignment operator
  Map(Map<K,V>& other);                  // copy constructor
  void print();                          // only for debugging purposes
 private:
  int n;
  Node* header;  // sentinel
  Node* trailer; // sentinel
}; // end of Map class

//--------------------------------------------------------
// Implementation of Map<K,V>::Iterator
//--------------------------------------------------------

template<typename K, typename V>
Map<K,V>::Iterator::Iterator(Node* u) // constructor
{
  v = u;  // assumes u is valid
}

template<typename K, typename V>
typename Map<K,V>::Entry& Map<K,V>::Iterator::operator*() // reference to the element
{
  return v->entry;  // assumes v is valid
}

template<typename K, typename V>
bool Map<K,V>::Iterator::operator==(const Map<K,V>::Iterator& p) const // compare positions
{
  return v == p.v; // could do (*this.v == p.v)
}

template<typename K, typename V>
bool Map<K,V>::Iterator::operator!=(const Map<K,V>::Iterator& p) const
{
  return v != p.v;
}

template<typename K, typename V>
typename Map<K,V>::Iterator& 
Map<K,V>::Iterator::operator=(const Map<K,V>::Iterator& p)
{
  v = p.v;
  return *this;
}

template<typename K, typename V>
typename Map<K,V>::Iterator& Map<K,V>::Iterator::operator++() // prefix move to next position
{
  v = v->next;
  return *this;
}

template<typename K, typename V>
typename Map<K,V>::Iterator& Map<K,V>::Iterator::operator--() // move to prev position
{
  v = v->prev; return *this;
}

//--------------------------------------------------------
// Implementation of Map<K,V>
//--------------------------------------------------------

template<typename K, typename V>
Map<K,V>::Map() // constructor
{
  // list is initially empty
  n = 0;

  // dynamically allocate the sentinels
  header = new Node;
  trailer = new Node;

  // make them pointer to each other
  header->next = trailer;
  header->prev = NULL;
  trailer->prev = header;
  trailer->next = NULL;
}

template<typename K, typename V>
int Map<K,V>::size() const // return size of list
{
  return n;
}

template<typename K, typename V>
bool Map<K,V>::empty() const // is list empty?
{
  return (n == 0);
}
  
template<typename K, typename V>
typename Map<K,V>::Iterator 
Map<K,V>::begin() // beginning position of first item
{
  return typename Map<K,V>::Iterator(header->next);
}

template<typename K, typename V>
typename Map<K,V>::Iterator 
Map<K,V>::end()  // ending position just beyond last item
{
  return typename Map<K,V>::Iterator(trailer);
}


//----------- find -------------------------
template<typename K, typename V>
typename Map<K,V>::Iterator 
  Map<K,V>::find(const K& k) //const
{
  typename Map<K,V>::Iterator iter = begin();
  
  while (iter != end() && ((*iter).key() != k) ) {
    ++iter;
  }
   
  return iter;  // either iter points to an Entry with k as key, or end()
}

//------------- put --------------------------
template<typename K, typename V>
typename Map<K,V>::Iterator 
Map<K,V>::put(const K& k, const V& v)
{
  typename Map<K,V>::Iterator iter = find(k);
  
  // if we found an entry with key k:
  if ( iter != end()) {
  
    // then an Entry exists with a key value of k
    //      so change it's value to v

	(*iter).setValue(v);
	// iter->setValue(v);

	return iter;  // return the iterator to the modified entry
  }

  // else no Entry exists with a key value of k, so just insert it
  
  Node * q =  new Node;
  q->entry.setKey(k);                                          // get a new node
  q->entry.setValue(v);
  
  
  // insert at tail, where p -> r
  //                       p <- r
  Node * r = trailer;
  
  Node * p = trailer->prev;

  // making       p -> q -> r
  //              p <- q <- r
  q->next = r;
  q->prev = p;
  p->next = q;
  r->prev = q;

  n++; // increment number of nodes
  
  return Iterator(q);                             // return iterator of new node
  // return --end();
}







//----------------------- erase(k) ---------------------
template<typename K, typename V>
  void Map<K,V>::erase (const K& k)
    throw (NonexistentElement)
{
  typename Map<K,V>::Iterator iter = find(k);
  
  // if not found .........................
  if ( iter == end() ) { 

	throw NonexistentElement("Error:  key not found in erase()");

  }

  // else if found, iter has Iterator
  erase(iter);

}

//----------------------- erase(p) ---------------------
// remove elemen p from map
template<typename K, typename V>
void Map<K,V>::erase(const Map<K,V>::Iterator& p)
{
  Node* v = p.v;  // node to remove
  Node* w = v->next;
  Node* u = v->prev; // currently, u -> v -> w
                     // want,        u -> w
  // make u and w point around v
  u->next = w;
  w->prev = u;

  // recycle v
  delete v;

  // decrease size of list by 1
  n--;
}

// housekeeping functions of Map<K,V>
//------------------------------------

// Destructor-----------
template<typename K, typename V>
Map<K,V>::~Map<K,V>() {
  Node* nPtr;
  
  // recycle list nodes
  nPtr = header->next;
  while (nPtr != trailer) {
    Node* ptr = nPtr;
    nPtr = nPtr->next;
    delete ptr;
  }

  // recycle sentinels
  delete header;
  delete trailer;
}

// Assignment operator ---------------------
template<typename K, typename V>
Map<K,V>&  Map<K,V>::operator=(Map<K,V>& other) {
  
  if (size() > 0) {
    // recycle nodes in current Map (*this)

    Node* nPtr;
  
  	// recycle list nodes
  	nPtr = header->next;
 	while (nPtr != trailer) {
  	  Node* ptr = nPtr;
      nPtr = nPtr->next;
      delete ptr;
    }
	header->next = trailer;
	trailer->prev = header;
	n = 0;

  }

  // copy nodes from other to (*this)

  typename Map<K,V>::Iterator iter = other.begin();
  while (iter != other.end()) {
  
  	put ((*iter).key(), (*iter).value());  // O(n) ... should do this manually
  	
  	++iter;
  }

  return (*this);   // to allow A = B = D; assignment statements for Maps
}
 
// copy constructor ----------------------------------   
template<typename K, typename V>
  Map<K,V>::Map(Map<K,V>& other) // copy constructor
{
  // list is initially empty
  n = 0;

  // dynamically allocate the sentinels
  header = new Node;
  trailer = new Node;

  // make them point to each other
  header->next = trailer;
  header->prev = NULL;
  trailer->prev = header;
  trailer->next = NULL;

  // copy over from other ... using assignment operator (above)
  *this = other;
}

// for debugging purposes only ... assumes K and V have overloaded << operator
template <typename K, typename V>
void Map<K,V>::print() {
  typename Map<K,V>::Iterator iter = begin();

  while (iter != end()) {
    cout << "\t<" << (*iter).key() << "," << (*iter).value() << ">\n";
    ++iter;
  }
}

#endif