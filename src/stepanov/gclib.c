/* Alexander Stepanov: gclib, AT&T Bell Laboratories, 1987 */

/*#####*/

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

/*#####*/

TYPE *minimum(register TYPE *begin, 
              register TYPE *end)
{
  register TYPE *index = begin;

  while (++begin < end)
    if (*begin < *index)
      index = begin;

  return index;
}

/*#####*/

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

/*#####*/

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

/*#####*/

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

/*#####*/

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

/*#####*/

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
      
/*#####*/

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
      
/*#####*/

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

/*#####*/

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

/*#####*/

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

/*#####*/

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

/*#####*/

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

/*#####*/

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

/*#####*/

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

/*#####*/

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

/*#####*/

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
  
/*#####*/

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

/*#####*/

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

/*#####*/

void substitute(REGISTER TYPE value, 
                REGISTER TYPE new_value, 
                register TYPE *begin, 
                register TYPE *end)
{
  for (; begin < end; begin++)
    if (*begin == value)
      *begin = new_value;
}

/*#####*/

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

/*#####*/

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
      
/*#####*/

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
      
/*#####*/

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
