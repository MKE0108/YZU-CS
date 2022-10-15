// xhash internal header

#ifndef XHASH
#define XHASH

#include <list>
using std::list;

// CLASS TEMPLATE VectorVal
template< typename ValueType >
class VectorVal
{
public:
   using value_type = ValueType;
   using pointer    = value_type *;

   VectorVal()
      : myFirst(),
        myLast(),
        myEnd()
   {
   }

   pointer myFirst; // pointer to beginning of array
   pointer myLast;  // pointer to current end of sequence
   pointer myEnd;   // pointer to end of array
};

template< typename Iter >
struct HashVec
{
   using value_type = Iter;
   using size_type  = size_t;

   HashVec()
      : myData()
   {
   }

   // set the elements stored here to cells copies of val
   void assignGrow( const size_type cells, const value_type val )
   {
       if (myData.myFirst != nullptr)
           delete[] myData.myFirst;
       myData.myFirst= new value_type[cells]();
       myData.myEnd = myData.myFirst + cells;
       myData.myLast = myData.myFirst + cells;
       for (int i = 0; i < cells; i++) {
           myData.myFirst[i] = val;
       }
   }

   ~HashVec()
   {
      if( myData.myFirst != nullptr )
         delete[] myData.myFirst;
   }
   
   VectorVal< value_type > myData;

};

// CLASS TEMPLATE Hash
template< typename Traits >
class Hash // hash table -- list with vector of iterators for quick access
{
protected:
   using MyList         = list< typename Traits::value_type >;
   using KeyCompare     = typename Traits::key_compare;

public:
   using key_type       = typename Traits::key_type;
   using value_type     = typename MyList::value_type;
   using size_type      = typename MyList::size_type;

   using iterator       = typename MyList::const_iterator;
   using const_iterator = typename MyList::const_iterator;

   static const size_type MinBuckets = 8; // must be a positive power of 2

protected:
   Hash( const KeyCompare &parg ) // construct empty hash table
      : traitsObj( parg ),
        myList(),
        myVec(),
        mask( MinBuckets - 1 ),
        maxidx( MinBuckets )
   {     
      myVec.assignGrow( MinBuckets * 2, myList.end() );
 
   }

public:
   // Returns the number of elements in the unordered_set container.
   size_type size() const
   {
      return myList.size();
   }

   // Returns the number of buckets in the unordered_set container.
   size_type bucket_count() const
   {
      return maxidx;
   }

   // Returns the bucket number where the element with value keyVal is located.
   size_type bucket( const key_type& keyVal ) const
   {
      return traitsObj( keyVal ) & mask;
//      return traitsObj.operator()( keyVal ) & mask;
//      return traitsObj( keyVal ) % maxidx;
//      return traitsObj.operator()( keyVal ) % maxidx;
   }

   // Returns the number of elements in bucket n.
   size_type bucket_size( size_type n ) const
   {
       if (n >= maxidx) {
           return 0;
       }
       int s = 0;
       iterator  begin = myVec.myData.myFirst[(n << 1)];
       iterator  end = myVec.myData.myFirst[(n << 1) + 1];
       if (begin == myList.end()) {
           return 0;
       }
       end++;
       for (auto it = begin; it != end; it++) {
           s++;
       }
       return s;
   }
   // Inserts a new element in the unordered_set.
   // The element is inserted only if it is not equivalent to any other element
   // already in the container ( elements in an unordered_set have unique values ).
   // This effectively increases the container size by one.
   void insert( const value_type &val )
   {
       if (find(val) == myList.end())
       {
           if (myList.size() == maxidx)
           {
               if (maxidx == 8 || maxidx == 64)
                   maxidx <<= 3;
               else
                   maxidx <<= 1;

               myVec.assignGrow(maxidx<<1, myList.end());
               mask = maxidx - 1;
               for (auto it : myList) {
                   temp.push_back(it);
               }
               myList.clear();
               for (auto it : temp) {
                   putIn(it);
               }
           }

           putIn(val);
       }
   }

   // Removes from the unordered_set a single element.
   // This effectively reduces the container size by one.
   void erase( const key_type &keyVal )
   {
       int n = bucket(keyVal);
       auto _48763 = find(keyVal);
       if (_48763 == myList.end()) {
  /*
  =========================================================
  | ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⠄⠀⠛
  |  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣋⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠐⢦⣀
  |   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠋⣤⣶⣶⣄⠙⣿⣷⢸⡀⠀⠀⠀⠀⠀⠀⠉⠉⠙⠛⢿⣶
  |   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠁⣿⣿⠉⠉⢻⣷⣿⣿⣾⣿⠀⠀⠀⠀⠀⢠⣿⠿⢿⣿⣦⠘⣷
  |   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⢸⣿⣿⠀⠀⠀⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀⣿⠏⠀⠀⣿⣿⣷⠘
  |   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣄⣀⣾⣿⣿⣿⠿⣿⣿⣧⠀⠀⠀⣿⡀⠀⠀⣿⣿⣿
  |   ⠀⠀⠀⠀⠀⠀⠀⠀⣆⢸⣿⣿⣯⣭⣿⣵⣾⣿⠃⠀⣿⣿⣿⡆⡀⠀⠹⢿⣶⣿⣿⣿⣣⣿
  |   ⠀⠀⠀⠀⠀⠀⠀⠀⢸⡀⣿⣿⣿⣿⣿⣿⣿⣿⠀⣾⣿⣿⣿⣿⣷⢰⣄⠀⠀⠀⠚⠛⢿⡇
  |   ⠀⠀⠀⠀⠀⠀⠀⠈⣌⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣿⣿⣿⣿⣿⣿⣾⣿⣿⣶⣶⣾⣿⡿
  |   ⠀⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⣴⠁⠀⠀⠀⠀⠀⡀
  |   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠀⠀⠀⠀⠀⣴⠃
  |   ⠀⢠⣶⡀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠀⣀⣠⣤⠀⠞
  |   ⠀⣿⡿⣷⠀⠀⠀⠀⠀⠸⣿⣿⣿⣿⣿⣿⣏⣈⠙⠛⠻⢿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⠋
  |   ⣸⣿⠀⠈⢿⣄⠀⠀⠀⠀⠙⣿⣿⣿⣿⣟⣛⣿⢿⣻⠿⣒⣺⣿⣿⣿⣿⣿⣿⣿⡿⠁⠀⠀⠀⠀⠀⢀
  |   ⣿⡟⠀⠀⠀⠙⢿⣦⣄⡀⠀⠈⢿⣿⣿⣿⣛⣿⣿⣿⣠⣽⣾⣿⣿⣿⣿⣿⣿⠋⠀⠀⠀⠀⠀⠀⢠⣿⡆
  |
  |                       是在哭嗎?
  =========================================================
  */
       }
       else {
           if (bucket_size(n) == 1) {
               myVec.myData.myFirst[(n << 1)] = myList.end();
               myVec.myData.myFirst[(n << 1) + 1] = myList.end();
           }
           if (_48763 == myVec.myData.myFirst[n << 1]) {
               ++myVec.myData.myFirst[(n << 1)];
           }
           if (_48763 == myVec.myData.myFirst[(n << 1)+1]) {
               --myVec.myData.myFirst[(n << 1) + 1];
           }
           
           
           
           myList.erase(_48763);
       }

   }

   // Searches the unordered_set for an element with keyVal as value and
   // returns an iterator to it if found, otherwise it returns myList.end()
   iterator find( const key_type &keyVal )
   {
       int n = bucket(keyVal);
       auto begin = myVec.myData.myFirst[(n << 1)];
       auto end= myVec.myData.myFirst[(n << 1) + 1];
       if (begin == myList.end()) {
           return myList.end();
       }
       end++;
       for (auto it = begin; it != end; it++) {
           if (*it == keyVal) {
               return it;
           }
       }
       return myList.end();
   }

private:
   // put a new element in the unordered_set when myVec is large enough
   void putIn( const value_type &val )
   {
       int n = bucket(val);
       auto begin = myVec.myData.myFirst[n << 1];
       if (begin == myList.end()) {
           myList.push_back(val);
           auto temp = myList.end();
           temp--;
           myVec.myData.myFirst[n << 1] = temp;
           myVec.myData.myFirst[(n << 1) + 1] = temp;
       }
       else {
            myList.insert(begin, val);
           myVec.myData.myFirst[n << 1] = --begin;
       }
   }

protected:
   Traits traitsObj;          // traits to customize behavior
   MyList myList;             // list of elements, must initialize before myVec
   HashVec< iterator > myVec; // "vector" of list iterators for buckets:
                              // each bucket is 2 iterators denoting
                              // the closed range of elements in the bucket,
                              // or both iterators set to end() if the bucket is empty.
   size_type mask;            // the key mask, equals maxidx - 1
   size_type maxidx;          // current maximum key value, must be a power of 2
};

#endif // XHASH