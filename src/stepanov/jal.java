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

