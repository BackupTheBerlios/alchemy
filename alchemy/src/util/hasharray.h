#ifndef HASHARRAY_H_JUL_19_2005
#define HASHARRAY_H_JUL_19_2005

#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <string>
#include "random.h"
using namespace std;
#include <ext/hash_map>
using namespace __gnu_cxx;


  //An Array of that does not contain duplicates. It is backed up by a hash_set
template <typename Type, class HashFn, class EqualFn> 
class HashArray
{
 public:
  HashArray(const int& initSize=0) : items_(NULL), maxItems_(0), numItems_(0),
                                     map_(new hash_map<Type,int,HashFn,EqualFn>)
  { allocateMemory(initSize); }
    
  
  HashArray(const HashArray<Type, HashFn, EqualFn>& x)
  { 
    if (x.numItems_ == 0) 
    {
      items_    = NULL;
      maxItems_ = 0;
      numItems_ = 0;
      allocateMemory(0);
      map_ = new hash_map<Type, int, HashFn, EqualFn>;
      return;
    }

    items_ = new Type[x.numItems_];      
    //commented out: Type may not be a basic type and thus need deep copying
    //memcpy(items_, x.items_, x.numItems_*sizeof(Type));
    for (int i = 0; i < x.numItems_; i++) items_[i] = x.items_[i];
    numItems_ = x.numItems_;
    maxItems_ = numItems_;
    map_ = new hash_map<Type, int, HashFn, EqualFn>;
    for (int i = 0; i < numItems_; i++)
      (*map_)[items_[i]] = i;
    assert(map_->size() == (unsigned int) numItems_);
  }


    // does not delete each individual item
  ~HashArray() { if (items_) delete [] items_; delete map_; }


    // returns index of appended item or -1 if item is already in the array
  int append(Type item)
  {
    if (map_->find(item) != map_->end()) return -1;

    if (numItems_ == maxItems_) 
      allocateMemory(maxItems_*2+1);
    items_[numItems_++] = item;
    (*map_)[item] = numItems_-1;

    assert(map_->size() == (unsigned int) numItems_);
    return numItems_-1;
  }


  int append(Type item, const int& max)
  {
    if (map_->find(item) != map_->end()) return -1;

    if (numItems_ == maxItems_) 
    {
      int newNumItems = maxItems_*2+1;
      if (newNumItems > max) newNumItems = max;
      allocateMemory(newNumItems);
    }
    items_[numItems_++] = item;
    (*map_)[item] = numItems_-1;
    assert(map_->size() == (unsigned int) numItems_);
    return numItems_-1;
  }



    // returns the number of items appended
  int append(const HashArray<Type, HashFn, EqualFn>& newItems)
  {
    int n = 0;
    for (int i = 0; i < newItems.size(); i++)
      if (append(newItems[i]) >= 0) n++;
    return n;
  }


  int append(const HashArray<Type, HashFn, EqualFn>* const & items) 
  { return append(*items); }


    // the contents of the array are cleared (but not deleted), and populated
    // with those in items
    // returns the number of items copied
  int copyFrom(const HashArray<Type, HashFn, EqualFn>& items) 
  { 
    clear(); 
    return append(items); 
  }


  bool growToSize(const int& newSize)
  {
    if (newSize <= numItems_) return false;
    
    if (newSize <= maxItems_)
      numItems_ = newSize;
    else
    {
      allocateMemory(newSize);
      numItems_ = newSize;
    }
    return true;
  }


  bool growToSize(const int& newSize, Type fill)
  {
    int origNumItems = numItems_;
    if(!growToSize(newSize)) return false;
    for (int i = origNumItems; i < numItems_; i++)
      items_[i] = fill;
    return true;
  }


    // the contents in the array are not deleted
  void clear()  { numItems_ = 0; map_->clear(); }

    // the contents in the array are not deleted
  void clearAndCompress()  { numItems_ = 0; map_->clear(); compress(); }


    // delete the contents of items_ array and set numItems = 0;
    // items_ is not deleted,and maxItems_ stay the same
  void deleteItemsAndClear() 
  { 
    for (int i = 0; i < numItems_; i++)
      if (items_[i]) delete items_[i];
    clear();
    assert(map_->size() == (unsigned int) numItems_);
  }

    // delete the contents of items_ array and set numItems = 0;
    // items_ is not deleted,and maxItems_ stay the same
  void deleteItemsAndClearCompress() 
  { 
    for (int i = 0; i < numItems_; i++)
      if (items_[i]) delete items_[i];
    clear();
    assert(map_->size() == (unsigned int) numItems_);
    compress();
  }


  int size() const { return numItems_; }

  bool empty() const { return size()==0; } 

    // important for the return value to be const so that the array cannot
    // be written via [] (e.g. array[0] = anotherObject). This prevents
    // the indexes in map_ from being inconsistent.
  const Type& operator[](const int& index) const { return item(index); } 


  Type& lastItem() { return items_[numItems_-1]; }


    // caller should not delete the returned pointer nor modify the object
  const Type* getItems() const { return items_; }


  //comment out: randomOneOf no longer a static function
  //Type& randomItem() const 
  //{
  //  assert(numItems_>0); 
  //  return items_[Random::randomOneOf(numItems_)];
  //}
  

  int find(Type const & item) const
  {
    typename hash_map<Type, int, HashFn, EqualFn>::iterator it=map_->find(item);
    if (it == map_->end()) return -1;
    assert((*it).second >= 0);
    return (*it).second;
  }

  
  bool contains(Type const & item) const { return find(item) >= 0;}


  Type removeItem(const int& index)
  {
      // move everything past the item back
    Type removedItem = items_[index];
    map_->erase(map_->find(items_[index]));
    for (int i = index+1; i < numItems_; i++) items_[i-1] = items_[i];

    //commented out: Type may not be a basic type and thus need deep copying
    //int numItemsToMove = numItems_ - index - 1;
    //if (numItemsToMove > 0)
    //  memmove(&items_[index], &items_[index+1], numItemsToMove*sizeof(Type));

    numItems_--;
    
      // subtract the indexes of the moved items by 1
    typename hash_map<Type, int, HashFn, EqualFn>::iterator it;
    for (int i = index; i < numItems_; i++)
    {
      it = map_->find(items_[i]);
      assert(it != map_->end());
      assert((*it).second-1==i);
      (*it).second = i;
    }

    assert(map_->size() == (unsigned int)numItems_);
    return removedItem;
  }


  Type removeItem(Type& item)
  {
    int idx = find(item);
    return removeItem(idx);
  }
  

  Type removeLastItem() { return removeItem(numItems_-1); }


    // removes the item but doesn't leave the array in the same order as before
  Type removeItemFastDisorder(const int& index) 
  {
    Type removedItem = items_[index];
    map_->erase(map_->find(items_[index]));

    if (numItems_ > 1 && index != numItems_-1)
    {
      item(index) = item(numItems_-1);    
      typename hash_map<Type, int, HashFn, EqualFn>::iterator it;
      it = map_->find(item(index));
      assert((*it).second == numItems_-1);
      (*it).second = index;
    }

    numItems_--;
    assert(map_->size() == (unsigned int)numItems_);
    return removedItem;
  }


  Type removeItemFastDisorder(Type& item)
  {
    int idx = find(item);
    if (idx < 0) return NULL;
    return removeItemFastDisorder(idx);
  }


    // resizes the array to be exactly the number of elements in it
  void compress()  { if (maxItems_ > numItems_)  allocateMemory(numItems_); }


    // reorder randomly
  void shuffle() 
  {
    for (int i = 0; i < numItems_; i++) 
    {
        // swap item i with a random item
      int swapwith = Random::randomOneOf(numItems_);
      Type tmp;
      tmp = items_[i];
      items_[i] = items_[swapwith];
      items_[swapwith] = tmp;
    }
  }


    // returns the index of largest item
  int getMaxIndex() 
  {
    assert(numItems_ > 0);
    int max = 0;
    for (int i = 1; i < numItems_; i++) 
    {
      if (items_[i] > items_[max])
        max = i;
    }
    return max;
  }


    // returns the value of largest item
  Type getMaxValue() 
  {
    assert(numItems_ > 0);
    Type max = items_[0];
    for (int i = 1; i < numItems_; i++) 
    {
      if (items_[i] > max)
        max = items_[i];
    }
    return max;
  }


 private:
  Type& item(const int& index) const 
  {
    assert(index < numItems_);
    return items_[index];
  }
 
    // if newSize < numItems, numItems will be shrunk to newSize
  void allocateMemory(const int& newSize)
  {
    if (newSize > 0)
    {
      if (newSize >= numItems_)
      {
        Type* tempItems = new Type[newSize];
        if (numItems_ > 0)
        {
          //commented out: Type may not be a basic type & thus need deep copying
          //memcpy(tempItems, items_, numItems_*sizeof(Type));
          for (int i = 0; i < numItems_; i++) tempItems[i] = items_[i];
        }
        delete [] items_;
        items_ = tempItems;
        maxItems_ = newSize;
      }
      else  // newSize < numItems_
      {
        Type* tempItems = new Type[newSize];
        if (numItems_ > 0) 
        {
          //commented out: Type may not be a basic type & thus need deep copying
          //memcpy(tempItems, items_, newSize*sizeof(Type));
          for (int i = 0; i < newSize; i++) tempItems[i] = items_[i];
        }
        delete [] items_;
        items_ = tempItems;
        maxItems_ = newSize;
        numItems_ = newSize;
      }
    }
    else // newSize==0
    {
      delete [] items_;
      items_ = NULL;
      maxItems_ = 0;
      numItems_ = 0;
    }
  }

  
 private:
  Type* items_;
  int maxItems_;
  int numItems_;

  hash_map<Type, int, HashFn, EqualFn>* map_;
};


// Useful functions

  // between is the filler string to be used between items
template <typename T, class HashFn, class EqualFn> 
void writeHashArray(const HashArray<T*,HashFn,EqualFn>& array, ostream& out, 
                    char* between=NULL, const char& delimiter)
{
  out << array.size() << delimiter;
  for (int i = 0; i < array.size(); i++)
  {
    array.item(i)->save(out);
    if (between) out << between;
  }
}


template <typename T, class HashFn, class EqualFn>
void writeArray(HashArray<T*,HashFn,EqualFn>&  array, ostream& out, 
                char* between=NULL)
{ writeArray(array, out, between, '\n'); }


template <typename T, class HashFn, class EqualFn>
void readArray(HashArray<T*,HashFn,EqualFn>& array, istream& in)
{
  int numItems;
  in >> numItems;
  for (int i = 0; i < numItems && in.good(); i++)
  {
    T* x = new T;
    x->load(in);
    array.append(x);
  }
}


template <typename T, class HashFn, class EqualFn>
void writeArray(HashArray<T,HashFn,EqualFn>& array, ostream& out, 
                char* between=NULL, const char& delimiter)
{
  out << array.Size() << delimiter;
  for (int i = 0; i < array.Size(); i++)  
  {
    array.item(i).save(out);
    if (between) out << between;
  }  
}


template <typename T, class HashFn, class EqualFn>
void writeArray(HashArray<T,HashFn,EqualFn>& array, ostream& out, 
                char* between=NULL)
{ writeArray(array, out, between, '\n'); }


template <typename T, class HashFn, class EqualFn>
void readArray(HashArray<T,HashFn,EqualFn>& array, istream& in) 
{
  int numItems;
  in >> numItems;
  for (int i = 0; i < numItems && in.good(); i++) 
  {
    T x;
    x.load(in);
    array.append(x);
  }
}


template <typename T, class HashFn, class EqualFn>
void writeBasicArray(HashArray<T,HashFn,EqualFn>& array, ostream& out, 
                     char* between=NULL, const char& delimiter)
{
  out << array.Size() << delimiter;
  for (int i = 0; i < array.Size(); i++)
  {
    out << array.item(i) << " ";
    if (between) out << between;
  }
}


template <typename T, class HashFn, class EqualFn>
void writeBasicArray(HashArray<T,HashFn,EqualFn>& array, ostream& out, 
                     char* between=NULL)
{ writeBasicArray(array, out, between, '\n'); }


template <typename T, class HashFn, class EqualFn>
void readBasicArray(HashArray<T,HashFn,EqualFn>& array, istream& in)
{
  int numItems=0;
  in >> numItems;
  for (int i = 0; i < numItems && in.good(); i++)
  {
    T x;
    in >> x;
    array.append(x);
  }
  if (!in.good())
    cout << "ERROR: readBasicArray(). Input not good." << endl;
}


#endif
