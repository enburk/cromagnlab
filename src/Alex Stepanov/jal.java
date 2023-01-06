/**
  * Matthew Austern and Alexander Stepanov: 
  * jal (Java Algorithm Library),
  * Silicon Graphics, Incorporated, 1996
  */
  
/* {{{ *//* }}} */

/**
* Applies a function to every element of a range.
* @param array    Array containing the range.
* @param first    Beginning of the range.
* @param last     One past the end of the range.
* @param f        Function to be applied.
*/
public static void for_each(generic[] array, int first, int last,
                                                  VoidFunction f)
{
  while(first < last)
        f.apply(array[first++]);
}

/* {{{ *//* }}} */

/**
* Finds the first adjacent pair of equal elements in a range.
* @param array    Array containing the range.
* @param first    Beginning of the range.
* @param last     One past the end of the range.
* @return         The first iterator <code>i</code> in the range
*                 <code>[first, last-1)</code> such that 
*                 <code>array[i] == array[i+1]</code>.  Returns
*                 <code>last</code> if no such iterator exists.
*/
public static int adjacent_find(generic[] array, int first, int last)
{
  int next = first;
  while (++next < last) {
        if (array[first] == array[next])
          return first;
        else
          first = next;
  }
  
  return last;
}

/* {{{ *//* }}} */

/**
* Finds the first adjacent pair of elements in a range that satisfy
* some condition.
* @param array    Array containing the range.
* @param first    Beginning of the range.
* @param last     One past the end of the range.
* @param p        Condition that must be satisfied.
* @return         The first iterator <code>i</code> in the range
*                 <code>[first, last-1)</code> such that 
*                 <code>p.apply(array[i], array[i+1])</code> is
*                 <code>true</code>.  Returns
*                 <code>last</code> if no such iterator exists.
*/
public static int adjacent_find(generic[] array, int first, int last,
                                                          BinaryPredicate p)
{
  int next = first;
  while (++next < last) {
        if (p.apply(array[first], array[next]))
          return first;
        else
          first = next;
  }
  
  return last;
}

/* {{{ *//* }}} */

/**
* Finds the first location at which two ranges differ.  
* Note: the two
* ranges are permitted to be in the same array and are permitted to
* overlap.
* @param array1   Array containing the first range.
* @param array2   Array containing the first range.
* @param first1   Beginning of the first range
* @param last1    One past the end of the first range
* @param first2   Beginning of the second range
* @return         The first index <code>i</code> such that
*                 <code>array1[i] != array2[first2 + (i-first1)]</code>,
*                 or <code>last1</code> if no such index in the range
*                 <code>[first1,last1)</code> exists.
*/
public static int mismatch(generic[] array1, generic[] array2,
                                                 int first1, int last1, int first2)
{
  while(first1 < last1 && array1[first1] == array2[first2]) {
        ++first1;
        ++first2;
  }
  return first1;
}

/* {{{ *//* }}} */

/**
* Searches, within one range, for a sequence of elements equal
* to the elements in a second range.
* 
* Note: the two
* ranges are permitted to be in the same array and are permitted to
* overlap.
* Note: the worst-case performance of this algorithm is quadratic.
* @param array1   Array containing the first range.
* @param array2   Array containing the first range.
* @param first1   Beginning of the first range.
* @param last1    One past the end of the first range.
* @param first2   Beginning of the second range.
* @param last2    One past the end of the second range.
* @return         The first index in the range
*                 <code> [first1, last1-len) </code> such that
*                 for every non-negative <code>i&lt;len</code> 
*                 (where <code> len = last2-first2</code>), the
*                 condition 
*                 <code>array1[first1+n] == array2[first1+n]</code>
*                 is satisfied, or <code>last1</code> if no such
*                 index exists.
*/
public static int search(generic[] array1, generic[] array2,
                                                   int first1, int last1,
                                                   int first2, int last2)
{
  int len1 = last1 - first1;
  int len2 = last2 - first2;
  int cur1 = first1;
  int cur2 = first2;

  if (len1 < len2)
        return last1;

  while (cur2 < last2) {
        if (array1[cur1++] != array2[cur2++]) {
          if (len1 == len2)
                return last1;
          else {
                cur1 = ++first1;
                cur2 = first2;
                --len1;
          }
        }
  }
  
  return (cur2 == last2) ? first1 : last1;
}

/* {{{ *//* }}} */

/**
* Searches, within one range, for a sequence of elements that match
* the elements in a second range.  Matching is defined as satisfying
* a BinaryPredicate passed as an argument.
* 
* Note: the two
* ranges are permitted to be in the same array and are permitted to
* overlap.
* Note: the worst-case performance of this algorithm is quadratic.
* @param array1   Array containing the first range.
* @param array2   Array containing the first range.
* @param first1   Beginning of the first range.
* @param last1    One past the end of the first range.
* @param first2   Beginning of the second range.
* @param last2    One past the end of the second range.
* @param p        Condition to be tested pairwise.
* @return         The first index in the range
*                 <code>[first1, last1-len)</code> such that
*                 for every non-negative <code>i&lt;len</code> 
*                 (where <code>len = last2-first2</code>), the
*                 condition 
*                 <code>p.apply(array1[first1+n],array2[first1+n])</code>
*                 is satisfied, or <code>last1</code> if no such
*                 index exists.
*/
public static int search(generic[] array1, generic[] array2,
                                                   int first1, int last1,
                                                   int first2, int last2,
                                                   BinaryPredicate p)
{
  int len1 = last1 - first1;
  int len2 = last2 - first2;
  int cur1 = first1;
  int cur2 = first2;

  if (len1 < len2)
        return last1;

  while (cur2 < last2) {
        if (!p.apply(array1[cur1++], array2[cur2++])) {
          if (len1 == len2)
                return last1;
          else {
                cur1 = ++first1;
                cur2 = first2;
                --len1;
          }
        }
  }
  
  return (cur2 == last2) ? first1 : last1;
}

  /**
   * Remove all elements from a range that are equal to a given value.
   * It is not guaranteed that the relative order of remaining elements is
   * unchanged.
   * @param array    Array containing the range 
   * @param first    Beginning of the range 
   * @param last     One past the end of the range
   * @param x        Value to be removed.
   * @return         An index <code>i</code> such that all remaining elements
   *                 are contained in the range <code>[first, i)</code>.
   */
  public static int remove_if(generic[] array, int first, int last, 
                              generic x)
    {
      int oldLast = last;
      --first;
      while (true) {
        while (++first < last && array[first] != x); 
        while (first < --last && array[last] == x); 
        if (first >= last) {
          return first;
        }
        array[first] = array[last];
      }
    }
        
  /** 
   * Rearranges elements in a range such that all elements that satisfy 
   * a condition are placed before all elements that do not satisfy it.
   * @param array    Array containing the range
   * @param first    Beginning of the range
   * @param last     One past the end of the range                
   * @param p        Condition being tested
   * @return         An index <code>a</code> such that for all
   *                 <code>first <= i < a</code>,
   *                 <code>p.apply(array[i])</code> is <code>true</code>
   *                 and such that for all
   *                 <code>a <= i < last</code>, 
   *                 <code>p.apply(array[i])</code> is <code>false</code>.
   * @see   Predicate         
   */
  public static int partition(generic[] array, int first, int last, 
                              Predicate p)
    {
      --first;
      while (true) {
        while (++first < last && p.apply(array[first]));
        while (first < --last && !p.apply(array[last])); 
        if (first >= last) return first;
        generic tmp = array[first];
        array[first] = array[last];
        array[last] = tmp;
      }
    }

  /**
   * Rearranges elements in a range such that all elements that satisfy 
   * a condition are placed before all elements that do not satisfy it.
   * It is guaranteed that the relative ordering within each group is
   * unchanged.
   * @param array    Array containing the range
   * @param first    Beginning of the range
   * @param last     One past the end of the range                
   * @param p        Condition being tested
   * @return         An index <code>a</code> such that for all
   *                 <code>first <= i < a</code>,
   *                 <code>p.apply(array[i])</code> is <code>true</code>
   *                 and such that for all
   *                 <code>a <= i < last</code>, 
   *                 <code>p.apply(array[i])</code> is <code>false</code>.
   * @see   Predicate         
   */
  public static int stable_partition(generic[] array, int first, 
                                     int last, Predicate p)
    {
      if (first + 1 < last) {
        int middle = first + (last - first) / 2;
        int firstCut = stable_partition(array, first, middle, p);
        int secondCut = stable_partition(array, middle, last, p);
        rotate(array, firstCut, middle, secondCut);
        return firstCut + (secondCut - middle);
      }
      if (first >= last || !p.apply(array[first])) 
        return first;
      else
        return last;
    }

  /**
   * Computes the generalized partial sums of elements in an input range and 
   * assigns them to elements in an output range.  Generalized partial sums
   * are identical to partial sums except that addition is replaced by an
   * arbitrary binary operation <code>op</code>.
   * <code>dest[to]</code> = <code>source[first]</code>,
   * <code>dest[to+1]</code> = <code>op.apply(source[first], source[first+1])</code>,
   * etc. 
   * There must be
   * enough space in the destination array, and existing elements 
   * will be overwritten.  
   * @param source    Array containing the input range.
   * @param dest      Array containing the output range.
   * @param first     Beginning of the input range.
   * @param last      One past the end of the input range.
   * @param to        Beginning of the output range.
   * @param op        Binary operation that plays the role of addition.
   * @return          One past the end of the output range, that is,
   *                  <code>to + (last - first)</code>.
   */
  public static int partial_sum(generic[] source, generic[] dest,
                                int first, int last, int to,
                                BinaryOperator op)
    {
      if (first < last) {
        dest[to] = source[first];
        generic value = dest[to];
        while (++first < last) {
          value = op.apply(value, source[first]);
          dest[++to] = value;
        }
        return to + 1;
      }
      else
        return to;
    }

  /**
   * Sort a range of elements by arithmetic comparison.  
   * Uses the insertion sort algorithm.  This is a quadratic
   * algorithm, but it is useful for sorting small numbers of elements.
   * @param array       Array containing the range.
   * @param first       Beginning of the range.
   * @param last        One past the end of the range.
   * @see Sorting#sort
   */
  public static void insertion_sort(generic[] array, int first, int last)
    {
      for (int current = first; ++current < last; ) { 
        generic tmp = array[current];
        int i = current;
        for (generic tmp1 = array[i - 1]; tmp < tmp1; 
             tmp1 = array[--i - 1] ) {
          array[i] = tmp1;
          if (first == i - 1) {
            --i;
            break;
          }
        }
        array[i] = tmp;
      }
    }

  private static int quickPartition(generic[] array, int first, int last)
    {
      generic f = array[first];
      generic l = array[last - 1];
      generic pivot = array[first + (last - first) / 2];

      if (pivot < f) {
        if (f < l)
          pivot = f;
        else if (pivot < l)
          pivot = l;
      }
      else if (l < f)
        pivot = f;
      else if (l < pivot)
        pivot = l;

      --first;
      while (true) {
        while (array[++first] < pivot)
          { }

        while (pivot < array[--last])
          { }
        
        if (first >= last)
          return first;

        generic tmp = array[first];
        array[first] = array[last];
        array[last] = tmp;
      }
    }        
        
        
        
  private static final int partitionCutoff = 13;
  private static final int qsort_stacksize = 56;

  private static void qsortLoop(generic[] array, int first, int last)
    {
      int[] stack = new int[qsort_stacksize];
      int position = 0;
      while (true) {
        int cut = quickPartition(array, first, last);

        if (last - cut < partitionCutoff) {
          if (cut - first < partitionCutoff) {
            if (position == 0) 
              return;
            last = stack[--position];
            first = stack[--position];
          }
          else
            last = cut;
        }
        else if (cut - first <  partitionCutoff) { 
          first = cut; 
        }
        else if (last - cut > cut - first) {
          stack[position++] = cut;
          stack[position++] = last;
          last = cut;
        }
        else {
          stack[position++] = first;
          stack[position++] = cut;
          first = cut;
        } 
      }
    }        
        
        
  /**
   * Sort a range of elements by arithmetic comparison.  
   * The sort is stable---that is, the relative order of equal elements
   * is unchanged.  Worst case performance is <code>N (log N)^2</code>.
   * @param array       Array containing the range.
   * @param first       Beginning of the range.
   * @param last        One past the end of the range.
   * @see Sorting#sort
   */
  public static void stable_sort(generic[] array, int first, int last)
    {
      if (last - first < stableSortCutoff) 
        insertion_sort(array, first, last);
      else {
        int middle = first + (last - first) / 2;
        stable_sort(array, first, middle);
        stable_sort(array, middle, last);
        inplace_merge(array, first, middle, last);
      }
    }

  /**
   * Partially sorts a range by arithmetic comparison: 
   * places the first <code>middle-first</code> elements in the range 
   * <code>[first, middle)</code>.  These elements are sorted, the rest
   * are not.  It is not guaranteed that the relative ordering of 
   * unsorted elements is preserved.
   * @param array       Array containing the range.
   * @param first       Beginning of the range.
   * @param middle      Element such that the range
   *                    <code>[first, middle)</code> will be sorted.
   * @param last        One past the end of the range.
   * @see Sorting#partial_sort_copy
   * @see Sorting#sort
   */   
  public static void partial_sort(generic[] array, int first, int middle, 
                                  int last)
    {
      make_heap(array, first, middle);
      int current = middle;
      while (current < last) {
        if (array[current] < array[first]) {
          generic tmp = array[current];
          array[current] = array[first];
          array[first] = tmp;
          adjust_heap(array, first, first, middle);
        }
        ++current;
      }
      sort_heap(array, first, middle);
    } 

  /**
   * Partitions a range of elements into two subranges
   * <code>[first, nth)</code> and <code>[nth, last)</code>.  These
   * satisfy the properties that no element in the first range is greater
   * than any element in the second, and that the element in the
   * position <code>nth</code> is the same as the one that would be
   * in that position if the entire range <code>[first, last)</code>
   * had been sorted.  Sorting is by arithmetic comparison.
   * @param array       Array containing the range.
   * @param first       Beginning of the range.
   * @param nth         Location of the partition point.
   * @param last        One past the end of the range.
   */
  public static void nth_element(generic[] array, int first, int nth, 
                                 int last)
    {
      while (last - first > 3) {
        int cut = quickPartition(array, first, last);
        if (cut <= nth)
          first = cut;
        else
          last = cut;
      }
      
      insertion_sort(array, first, last);
    }

  /**
   * Performs a binary search on an already-sorted range: finds the first
   * position where an element can be inserted without violating the ordering.
   * Sorting is by arithmetic comparison.
   * @param array    Array containing the range.
   * @param first    Beginning of the range.
   * @param last     One past the end of the range.
   * @param x        Element to be searched for.
   * @return         The largest index i such that, for every j in the
   *                 range <code>[first, i)</code>, 
   *                 <code>array[j] &lt; x</code>.
   * @see Sorting#upper_bound
   * @see Sorting#equal_range
   * @see Sorting#binary_search
   */
  public static int lower_bound(generic[] array, int first, int last,
                                generic x)
    {
      int len = last - first;
      while (len > 0) {
        int half = len / 2;
        int middle = first + half;
        if (array[middle] < x) {
          first = middle + 1;
          len -= half + 1;
        } else
          len = half;
      }
      return first;
    }          
        
  /**
   * Performs a binary search on an already-sorted range: finds the last
   * position where an element can be inserted without violating the ordering.
   * Sorting is by arithmetic comparison.
   * @param array    Array containing the range
   * @param first    Beginning of the range
   * @param last     One past the end of the range
   * @param x        Element to be searched for
   * @return         The largest index i such that, for every j in the
   *                 range <code>[first, i)</code>, 
   *                 <code>!(x &lt; array[j])</code>.
   * @see Sorting#lower_bound
   * @see Sorting#equal_range
   * @see Sorting#binary_search
   */
  public static int upper_bound(generic[] array, int first, int last,
                                generic x)
    {
      int len = last - first;
      while (len > 0) {
        int half = len / 2;
        int middle = first + half;
        if (x < array[middle])
          len = half;
        else {
          first = middle + 1;
          len -= half + 1;
        }
      }
      return first;
    } 

  /**
   * Performs a binary search on an already-sorted range:
   * Finds the largest subrange in the supplied range such that an
   * element can be inserted at any point in that subrange without violating
   * the existing ordering.  Sorting is by arithmetic comparison.
   * @param array    Array containing the range.
   * @param first    Beginning of the range.
   * @param last     One past the end of the range.
   * @param x        Element to be searched for.
   * @return         An object <code>R</code>of class <code>R</code> such 
   *                 that, for any index <code>i</code> in the range
   *                 <code>[R.first, R.last)</code>, the conditions
   *                 <code>array[i] &lt; x</code> and
   *                 <code>x &lt; array[i]</code> are both false.
   *                 Note that it is possible for the return value to be
   *                 an empty range.
   * @see Sorting#lower_bound
   * @see Sorting#upper_bound
   * @see Sorting#binary_search
   */
  public static Range equal_range(generic[] array, int first, int last,
                                  generic x)
    {
      int len = last - first;
      while (len > 0) {
        int half = len / 2;
        int middle = first + half;
        if (array[middle] < x) {
          first = middle + 1;
          len = len - half + 1;
        }
        else if (x < array[middle])
          len = half;
        else {
          int left  = lower_bound(array, first, middle, x);
          int right = upper_bound(array, middle + 1, first + len, x);
          return new Range(array, left, right);
        }
      }

      return new Range(array, first, first); // An empty range.
    }

  /**
   * Transforms two consecutive sorted ranges into a single sorted 
   * range.  The initial ranges are <code>[first, middle)</code>
   * and <code>[middle, last)</code>, and the resulting range is
   * <code>[first, last)</code>.  
   * Elements in the first input range will precede equal elements in the 
   * second.
   * Sorting is by arithmetic comparison.
   * @param array    Array containing the ranges.
   * @param first    Beginning of the first range.
   * @param middle   One past the end of the first range, and beginning
   *                 of the second.
   * @param last     One past the end of the second range.
   * @see Sorting#merge
   */
  public static void inplace_merge(generic[] array, 
                                   int first, int middle, int last)
    {

      if (first >= middle || middle >= last)
        return;

      if (last - first == 2) {
        if (array[middle] < array[first]) {
          generic tmp = array[first];
          array[first] = array[middle];
          array[middle] = tmp;
        }
        return;
      }

      int firstCut;
      int secondCut;

      if (middle - first > last - middle) {
        firstCut = first + (middle - first) / 2;
        secondCut = lower_bound(array, middle, last, array[firstCut]);
      }
      else {
        secondCut = middle + (last - middle) / 2;
        firstCut = upper_bound(array, first, middle, array[secondCut]);
      }

      Modification.rotate(array, firstCut, middle, secondCut);
      middle = firstCut + (secondCut - middle);

      inplace_merge(array, first, firstCut, middle);
      inplace_merge(array, middle, secondCut, last);
    }

  /**
   * Tests whether the first range is a superset of the second; both ranges
   * must be sorted.
   * Sorting is by arithmetic comparison.
   * @param array1   Array containing the first range.
   * @param array2   Array containing the second range.
   * @param first1   Beginning of the first range.
   * @param last1    One past the end of the first range.
   * @param first2   Beginning of the second range.
   * @param last2    One past the end of the second range.
   * @return         <code>true</code> if and only if, for every element in
   *                 the range <code>[first2,last2)</code>, the range
   *                 <code>[first1,last1)</code> contains an equivalent 
   *                 element.
   * @see Sorting#set_union
   * @see Sorting#set_intersection
   * @see Sorting#set_difference
   * @see Sorting#set_symmetric_difference
   */
  public static boolean includes(generic[] array1, generic[] array2,
                                 int first1, int last1, int first2, int last2)
    {
      while (first1 < last1 && first2 < last2) {
        if (array2[first2] < array1[first1])
          return false;
        else if (array1[first1] < array2[first2])
          ++first1;
        else {
          ++first1;
          ++first2;
        }
      }

      return first2 == last2;            
    }

  /**
   * Constructs a union of two already-sorted ranges.  That is, 
   * the output range will be a sorted range containing every element from
   * either of the two input ranges.  If an element in the second range
   * is equivalent to one in the first, the one in the first range is
   * copied.  
   * There must be
   * enough space in the destination array, and existing elements 
   * will be overwritten.
   * Sorting is by arithmetic comparison.
   * Note: the destination range is not permitted to overlap either of 
   * the two input ranges.
   * @param source1     Array containing the first input range.
   * @param source2     Array containing the second input range.
   * @param destination Array containing the output range.
   * @param first1      Beginning of the first input range.
   * @param last1       One past the end of the first input range.
   * @param first2      Beginning of the second input range.
   * @param last2       One past the end of the second input range.
   * @param to          Beginning of the output range.
   * @return            One past the end of the output range.
   * @see Sorting#includes
   * @see Sorting#set_intersection
   * @see Sorting#set_difference
   * @see Sorting#set_symmetric_difference
   */ 
  public static int set_union(generic[] source1, generic[] source2,
                              generic[] destination,
                              int first1, int last1, int first2, int last2,
                              int to)
    {
      while (first1 < last1 && first2 < last2) {
        if (source1[first1] < source2[first2])
          destination[to++] = source1[first1++];
        else if (source2[first2] < source1[first1])
          destination[to++] = source2[first2++];
        else {
          destination[to++] = source1[first1++];
          first2++;
        }
      }

      Modification.copy(source1, destination, first1, last1, to);
      Modification.copy(source2, destination, first2, last2, to);
      return to + (last1 - first1) + (last2 - first2);
    }

  /**
   * Adds an element to a heap.  The range <code>[first, last-1)</code>
   * must be a valid heap, and the element to be added must be in
   * <code>array[last-1]</code>.  
   * The heap is ordered by arithmetic comparison.
   * @param array    Array containing the heap.
   * @param first    Beginning of the heap.
   * @param last     Index such that <code>[first, last-1)</code> is a
   *                 valid heap and such that <code>array[last-1]</code> 
   *                 contains the element to be added to the heap. 
   * @see Sorting#make_heap
   */
  public static void push_heap(generic[] array, int first, int last)
    {
      if (last - first < 2) return;
      generic tmp = array[--last];
      int parent = first + ((last - first) - 1) / 2;
      while (last > first && array[parent] < tmp) {
        array[last] = array[parent];
        last = parent;
        parent = first + ((last - first) - 1) / 2;
      }
      array[last] = tmp;
    }

  /** 
   * Fixes a heap that is slightly invalid.  If the range
   * <code>[first, last)</code> is a valid heap except for the element
   * <code>array[position]</code>, rearrange elements so that it is
   * a valid heap again.
   * The heap is ordered by arithmetic comparison.
   * @param array    Array containing the heap.
   * @param first    Beginning of the heap.
   * @param position Index of the incorrectly positioned element.
   * @param last     One past the end of the heap.
   * @see Sorting#make_heap
   */
  private static void adjust_heap(generic[] array,
                                    int first, int position, int last)
    {
      generic tmp = array[position];
      int len = last - first;
      int holeIndex = position - first;
      int secondChild = 2 * holeIndex + 2;
      while (secondChild < len) {
        if (array[first + secondChild] < 
            array[first + (secondChild - 1)])
          --secondChild;
        array[first + holeIndex] = array[first + secondChild];
        holeIndex = secondChild++;
        secondChild *= 2;
      }
      if (secondChild-- == len) {
        array[first + holeIndex] = array[first + secondChild];
        holeIndex = secondChild;
      }

      int parent = (holeIndex - 1) / 2;
      int topIndex = position - first;
      
      while (holeIndex != topIndex && array[first + parent] < tmp) {
        array[first + holeIndex] = array[first + parent];
        holeIndex =  parent;
        parent = (holeIndex - 1) / 2;
      }
      array[first + holeIndex] = tmp;
    }

  /**  
   * Removes the largest element from a heap.  If the range 
   * <code>[first, last)</code> is a valid heap, then remove
   * <code>array[first]</code> (the largest element) from the heap,
   * rearrange elements such that <code>[first, last-1)</code> is
   * a valid heap, and place the removed element in <code>array[last]</code>.
   * The heap is ordered by arithmetic comparison.
   * @param array    Array containing the heap.
   * @param first    Beginning of the heap.
   * @param last     One past the end of the heap.
   * @see Sorting#make_heap
   */
  public static void pop_heap(generic[] array, int first, int last)
    {
      if (last - first < 2) return;
      generic tmp = array[--last];
      array[last] = array[first];
      array[first] = tmp;
      adjust_heap(array, first, first, last);
    }


  /**
   * Turns the range <code>[first, last)</code> into a heap.  A heap has
   * the properties that <code>array[first]</code> is the largest element,
   * and that it is possible to add a new element, or to remove 
   * <code>array[first]</code>, efficiently.
   * The heap is ordered by arithmetic comparison.
   * @param array    Array containing the range that is to be made a heap.
   * @param first    Beginning of the range.
   * @param last     One past the end of the range.
   * @see Sorting#push_heap
   * @see Sorting#pop_heap
   * @see Sorting#sort_heap
   */
  public static void make_heap(generic[] array, int first, int last)
    {
      if (last - first < 2) return;
      int parent = (last - first - 2) / 2;

      do 
        adjust_heap(array, first, first + parent, last);
      while (parent-- != 0);
    } 

  /**
   * Turns a heap into a sorted range; this operation is
   * <code>O(N log N)</code>.  Note that <code>make_heap</code>
   * followed by <code>sort_heap</code> is the heap sort algorithm.
   * Ordering is by arithmetic comparison.
   * @param array    Array containing the heap that is to be made a sorted
   *                 range.
   * @param first    Beginning of the heap.
   * @param last     One past the end of the range.
   * @see Sorting#make_heap
   */
  public static void sort_heap(generic[] array, int first, int last)
    {
      while (last - first > 1) {
        generic tmp = array[--last];
        array[last] = array[first];
        array[first] = tmp;
        adjust_heap(array, first, first, last);
      }
    } 

  /**
   * Finds the largest element in a range.
   * Ordering is by arithmetic comparison.
   * @param array    Array containing the range. 
   * @param first    Beginning of the range. 
   * @param last     End of the range. 
   * @return         The smallest index <code>i</code> such that every element
   *                 in the range is less than or equivalent to 
   *                 <code>array[i]</code>.  Returns <code>last</code>
   *                 if the range is empty.
   * @see Sorting#min_element                    
   */
  public static int max_element(generic[] array, int first, int last)
    {
      if (first >= last) return last;

      int result = first;

      while (++first < last) 
        if (array[result] < array[first])
          result = first;

      return result;
    }

  /** 
   * Transforms a range of elements into the next permutation of those
   * elements, where the <em>next</em> permutation is defined by 
   * a lexicographical ordering of the set of all permutations.
   * If no such permutation exists, transforms the range into the 
   * smallest permutation.
   * Ordering of individual elements is by arithmetic comparison.
   * @param array    Array containing the range.
   * @param first    Beginning of the range.
   * @param last     One past the end of the range.
   * @return         <code>true</code> if a next permutation exists,
   *                 <code>false</code> if the range is already the largest
   *                 permutation.
   * @see            Sorting#lexicographical_compare
   * @see            Sorting#prev_permutation
   */
  public static boolean next_permutation(generic[] array, int first, int last)
    {
      if (last - first < 2)
        return false;

      int i  = last - 1;
      while(true) {
        int ii = i--;
        if (array[i] < array[ii]) {
          int j = last;
          while (!(array[i] < array[--j]))
            { }
          generic tmp = array[i];
          array[i] = array[j];
          array[j] = tmp;
          Modification.reverse(array, ii, last);
          return true;
        }
        if (i == first) {
          Modification.reverse(array, first, last);
          return false;
        }
      }
    }

          /**
   * Tests whether two ranges are pairwise equal.
   * Note: the two
   * ranges are permitted to be in the same array and are permitted to
   * overlap.
   * @param array1   Array containing the first range.
   * @param array2   Array containing the first range.
   * @param first1   Beginning of the first range
   * @param last1    One past the end of the first range
   * @param first2   Beginning of the second range
   * @return         <code>true</code> if, for every index <code>i</code>
   *                 in the range <code>[first1,last1)</code>, 
   *                 <code>array1[i] == array2[first2 + (i-first1)]</code>,
   *                 otherwise returns <code>false</code>.
   */
  public static boolean equal(generic[] array1, generic[] array2,
                              int first1, int last1, int first2)
    {
      while (first1 < last1 && array1[first1] == array2[first2]) {
        ++first1;
        ++first2;
      }

      return first1 >= last1;
    }
        
  /**
   * Searches, within one range, for a sequence of elements equal
   * to the elements in a second range.
   * 
   * Note: the two
   * ranges are permitted to be in the same array and are permitted to
   * overlap.
   * Note: the worst-case performance of this algorithm is quadratic.
   * @param array1   Array containing the first range.
   * @param array2   Array containing the first range.
   * @param first1   Beginning of the first range.
   * @param last1    One past the end of the first range.
   * @param first2   Beginning of the second range.
   * @param last2    One past the end of the second range.
   * @return         The first index in the range
   *                 <code> [first1, last1-len) </code> such that
   *                 for every non-negative <code>i&lt;len</code> 
   *                 (where <code> len = last2-first2</code>), the
   *                 condition 
   *                 <code>array1[first1+n] == array2[first1+n]</code>
   *                 is satisfied, or <code>last1</code> if no such
   *                 index exists.
   */
  public static int search(generic[] array1, generic[] array2,
                               int first1, int last1,
                               int first2, int last2)
    {
      int len1 = last1 - first1;
      int len2 = last2 - first2;
      int cur1 = first1;
      int cur2 = first2;

      if (len1 < len2)
        return last1;

      while (cur2 < last2) {
        if (array1[cur1++] != array2[cur2++]) {
          if (len1 == len2)
            return last1;
          else {
            cur1 = ++first1;
            cur2 = first2;
            --len1;
          }
        }
      }
      
      return (cur2 == last2) ? first1 : last1;
    }
        

  /**
   * Remove all elements from a range that satisfy a specified condition.
   * It is not guaranteed that the relative order of remaining elements is
   * unchanged.
   * @param array    Array containing the range 
   * @param first    Beginning of the range 
   * @param last     One past the end of the range
   * @param p        Condition being tested
   * @return         An index <code>i</code> such that all remaining elements
   *                 are contained in the range <code>[first, i)</code>.
   */
  public static int remove_if(generic[] array, int first, int last, 
                              Predicate p)
    {
      int oldLast = last;
      --first;
      while (true) {
        while (++first < last && !p.apply(array[first])); 
        while (first < --last && p.apply(array[last])); 
        if (first >= last) {
          return first;
        }
        array[first] = array[last];
      }
    } 
        
  /**
   * Remove all elements from a range that satisfy a specified condition.
   * It is guaranteed that the relative order of remaining elements is
   * unchanged.
   * @param array    Array containing the range 
   * @param first    Beginning of the range 
   * @param last     One past the end of the range
   * @param p        Condition being tested
   * @return         An index <code>i</code> such that all remaining elements
   *                 are contained in the range <code>[first, i)</code>.
   */
  public static int stable_remove_if(generic[] array, int first, int last, 
                                     Predicate p)
    {
      first = Inspection.find_if(array, first, last, p);
      int next = Inspection.find_if_not(array, first, last, p);
      while (next < last) {
        array[first++] = array[next];
        next = Inspection.find_if_not(array, ++next, last, p);
      }
      return first;
    } 
        
  /**
   * Rotate a range in place: <code>array[middle]</code> is put in
   * <code>array[first]</code>, <code>array[middle+1]</code> is put in
   * <code>array[first+1]</code>, etc.  Generally, the element in position
   * <code>i</code> is put into position 
   * <code>(i + (last-middle)) % (last-first)</code>.
   * @param array    Array containing the range
   * @param first    Beginning of the range
   * @param middle   Index of the element that will be put in
   *                 <code>array[first]</code>
   * @param last     One past the end of the range
   */
  public static void rotate(generic[] array, int first, int middle, 
                                 int last)
    {
      if (middle != first && middle != last) {
        reverse(array, first, middle);
        reverse(array, middle, last);
        reverse(array, first, last);
      }
    }
        
  /** 
   * Shuffles elements in a range, with uniform distribution.  
   * @param array     Array containing the range to be shuffled
   * @param first     Beginning of the range
   * @param last      One past the end of the range
   * @param RNG       Object of class <code>java.util.Random</code>,
   *                  used to supply random numbers.
   */
  public static void random_shuffle(generic[] array, int first, int last,
                                    Random RNG)
    {
      for (int i = first + 1; i < last; ++i) {
        int randomPlace =  Math.abs(RNG.nextInt()) % 
          ((i - first) + 1);
        generic tmp = array[randomPlace];
        array[randomPlace] = array[i];
        array[i] = tmp;
      }
    }
        
  /** 
   * Rearranges elements in a range such that all elements that satisfy 
   * a condition are placed before all elements that do not satisfy it.
   * @param array    Array containing the range
   * @param first    Beginning of the range
   * @param last     One past the end of the range                
   * @param p        Condition being tested
   * @return         An index <code>a</code> such that for all
   *                 <code>first <= i < a</code>,
   *                 <code>p.apply(array[i])</code> is <code>true</code>
   *                 and such that for all
   *                 <code>a <= i < last</code>, 
   *                 <code>p.apply(array[i])</code> is <code>false</code>.
   * @see   Predicate         
   */
  public static int partition(generic[] array, int first, int last, 
                              Predicate p)
    {
      --first;
      while (true) {
        while (++first < last && p.apply(array[first]));
        while (first < --last && !p.apply(array[last])); 
        if (first >= last) return first;
        generic tmp = array[first];
        array[first] = array[last];
        array[last] = tmp;
      }
    }

  /**
   * Rearranges elements in a range such that all elements that satisfy 
   * a condition are placed before all elements that do not satisfy it.
   * It is guaranteed that the relative ordering within each group is
   * unchanged.
   * @param array    Array containing the range
   * @param first    Beginning of the range
   * @param last     One past the end of the range                
   * @param p        Condition being tested
   * @return         An index <code>a</code> such that for all
   *                 <code>first <= i < a</code>,
   *                 <code>p.apply(array[i])</code> is <code>true</code>
   *                 and such that for all
   *                 <code>a <= i < last</code>, 
   *                 <code>p.apply(array[i])</code> is <code>false</code>.
   * @see   Predicate         
   */
  public static int stable_partition(generic[] array, int first, 
                                     int last, Predicate p)
    {
      if (first + 1 < last) {
        int middle = first + (last - first) / 2;
        int firstCut = stable_partition(array, first, middle, p);
        int secondCut = stable_partition(array, middle, last, p);
        rotate(array, firstCut, middle, secondCut);
        return firstCut + (secondCut - middle);
      }
      if (first >= last || !p.apply(array[first])) 
        return first;
      else
        return last;
    } 