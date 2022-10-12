// algorithm standard header

#ifndef ALGORITHM
#define ALGORITHM
template<typename Ty>
inline void swap(Ty &a,Ty &b) {
    Ty temp = a;
    a = b;
    b = temp;
}
namespace MyNamespace
{
   template< typename RanIt, typename Ty, typename Pr >
   inline void pushHeapByIndex( RanIt first, ptrdiff_t hole, ptrdiff_t top, Ty &val, Pr pred )
   {
       while (hole != top) {
           ptrdiff_t idx = (hole-1) / 2;
           if (pred(*(first + idx) ,*(first + hole)  ) ) {
               swap(*(first + idx), *(first + hole));
           }
           hole=(hole-1) / 2;
       }
   }

   // push *(last - 1) onto heap at [first, last - 1), using pred
   template< typename RanIt, typename Pr >
   inline void push_heap( RanIt first, RanIt last, Pr pred )
   {
      ptrdiff_t count = last - first;
      if( count >= 2 )
      {
         --last;
         typename RanIt::value_type val = *last;
         --count;
         pushHeapByIndex( first, count, 0, val, pred );
      }
   }

   template< typename RanIt, typename Ty, typename Pr >
   inline void popHeapHoleByIndex( RanIt first, ptrdiff_t hole, ptrdiff_t bottom, Ty &val, Pr pred )
   {
       ptrdiff_t idx = hole * 2 + 1;
       *(first+ hole) = val;
       while (idx < bottom) {
           if(idx == bottom - 1) {
               if ( pred(val,*(first+idx)) ) swap(*(first + idx), *(first + hole));
           }
           else {
               if (pred(val, *(first + idx)) || pred(val, *(first + idx + 1))) {
                   if (!pred(*(first + idx + 1), *(first + idx))) idx += 1; 
                   swap(*(first + idx), *(first + hole));
               }
           }
           hole = idx;
           idx = hole * 2 + 1;
       }
   }

   // pop *first to *(last - 1) and reheap, using pred
   template< typename RanIt, typename Pr >
   inline void pop_heap( RanIt first, RanIt last, Pr pred )
   {
      if( last - first >= 2 )
      {
         --last;
         typename RanIt::value_type val = *last;
         popHeapHoleByIndex( first, 0, last - first, val, pred );
      }
   }

   // make [first, last) into a heap, using pred
   template< typename RanIt, typename Pr>
   inline void make_heap( RanIt first, RanIt last, Pr pred)
   {
      ptrdiff_t bottom = last - first;
      ptrdiff_t hole = bottom / 2;
      while( hole > 0 )
      {
         // reheap top half, bottom to top
         --hole;
         typename RanIt::value_type val = *( first + hole );
         popHeapHoleByIndex( first, hole, bottom, val, pred );
      }
   }
}

#endif // ALGORITHM