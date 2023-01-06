void quicksort(int l, int u)
{ int i, m;
  if (l >= u) return;
  swap(l, randint(l, u));
  m = l;
  for (i = l+1; i <= u; i++)
  if (x[i] < x[l])
  swap(++m, i);
  swap(l, m);
  quicksort(l, m-1);
  quicksort(m+1, u);
}

/* C.A.R. Hoare, 1959
 * Code from the book Programming Pearls by Jon Bentley, 1999
 */
