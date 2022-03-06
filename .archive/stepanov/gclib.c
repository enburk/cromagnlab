#define TYPE int
#define REGISTER register
#define ptrdiff_t int
#define register

TYPE *binary_partition(register TYPE value, 
		       register TYPE *begin, 
		       register TYPE *end)
{
  while (5 < end-begin)
    {
      register TYPE *index = begin + ((end - begin) >> 1);

      if (value < *index)
	end = index;
      else
	begin = index + 1;
    }

  while (begin < end) 
    if (!(value < *--end))
      return end + 1;

  return end;
}


TYPE *binary_partition(register int (*relation)(TYPE*, TYPE*), 
		       TYPE value, 
		       register TYPE *begin, 
		       register TYPE *end)
{
  register TYPE *temp = &value;
  while (5 < end-begin)
    {
      register TYPE *index = begin + ((end - begin) >> 1);

      if ((*relation)(temp, index) < 0)
	end = index;
      else
	begin = index + 1;
    }

  while (begin < end) 
    if (!((*relation)(temp, --end) < 0))
      return end + 1;

  return end;
}


TYPE *binary_search(TYPE value, 
		    TYPE *begin, 
		    TYPE *end)
{
  TYPE *index = binary_partition(value, begin, end);

  if (begin < index && !(*(index - 1) < value))
    return index - 1;

  else
    return 0;
}


TYPE *binary_search(int (*relation)(TYPE*, TYPE*), 
		    TYPE value, 
		    TYPE *begin, 
		    TYPE *end)
{
  TYPE *index = binary_partition(relation, value, begin, end);

  if (begin < index && !((*relation)(index - 1, &value) < 0))
    return index - 1;

  else
    return 0;
}


void copy(register TYPE *begin, 
	  register TYPE *end, 
	  register TYPE *result)
{
  if (begin < result)
    {
      result += end - begin;
      while (begin < end)
	*--result = *--end;
    }
  else if (result < begin)
    while (begin < end)
      *result++ = *begin++;
}


ptrdiff_t count(register int (*predicate)(TYPE*), 
		register TYPE *begin, 
		register TYPE *end)
{
  register ptrdiff_t n = 0; 

  while (begin < end)
   if ((*predicate)(begin++))
     n++;

  return n;
}


ptrdiff_t count(register TYPE value, 
		register TYPE *begin, 
		register TYPE *end)
{
  register ptrdiff_t n = 0; 

  while (begin < end)
    if (*begin++ == value)
      n++;

  return n;
}


ptrdiff_t count(register int (*relation)(TYPE*, TYPE*), 
		TYPE value, 
		register TYPE *begin, 
		register TYPE *end)
{
  register TYPE *temp = &value;
  register ptrdiff_t n = 0; 

  while (begin < end)
    if ((*relation)(begin++, temp))
      n++;

  return n;
}


TYPE *difference(register int (*relation)(TYPE*, TYPE*), 
		 register TYPE *begin1, 
		 register TYPE *end1, 
		 register TYPE *begin2, 
		 register TYPE *end2, 
		 register TYPE *result)
{
  while (begin1 < end1 && begin2 < end2)
    {
      register int c = (*relation)(begin2, begin1);
      if (0 < c)
	*result++ = *begin1++;
      else 
	{
	  if (c == 0)
	    begin1++;
	  begin2++;
	}
    }

  while (begin1 < end1)
    *result++ = *begin1++;

  return result;
}


TYPE *difference(register TYPE *begin1, 
		 register TYPE *end1, 
		 register TYPE *begin2, 
		 register TYPE *end2, 
		 register TYPE *result)
{
  while (begin1 < end1 && begin2 < end2)
    if (*begin1 < *begin2)
      *result++ = *begin1++;
    else if (!(*begin2++ < *begin1))
      begin1++;

  while (begin1 < end1)
    *result++ = *begin1++;

  return result;
}



void fill(register TYPE value, 
	  register TYPE *begin, 
	  register TYPE *end)
{
  while (begin < end)
    *begin++ = value;
}


void for_each(register void (*function)(TYPE*), 
	      register TYPE *begin, 
	      register TYPE *end)
{
  while (begin < end)
    (*function)(begin++);
}


void generate(register void (*function)(ptrdiff_t, TYPE*), 
	      register TYPE *begin, 
	      register TYPE *end)
{
  register TYPE *index = begin;

  for (; begin < end; begin++)
    (*function)(begin - index, begin);
}


TYPE *insert(TYPE value, 
	     TYPE *begin, 
	     TYPE *end)
{
  begin = binary_partition(value, begin, end);

  copy(begin, end, begin + 1);

  *begin = value;

  return begin;
}


TYPE *insert(int (*relation)(TYPE*, TYPE*), 
	     TYPE value, 
	     TYPE *begin, 
	     TYPE *end)
{
  begin = binary_partition(relation, value, begin, end);

  copy(begin, end, begin + 1);

  *begin = value;

  return begin;
}


void insertion_sort(register TYPE *begin, 
		    register TYPE *end)
{
  if (end - begin < 2)  /* size < 2 */
    return;

  TYPE *r = minimum(begin, end); /* create a sentinel */
  TYPE temp = *begin;          /* swap */
  *begin = *r;
  *r = temp;

  begin++; /* there is no need to insert the second element */
  
  while (++begin < end)
    {
      register TYPE value = *begin;
      register TYPE *index = begin;
      
      while (value < *--index)
	*(index + 1) = *index;
      
      *(index + 1) = value;
    }
}


void insertion_sort(register int (*relation)(TYPE*, TYPE*), 
		    register TYPE *begin, 
		    TYPE *end)
{
  register TYPE *index = begin;

  while (++index < end)
    {
      TYPE value = *index;
      register TYPE *temp = &value;
      register TYPE *current = index;

      for (; begin < current && (*relation)(temp , current - 1) < 0; current--)
	*current = *(current - 1);

      *current = value;
    }
}


TYPE *intersection(register int (*relation)(TYPE*, TYPE*), 
		   register TYPE *begin1, 
		   register TYPE *end1, 
		   register TYPE *begin2, 
		   register TYPE *end2, 
		   register TYPE *result)
{
  while (begin1 < end1 && begin2 < end2)
    {
      register int c = (*relation)(begin2, begin1);

      if (c < 0)
	begin2++;
      else if (0 < c)
	begin1++;
      else
	{
	  *(result++) = *(begin1++);
	  begin2++;
	}
    }
  return result;
}


TYPE *intersection(register TYPE *begin1, 
		   register TYPE *end1, 
		   register TYPE *begin2, 
		   register TYPE *end2, 
		   register TYPE *result)
{
  while (begin1 < end1 && begin2 < end2)
    if (*begin2 < *begin1)
      begin2++;
    else if (*begin1 < *begin2)
      begin1++;
    else
      {
	*(result++) = *(begin1++);
	begin2++;
      }
  return result;
}


void merge(register int (*relation)(TYPE*, TYPE*), 
	   register TYPE *begin1, 
	   register TYPE *end1, 
	   register TYPE *begin2, 
	   register TYPE *end2, 
	   register TYPE *result)
{
  while (begin1 < end1 && begin2 < end2)
    *result++ = ((*relation)(begin2, begin1) < 0 ? *begin2++ : *begin1++);

  while (begin1 < end1)
    *result++ = *begin1++;
  
  while (begin2 < end2)
    *result++ = *begin2++;
}


void merge(register TYPE *begin1, 
	   register TYPE *end1, 
	   register TYPE *begin2, 
	   register TYPE *end2, 
	   register TYPE *result)
{
  while (begin1 < end1 && begin2 < end2)
    *result++ = (*begin2 < *begin1 ? *begin2++ : *begin1++);

  while (begin1 < end1)
    *result++ = *begin1++;
  
  while (begin2 < end2)
    *result++ = *begin2++;

}


static void merge_sort_step(ptrdiff_t number, 
			    TYPE *begin, 
			    TYPE *end, 
			    TYPE *result)
{
  ptrdiff_t m = 2 * number;

  while (begin + m < end)
    {
      merge(begin, begin + number, begin + number, begin + m, result);
      begin += m;
      result += m;
    }

  if (begin + number + 1 < end)
    merge(begin, begin + number, begin + number, end, result);
  else
    while (begin < end)
      *result++ = *begin++;
}

static void insertion_sort_chunks(ptrdiff_t number, 
				  TYPE *begin, 
				  TYPE *end)
{
  for (; begin + number < end; begin += number)
    insertion_sort(begin, begin + number);

  insertion_sort(begin, end);
}

TYPE *merge_sort(TYPE *begin, 
		 TYPE *end, 
		 TYPE *result)
{
  ptrdiff_t number = 8, length = end - begin;

  insertion_sort_chunks(number, begin, end);

  for (; number < length; 
       number += number, end = begin, begin = result, result = end)
    merge_sort_step(number, begin, begin + length, result);

  return begin;
}


static void merge_sort_step(int (*relation)(TYPE*, TYPE*), 
			    ptrdiff_t number, 
			    TYPE *begin, 
			    TYPE *end, 
			    TYPE *result)
{
  ptrdiff_t m = 2 * number;

  while (begin + m < end)
    {
      merge(relation, begin, begin + number, begin + number, begin + m, 
	    result);
      begin += m;
      result += m;
    }

  if (begin + number + 1 < end)
    merge(relation, begin, begin + number, begin + number, end, result);
  else
    while (begin < end)
      *result++ = *begin++;
}

static void insertion_sort_chunks(int (*relation)(TYPE*, TYPE*), 
				  ptrdiff_t number, 
				  TYPE *begin, 
				  TYPE *end)
{
  for (; begin + number < end; begin += number)
    insertion_sort(relation, begin, begin + number);

  insertion_sort(relation, begin, end);
}

TYPE *merge_sort(int (*relation)(TYPE*, TYPE*), 
		 TYPE *begin, 
		 TYPE *end, 
		 TYPE *result)
{
  ptrdiff_t number = 8, length = end - begin;

  insertion_sort_chunks(relation, number, begin, end);

  for (; number < length; 
       number += number, end = begin, begin = result, result = end)
    merge_sort_step(relation, number, begin, begin + length, result);

  return begin;
}


TYPE *minimum(register TYPE *begin, 
	      register TYPE *end)
{
  register TYPE *index = begin;

  while (++begin < end)
    if (*begin < *index)
      index = begin;

  return index;
}


TYPE *minimum(register int (*relation)(TYPE*, TYPE*), 
	      register TYPE *begin, 
	      register TYPE *end)
{
  register TYPE *index = begin;

  while (++begin < end)
    if ((*relation)(begin, index) < 0)
      index = begin;

  return index;
}


TYPE *mismatch(register TYPE *begin1, 
	       register TYPE *end1, 
	       register TYPE *begin2, 
	       register TYPE *end2)
{
  register ptrdiff_t len = 
    end1 - begin1 < end2 - begin2 ? end1 - begin1 : end2 - begin2;

  while (0 < len--)
    if (!(*begin1++ == *begin2++))
      return begin1;

  return 0;
}


TYPE *mismatch(register int (*relation)(TYPE*, TYPE*), 
	       register TYPE *begin1, 
	       register TYPE *end1, 
	       register TYPE *begin2, 
	       register TYPE *end2)
{
  register ptrdiff_t len = 
    end1 - begin1 < end2 - begin2 ? end1 - begin1 : end2 - begin2;

  while (0 < len--)
    if (!(*relation)(begin1++, begin2++))
      return begin1;

  return 0;
}


TYPE *partition(register int (*predicate)(TYPE*), 
		register TYPE *begin, 
		register TYPE *end) 
{
  for (;;)
    {
      for (;;begin++) 
	if (begin >= end) 
	  return end;
	else if (!(*predicate)(begin))
	  break;

      for (;;) 
	if (begin >= --end)
	  return end;
	else if ((*predicate)(end))
	  break;

      {
	REGISTER TYPE temp = *begin;
	*begin++ = *end;
	*end = temp;
      }
    }
}


TYPE *partition(REGISTER TYPE value, 
		register TYPE *begin, 
		register TYPE *end) 
{
  for (;;)
    {
      for (;;begin++)
	if (begin >= end) 
	  return end;
	else if (!(*begin == value))
	  break;

      for (;;) 
	if (begin >= --end)
	  return end;
	else if (*end == value)
	  break;

      {
	REGISTER TYPE temp = *begin;
	*begin++ = *end;
	*end = temp;
      }
    }
}


TYPE *partition(register int (*relation)(TYPE*, TYPE*), 
		TYPE value, 
		register TYPE *begin, 
		register TYPE *end)
{
  register TYPE *temp = &value;

  for (;;)
    {
      for (;;begin++) 
	if (begin >= end) 
	  return end;
	else if (!(*relation)(begin, temp))
	  break;

      for (;;) 
	if (begin >= --end)
	  return end;
	else if ((*relation)(end, temp))
	  break;

      {
	REGISTER TYPE temp = *begin;
	*begin++ = *end;
	*end = temp;
      }
    }
}


TYPE *partition_copy(register int (*predicate)(TYPE*), 
		     register TYPE *begin, 
		     register TYPE *end, 
		     register TYPE *result)
{
  register TYPE *last = result + (end - begin);

  for (; begin < end; begin++)
    if ((*predicate)(begin))
      *(result++) = *begin;
    else
      *--last = *begin;

  return last;
}


TYPE *partition_copy(REGISTER TYPE value, 
		     TYPE *begin, 
		     register TYPE *end, 
		     register TYPE *result)
{
  register TYPE *last = result + (end - begin); 

  for (; begin < end; begin++)
    if (*begin == value)
      *(result++) = *begin;
    else
      *(--last) = *begin;

  return last;
}


TYPE *partition_copy(register int (*relation)(TYPE*, TYPE*), 
		     TYPE value, 
		     register TYPE *begin, 
		     register TYPE *end, 
		     register TYPE *result)
{
  register TYPE *last = result + (end - begin);
  register TYPE *temp = &value;
 
  for (; begin < end; begin++)
    if ((*relation)(begin, temp))
      *(result++) = *begin;
    else
      *(--last) = *begin;

  return last;
}


TYPE *position(register int (*predicate)(TYPE*), 
	       register TYPE *begin, 
	       register TYPE *end)
{
  while (begin < end)
    if ((*predicate)(begin++))
      return begin - 1;

  return 0;
}


TYPE *position(REGISTER TYPE value, 
	       register TYPE *begin, 
	       register TYPE *end)
{
  while (begin < end)
    if (*begin++ == value)
      return begin - 1;

  return 0;
}


TYPE *position(register int (*relation)(TYPE*, TYPE*), 
	       TYPE value, 
	       register TYPE *begin, 
	       register TYPE *end)
{
  register TYPE *temp = &value;

  while (begin < end) 
    if ((*relation)(begin++, temp))
      return begin - 1;

  return 0;
}


/* #include <rand48.h> */

TYPE *random(TYPE *begin, 
	     TYPE *end)
{

  if (begin < end) 
    return begin + (ptrdiff_t)(drand48() * (end - begin));
  else
    return 0;

}


TYPE *remove(register int (*predicate)(TYPE*), 
	     register TYPE *begin, 
	     register TYPE *end)
{
  for (;;)
    {
      for (;;begin++) 
	if (begin >= end) 
	  return end;
	else if ((*predicate)(begin))
	  break;

      for (;;) 
	if (begin >= --end)
	  return end;
	else if (!(*predicate)(end))
	  break;

      *begin++ = *end;
    }
}


TYPE *remove(REGISTER TYPE value, 
	     register TYPE *begin, 
	     register TYPE *end)
{
  for (;;)
    {
      for (;;begin++)
	if (begin >= end) 
	  return end;
	else if (*begin == value)
	  break;

      for (;;) 
	if (begin >= --end)
	  return end;
	else if (!(*end == value))
	  break;

      *begin++ = *end;
    }
}


TYPE *remove(register int (*relation)(TYPE*, TYPE*), 
	     TYPE value, 
	     register TYPE *begin, 
	     register TYPE *end)
{
  register TYPE *temp = &value;
  for (;;)
    {
      for (;;begin++) 
	if (begin >= end) 
	  return end;
	else if ((*relation)(begin, temp))
	  break;

      for (;;) 
	if (begin >= --end)
	  return end;
	else if (!(*relation)(end, temp))
	  break;

      *begin++ = *end;
    }
}


TYPE *remove_copy(register int (*predicate)(TYPE*), 
		  register TYPE *begin, 
		  register TYPE *end, 
		  register TYPE *result)
{
  for (; begin < end; begin++)
    if (!(*predicate)(begin))
      *result++ = *begin;

  return result;
}


TYPE *remove_copy(REGISTER TYPE value, 
		  register TYPE *begin, 
		  register TYPE *end, 
		  register TYPE *result)
{
  for (; begin < end; begin++)
    if (!(*begin == value))
      *result++ = *begin;

  return result;
}


TYPE *remove_copy(register int (*relation)(TYPE*, TYPE*), 
		  TYPE value, 
		  register TYPE *begin, 
		  register TYPE *end, 
		  register TYPE *result)
{
  register TYPE *temp = &value;

  for (; begin < end; begin++)
    if (!(*relation)(begin, temp))
      *result++ = *begin;

  return result;
}


TYPE *remove_duplicates(register TYPE *begin, 
			register TYPE *end)
{
  register TYPE *index = begin;
  register TYPE *m;

  for(;index < end; index++)
    if (position(*index, begin, index) != index)
      break;

  m = index;

  while (++index < end)
    if (position(*index, begin, m) == m)
      *m++ = *index;

  return m;
}
      

TYPE *remove_duplicates(int (*relation)(TYPE*, TYPE*), 
			register TYPE *begin, 
			register TYPE *end)
{
  register TYPE *index = begin;
  register TYPE *m;

  for(;index < end; index++)
    if (position(relation, *index, begin, index) != index)
      break;

  m = index;

  while (++index < end)
    if (!position(relation, *index, begin, m))
      *m++ = *index;

  return m;
}
      

TYPE *remove_duplicates_copy(register TYPE *begin, 
			     register TYPE *end, 
			     register TYPE *result)
{
  register TYPE *m = result;

  for (; begin < end; begin++)
    if (!position(*begin, result, m))
      *m++ = *begin;

  return m;
}
      

TYPE *remove_duplicates_copy(int (*relation)(TYPE*, TYPE*), 
			     register TYPE *begin, 
			     register TYPE *end, 
			     register TYPE *result)
{
  register TYPE *m = result;

  for (; begin < end; begin++)
    if (position(relation, *begin, result, m) == m)
      *m++ = *begin;

  return m;
}
      


void reverse(register TYPE *begin, 
	     register TYPE *end)
{
  while (begin < --end)
    {
      REGISTER TYPE temp = *begin;
      *begin++ = *end;
      *end = temp;
    }
}


void reverse_copy(register TYPE *begin, 
		  register TYPE *end, 
		  register TYPE *result)
{
  while (begin < end--)
    *result++ = *end;
}


TYPE *right_position(register int (*predicate)(TYPE*), 
		     register TYPE *begin, 
		     register TYPE *end)
{
  while (begin < end) 
    if ((*predicate)(--end))
      return end;

  return 0;
}


TYPE *right_position(REGISTER TYPE value, 
		     register TYPE *begin, 
		     register TYPE *end)
{
  while (begin < end) 
    if (*--end == value)
      return end;

  return 0;
}



TYPE *right_position(register int (*relation)(TYPE*, TYPE*), 
		     TYPE value, 
		     register TYPE *begin, 
		     register TYPE *end)
{
  register TYPE *temp = &value;

  while (begin < end) 
    if ((*relation)(--end, temp))
      return end;

  return 0;
}


void rotate(ptrdiff_t number, 
	    TYPE *begin, 
	    TYPE *end)
{
  if (begin >= end)
    return;

  number %= end - begin;

  if (number == 0)
    return;

  if (number < 0)
    number += (end - begin);

  reverse(begin, end);
  reverse(begin, begin + number);
  reverse(begin + number, end);
}


void rotate_copy(ptrdiff_t number, 
		 TYPE *begin, 
		 TYPE *end, 
		 TYPE *result)
{
  if (begin >= end)
    return;

  number %= end - begin;

  if (number == 0)
    {
      copy(begin, end, result);
      return;
    }

  if (number < 0)
    number += (end - begin);

  copy(end - number, end, result);
  copy(begin, end - number, result + number);
}


TYPE *search(register TYPE *begin1, 
	     TYPE *end1, 
	     register TYPE *begin2, 
	     TYPE *end2)
{
  register ptrdiff_t d1, d2, k;

  if (begin1 >= end1)
    return begin2;

  d1 = end1 - begin1;
  d2 = end2 - begin2;

  if (d2 < d1)
    return 0;

  k = 0;

  while (k < d1)
    if (begin1[k] == begin2[k])
      k++;
    else if (d1 == d2)
      return 0;
    else
      {
	k = 0;
	begin2++;
	d2--;
      }

  return begin2;
}


TYPE *search(register int (*relation)(TYPE*, TYPE*), 
	     register TYPE *begin1, 
	     TYPE *end1, 
	     register TYPE *begin2, 
	     TYPE *end2)
{
  register ptrdiff_t d1, d2, k;

  if (begin1 >= end1)
    return begin2;

  d1 = end1 - begin1;
  d2 = end2 - begin2;

  if (d2 < d1)
    return 0;

  k = 0;

  while (k < d1)
    if ((*relation)(begin1 + k, begin2 + k))
      k++;
    else if (d1 == d2)
      return 0;
    else
      {
	k = 0;
	begin2++;
	d2--;
      }

  return begin2;
}


static TYPE *ordered_partition(register TYPE *begin, 
			       register TYPE *end);

/* see sort for the implementation of ordered_partition */

void select(ptrdiff_t nth, 
	    TYPE *begin, 
	    TYPE *end)
{
  if (begin >= end || nth <= 0 || end - begin < nth)
    return;

  for (;;)
    if (end - begin < 6)
      {
	insertion_sort(begin, end);
	return;
      }
    else if (nth < 4)
      {
	while (nth--)
	  {
	    register TYPE *r = minimum(begin, end);
	    REGISTER TYPE temp = *begin;
	    *begin = *r;
	    *r = temp;
	  }    
	return;
      }
    else
      {
	TYPE *index = ordered_partition(begin, end);

	if (index - begin >= nth)
	  end = index;
	else
	  {
	    nth -= index - begin;
	    begin = index;
	  }
      }
}


static TYPE *ordered_partition(register int (*relation)(TYPE*, TYPE*), 
			       register TYPE *begin, 
			       register TYPE *end);

/* see sort for the implementation of ordered_partition */
void select(int (*relation)(TYPE*, TYPE*), 
	    ptrdiff_t nth, 
	    TYPE *begin, 
	    TYPE *end)
{
  if (begin >= end || nth <= 0 || end - begin < nth)
    return;

  for (;;)
    if (end - begin < 6)
      {
	insertion_sort(relation, begin, end);
	return;
      }
    else if (nth < 4)
      {
	while (nth--)
	  {
	    register TYPE *r = minimum(relation, begin, end);
	    REGISTER TYPE temp = *begin;
	    *begin++ = *r;
	    *r = temp;
	  }    
	return;
      }
    else
      {
	TYPE *index = ordered_partition(relation, begin, end);

	if (index - begin >= nth)
	  end = index;
	else
	  {
	    nth -= index - begin;
	    begin = index;
	  }
      }
}








TYPE *set_insert(TYPE value, 
		 TYPE *begin, 
		 TYPE *end)
{
  TYPE *index = binary_partition(value, begin, end);

  if (begin < index && !(*(index - 1) < value))
    return 0;

  else
    {
      copy(index, end, index + 1);
      *index = value;
      return index;
    }
}


TYPE *set_insert(int (*relation)(TYPE*, TYPE*), 
		 TYPE value, 
		 TYPE *begin, 
		 TYPE *end)
{
  TYPE *index = binary_partition(relation, value, begin, end);

  if (begin < index && !((*relation)(index - 1, &value) < 0))
    return 0;

  else
    {
      copy(index, end, index + 1);
      *index = value;
      return index;
    }
}


TYPE *set_union(register int (*relation)(TYPE*, TYPE*), 
		register TYPE *begin1, 
		register TYPE *end1, 
		register TYPE *begin2, 
		register TYPE *end2, 
		register TYPE *result)
{
  while (begin1 < end1 && begin2 < end2)
    {
      register int c = (*relation)(begin2, begin1);

      if (c < 0)
	*result++ = *begin2++;
      else 
	{
	  *result++ = *begin1++;

	  if (c == 0)
	    begin2++;
	}
    }

  while (begin1 < end1)
    *result++ = *begin1++;

  while (begin2 < end2)
    *result++ = *begin2++;

  return result;
}


TYPE *set_union(register TYPE *begin1, 
		register TYPE *end1, 
		register TYPE *begin2, 
		register TYPE *end2, 
		register TYPE *result)
{
  while (begin1 < end1 && begin2 < end2)
    if (*begin2 < *begin1)
      *result++ = *begin2++;
    else 
      {
	*result++ = *begin1++;
	if (!(*begin1 < *begin2))
	  begin2++;
      }

  while (begin1 < end1)
    *result++ = *begin1++;

  while (begin2 < end2)
    *result++ = *begin2++;

  return result;
}


void shuffle(register TYPE *begin, 
	     register TYPE *end)
{
  register TYPE *index = begin + 1;

  while (index < end)
    {
      register TYPE *r = random(begin, index + 1);
      REGISTER TYPE temp = *index;
      *index++ = *r;
      *r = temp;
    }    
}


void shuffle_copy(register TYPE *begin, 
		  register TYPE *end, 
		  register TYPE *result)
{
  register TYPE *result_end = result; 

  while (begin < end)
    {
      register TYPE *r = random(result, result_end + 1);
      *result_end++ = *r;
      *r = *begin++;
    }
}


static TYPE *ordered_partition(register TYPE *begin, 
			       register TYPE *end) 
{
  TYPE *old_begin = begin;

  REGISTER TYPE value = *random(begin, end);

  begin--;

  for (;;)
    {
      while (*++begin < value);

      while (value < *--end);

      if (begin < end)
	{
	  REGISTER TYPE temp = *begin;
	  *begin = *end;
	  *end = temp;
	}
      else
	return (begin == old_begin) ? begin + 1 : begin;
    }
}

static void quicksort_loop(register TYPE *begin, 
			   register TYPE *end)
{
  while (10 < end - begin)
    {
      register TYPE *index = ordered_partition(begin, end);

      if (end - index < index - begin)
	{
	  quicksort_loop(index, end);
	  end = index;
	}
      else
	{
	  quicksort_loop(begin, index);
	  begin = index;
	}
    }
}

void sort(TYPE *begin, 
	  TYPE *end)
{
  quicksort_loop(begin, end);
  insertion_sort(begin, end);
}


static TYPE *ordered_partition(register int (*relation)(TYPE*, TYPE*), 
			       register TYPE *begin, 
			       register TYPE *end)
{
  TYPE *old_begin = begin;

  TYPE value = *random(begin, end);

  register TYPE *temp = &value;

  begin--;

  for (;;)
    {
      while ((*relation)(++begin, temp) < 0);

      while ((*relation)(temp, --end) < 0);

      if (begin < end)
	{
	  REGISTER TYPE temp = *begin;
	  *begin = *end;
	  *end = temp;
	}
      else
	return (begin==old_begin) ? begin + 1 : begin;
    }
}

static void quicksort_loop(int (*relation)(TYPE*, TYPE*), 
			   TYPE *begin, 
			   TYPE *end)
{
  while (10 < end - begin)
    {
      TYPE *index = ordered_partition(relation, begin, end);

      if (end - index < index - begin)
	{
	  quicksort_loop(relation, index, end);
	  end = index;
	}
      else
	{
	  quicksort_loop(relation, begin, index);
	  begin = index;
	}
    }
}

void sort(int (*relation)(TYPE*, TYPE*), 
	  TYPE *begin, 
	  TYPE *end)
{
  quicksort_loop(relation, begin, end);
  insertion_sort(relation, begin, end);
}



TYPE *stable_partition(int (*predicate)(TYPE*),  
		       TYPE *begin, 
		       TYPE *end) 
{
  if (end - begin > 1)
    {
      TYPE *middle = begin + ((end - begin) >> 1);
      TYPE *first_half = stable_partition(predicate, begin, middle);
      TYPE *second_half = stable_partition(predicate, middle, end);

      rotate(first_half - middle, first_half, second_half);

      return first_half + (second_half - middle);
    }

  else if (begin < end)

    return (*predicate)(begin) ? begin + 1 : begin;

  else

    return end;
}


TYPE *stable_partition(TYPE value,  
		       TYPE *begin, 
		       TYPE *end) 
{
  if (end - begin > 1)
    {
      TYPE *middle = begin + ((end - begin) >> 1);
      TYPE *first_half = stable_partition(value, begin, middle);
      TYPE *second_half = stable_partition(value, middle, end);

      rotate(first_half - middle, first_half, second_half);

      return first_half + (second_half - middle);
    }

  else if (begin < end)

    return *begin == value ? begin + 1 : begin;

  else

    return end;
}


TYPE *stable_partition(int (*relation)(TYPE*, TYPE*), 
		       TYPE value,  
		       TYPE *begin, 
		       TYPE *end) 
{
  if (end - begin > 1)
    {
      TYPE *middle = begin + ((end - begin) >> 1);
      TYPE *first_half = stable_partition(relation, value, begin, middle);
      TYPE *second_half = stable_partition(relation, value, middle, end);

      rotate(first_half - middle, first_half, second_half);

      return first_half + (second_half - middle);
    }

  else if (begin < end)

    return (*relation)(begin, &value) ? begin + 1 : begin;

  else

    return end;
}


TYPE *stable_partition_copy(int (*predicate)(TYPE*), 
			    TYPE *begin, 
			    TYPE *end, 
			    TYPE *result)
{

  TYPE *m = partition_copy(predicate, begin, end, result);
  reverse(m, result + (end - begin));
  return m;
}


TYPE *stable_partition_copy(TYPE value, 
			    TYPE *begin, 
			    TYPE *end, 
			    TYPE *result)
{
  TYPE *m = partition_copy(value, begin, end, result);
  reverse(m, result + (end - begin));
  return m;
}


TYPE *stable_partition_copy(int (*relation)(TYPE*, TYPE*), 
			    TYPE value, 
			    TYPE *begin, 
			    TYPE *end, 
			    TYPE *result)
{
  TYPE *m = partition_copy(relation, value, begin, end, result);
  reverse(m, result + (end - begin));
  return m;
}


TYPE *stable_remove(register int (*predicate)(TYPE*), 
		    register TYPE *begin, 
		    register TYPE *end)
{
  register TYPE *m;

  while (begin < end && !(*predicate)(begin))
    begin++;

  if (begin >= end)
    return end;

  m = begin;

  while (++begin < end)
    if (!(*predicate)(begin))
      *m++ = *begin;

  return m;
}
  

TYPE *stable_remove(REGISTER TYPE value, 
		    register TYPE *begin, 
		    register TYPE *end)
{
  register TYPE *m;

  while (begin < end && !(*begin == value))
    begin++;

  if (begin >= end)
    return end;

  m = begin;

  while (++begin < end)
    if (!(*begin == value))
      *m++ = *begin;

  return m;
}


TYPE *stable_remove(register int (*relation)(TYPE*, TYPE*), 
		    TYPE value, 
		    register TYPE *begin, 
		    register TYPE *end)
{
  register TYPE *temp = &value;
  register TYPE *m;

  while (begin < end && !(*relation)(begin, temp))
    begin++;

  if (begin >= end)
    return end;

  m = begin;

  while (++begin < end)
    if (!(*relation)(begin, temp))
      *m++ = *begin;

  return m;
}


TYPE *stable_remove_copy(int (*predicate)(TYPE*), 
			 TYPE *begin, 
			 TYPE *end,  
			 TYPE *result)
{
  return remove_copy(predicate, begin, end, result);
}


TYPE *stable_remove_copy(TYPE value, 
			 TYPE *begin, 
			 TYPE *end, 
			 TYPE *result)
{
  return remove_copy(value, begin, end, result);
}


TYPE *stable_remove_copy(int (*relation)(TYPE*, TYPE*), 
			 TYPE value, 
			 TYPE *begin, 
			 TYPE *end, 
			 TYPE *result)
{
  return remove_copy(relation, value, begin, end, result);
}


#include <malloc.h>

void stable_sort(int (*relation)(TYPE*, TYPE*), 
		 TYPE *begin, 
		 TYPE *end)
{
  TYPE *index = (TYPE*) malloc((unsigned)(sizeof(TYPE) * (end - begin)));

  if (index == 0)
    {
      insertion_sort(relation, begin, end);
      return;
    }

  if (merge_sort(relation, begin, end, index) == index)
    {
      TYPE *i = index;

      while (begin < end)
	*begin++ = *i++;
    }

  free((char*)index);

}


#include <malloc.h>

void stable_sort(TYPE *begin, 
		 TYPE *end)
{
  TYPE *index = (TYPE*) malloc((unsigned)(sizeof(TYPE) * (end - begin)));

  if (index == 0)
    {
      insertion_sort(begin, end);
      return;
    }

  if (merge_sort(begin, end, index) == index)
    {
      TYPE *i = index;

      while (begin < end)
	*begin++ = *i++;
    }

  free((char*)index);
}


void substitute(REGISTER TYPE value, 
		REGISTER TYPE new_value, 
		register TYPE *begin, 
		register TYPE *end)
{
  for (; begin < end; begin++)
    if (*begin == value)
      *begin = new_value;
}


void substitute(register int (*relation)(TYPE*, TYPE*), 
		TYPE value, 
		REGISTER TYPE new_value, 
		register TYPE *begin, 
		register TYPE *end)
{
  register TYPE *temp = &value;

  for (; begin < end; begin++)
    if ((*relation)(begin, temp))
      *begin = new_value;
}


void substitute_copy(REGISTER TYPE value, 
		     REGISTER TYPE new_value, 
		     register TYPE *begin, 
		     register TYPE *end, 
		     register TYPE *result)
{
  for (; begin < end; begin++)
    *result++ = (*begin == value ? new_value : *begin);
}


void substitute_copy(register int (*relation)(TYPE*, TYPE*), 
		     TYPE value, 
		     REGISTER TYPE new_value, 
		     register TYPE *begin, 
		     register TYPE *end, 
		     register TYPE *result)
{
  register TYPE *temp = &value;

  for (; begin < end; begin++)
    *result++ = ((*relation)(begin, temp) ? new_value : *begin);
}


TYPE *symmetric_difference(register int (*relation)(TYPE*, TYPE*), 
			   register TYPE *begin1, 
			   register TYPE *end1, 
			   register TYPE *begin2, 
			   register TYPE *end2, 
			   register TYPE *result)
{
  while (begin1 < end1 && begin2 < end2)
    {
      register int c = (*relation)(begin2, begin1);

      if (0 < c)
	*result++ = *begin1++;
      else if (c < 0)
	*result++ = *begin2++;
      else 
	begin1++, begin2++;
    }

  while (begin1 < end1)
    *result++ = *begin1++;

  while (begin2 < end2)
    *result++ = *begin2++;

  return result;
}


TYPE *symmetric_difference(register TYPE *begin1, 
			   register TYPE *end1, 
			   register TYPE *begin2, 
			   register TYPE *end2, 
			   register TYPE *result)
{
  while (begin1 < end1 && begin2 < end2)
    if (*begin1 < *begin2)
      *result++ = *begin1++;
    else if (*begin2 < *begin1)
      *result++ = *begin2++;
    else 
      begin1++ ,begin2++;

  while (begin1 < end1)
    *result++ = *begin1++;

  while (begin2 < end2)
    *result++ = *begin2++;

  return result;
}


TYPE *unique(register TYPE *begin, 
	     register TYPE *end)
{
  register TYPE *m;

  if (begin >= end)
    return begin;

  while (++begin < end)
    if (*(begin - 1) == *begin)
      break;

  if (begin == end)
    return begin;

  m = begin - 1;

  while (++begin < end)
    if (!(*m == *begin))
      *++m = *begin;

  return m + 1;
}
      

TYPE *unique(register int (*relation)(TYPE*, TYPE*), 
	     register TYPE *begin, 
	     register TYPE *end)
{
  register TYPE *m;

  if (begin >= end)
    return begin;

  while (++begin < end)
    if (!(*relation)(begin - 1, begin))
      break;

  if (begin == end)
    return begin;

  m = begin - 1;

  while (++begin < end)
    if ((*relation)(m, begin))
      *++m = *begin;

  return m + 1;
}
      

TYPE *unique_copy(register TYPE *begin, 
		  register TYPE *end, 
		  register TYPE *result)
{
  if (begin >= end)
    return result;

  *result = *begin; 

  while (++begin < end)
    if (!(*result == *begin))
      *++result = *begin;

  return result + 1;
}
      

TYPE *unique_copy(register int (*relation)(TYPE*, TYPE*), 
		  register TYPE *begin, 
		  register TYPE *end, 
		  register TYPE *result)
{
  if (begin >= end)
    return result;

  *result = *begin; 

  while (++begin < end)
    if ((*relation)(result, begin))
      *++result = *begin;

  return result + 1;
}

