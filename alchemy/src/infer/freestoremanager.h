#ifndef _FREE_STORE_MANAGER_
#define _FREE_STORE_MANAGER_

#include <iostream>

using namespace std;

class FreeStoreManager
{
  public:
	 
   FreeStoreManager(int blockSize)
   {
     storePtrList_ = NULL;
     maxCount_ = 0;
	 allocatedCount_ = 0;
	 occupiedCount_ = 0;
	 this->blockSize_ = blockSize;
   }

   ~FreeStoreManager()
   {
	 if(storePtrList_ == NULL)
	   return;
	 for(int i=0;i<allocatedCount_;i++)
	   delete [] storePtrList_[i];
	 delete [] storePtrList_;
   }
	  
	 /* get the block size */
   int getBlockSize(){ return blockSize_; }

	 /* get next available store ptr */	  
   int* getStorePtr() {
	 //	cout<<"came to get a new block.."<<endl;
     int *storePtr;
	 if(allocatedCount_ == maxCount_)
	   increaseMaxCount();
	 if(allocatedCount_ == occupiedCount_)
	 {
	   storePtr = new int[blockSize_];
	   storePtrList_[allocatedCount_++] = storePtr;
	 }
     return storePtrList_[occupiedCount_++];
   }

   /* release a store ptr */
   void releaseStorePtr()
   {
	 if(occupiedCount_ == 0)
	   cout<<"Problem in freeStorePtr!!"<<endl<<"Nothing to free!"<<endl;
	 occupiedCount_--;
   }

   /* release a store ptr */
   void releaseAllStorePtr()
   {
	 //	cout<<"releasing all the blocks.."<<endl;
	 occupiedCount_=0;
   }
	
  private:
	
   /* increase the max size */
   void increaseMaxCount()
   {
	 maxCount_ = maxCount_*2+1;
	 int **tmpPtrList = new int*[maxCount_];
	 if(storePtrList_ != NULL)
	 {
	   for(int i=0;i<allocatedCount_;i++)
     	 tmpPtrList[i] = storePtrList_[i];
	   delete storePtrList_;
	 }
	 storePtrList_ = tmpPtrList;
   }

  private:
	
   int maxCount_;
   int allocatedCount_;
   int occupiedCount_;
   int **storePtrList_;
   int blockSize_;
};


#endif
