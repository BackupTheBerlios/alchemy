#ifndef STRFIFOLIST_H_JUN_26_2005
#define STRFIFOLIST_H_JUN_26_2005

#include <list>
#include <ostream>
using namespace std;


  // Stores the tokens returned by follex. The first token is the latest one.
class StrFifoList
{
 public: 
  StrFifoList() { fifoList_ = new list<const char*>; }

  ~StrFifoList() 
  {
    list<const char*>::iterator it = fifoList_->begin();
    for (; it != fifoList_->end(); it++)  delete [] (*it);
    delete fifoList_; 
  }

  
  void deleteContentsAndClear()
  {
    list<const char*>::iterator it = fifoList_->begin();
    for (; it != fifoList_->end(); it++)  delete [] (*it);
    fifoList_->clear();
  }


    // Caller is responsible for deleting str.
  void add(const char* const & str)
  {
    //commented out: don't impose a max
    //if (fifoList_->size() == STRFIFOLIST_MAXSIZE) fifoList_->pop_back();
    char* s = new char[strlen(str)+1];
    strcpy(s,str);
    fifoList_->push_front(s);
  }


  void addLast(const char* const & str)
  {
    //commented out: don't impose a max
    //if (fifoList_->size() == STRFIFOLIST_MAXSIZE) fifoList_->pop_back();
    char* s = new char[strlen(str)+1];
    strcpy(s,str);
    fifoList_->push_back(s);
  }


    // Caller should not delete returned const char*
  const char* getItem(const int& idx)
  {
    if ((unsigned int)idx > fifoList_->size()-1) return NULL;
    list<const char*>::iterator it = fifoList_->begin();
    int i = 0;
    for (; it != fifoList_->end(); it++)
      if (i++ == idx) return (*it);
    return NULL;
  }
  

  unsigned int getSize() const { return fifoList_->size(); }

    // the caller should not modify or delete the returned pointer
  const char* back() const { return fifoList_->back(); }

    // caller is responsible to deleting returned pointer
  const char* removeLast() 
  {
    const char* last = fifoList_->back();
    fifoList_->pop_back();
    return last;
  }


  ostream& print(ostream& out) const 
  {
    list<const char*>::iterator it = fifoList_->begin();
    for (; it != fifoList_->end(); it++)
      out << (*it) << " ";
    return out;
  } 
  
 private:
  list<const char*>* fifoList_;
  
};


inline
ostream& operator<<(ostream& out, const StrFifoList& l) { return l.print(out); }


#endif
