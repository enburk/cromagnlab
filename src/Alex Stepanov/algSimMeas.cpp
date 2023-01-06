// Alexander Stepanov: Algorithmic simulation and measurement codes, 1993

template <class Iterator>
void slowSort(Iterator first, Iterator last)
{
    while (nextPermutation(first, last));
}
 
template <class Iterator>
void selectionSort(Iterator first, Iterator last)
{
    while (first != last) {
        swap(*first, *minElement(first, last));
        first++;
    }
}

template <class Iterator>
void probabilisticSort(Iterator first, Iterator last)
{
    while (!isSorted(first, last))
        randomShuffle(first, last);
}

// this is Knuth's version of bubble-sort

template <class Iterator>
void bubbleSort(Iterator first, Iterator last)
{
    if (last - first < 2) return;
    do {
        Iterator i = first;
        Iterator bound = first;
        
        while (last > ++i) 
            if (*(i - 1) - *i > 0) {
                swap(*(i - 1), *i);
                bound = i;
            }
        last = bound;
    } while (last > first);
}

// shell sort

template <class Iterator, class T>
static void sift(Iterator first, Iterator i, ptrdiff_t step, T value)
{
    while (i - first >= step && value - *(i - step) < 0) {
        *i = *(i - step);
        i -= step;
    }
    *i = value;
}

template <class Iterator>
static void siftAll(Iterator first, Iterator last, ptrdiff_t step)
{
    Iterator next = first + (step - 1);
    
    while (++next < last)
        sift(first, next, step, *next);
}

template <class Iterator, class StepInitializer, class StepGenerator>
static void shellSortLoop(Iterator first, Iterator last, 
                          StepInitializer init, StepGenerator gen)
{
    if (last - first < 2) return;

    for (ptrdiff_t step = init(last - first); step > 1; step = gen(step))
        siftAll(first, last, step);

    siftAll(first, last, 1);
}

ptrdiff_t gonnetIncrements(ptrdiff_t n)
{
    return ptrdiff_t(floor(n * .45454));
}

template <class Iterator>
static void shellSortGonnet(Iterator first, Iterator last)
{
    shellSortLoop(first, last, gonnetIncrements, gonnetIncrements);
}

ptrdiff_t knuthInitializer(ptrdiff_t n)
{
    ptrdiff_t i = 1;
    while (i < n)
        i = 3 * i + 1;
    i = (i - 1) / 3;
    i = (i - 1) / 3;
    return i;
}

ptrdiff_t knuthStep(ptrdiff_t n)
{
    return (n - 1)/3;
}

template <class Iterator>
static void shellSortKnuth(Iterator first, Iterator last)
{
    shellSortLoop(first, last, knuthInitializer, knuthStep);
}

ptrdiff_t PAPStep(ptrdiff_t n)
{
    return floor(n / 2.7182818285);
}

ptrdiff_t PAPInitializer(ptrdiff_t n)
{
    return ptrdiff_t(floor(exp(floor(log(n)))));
}

template <class Iterator>
static void shellSortPAP(Iterator first, Iterator last)
{
    shellSortLoop(first, last, PAPInitializer, PAPStep);
}

////

template <class Iterator>
int nextPermutation(Iterator first, Iterator last)
{
    if (last - first < 2) return 0;

    Iterator i = last - 2;
    
    while (1) {
        if (*i < *(i + 1)) {
            Iterator j = last - 1;
            while (*i >= *j) j--;
            swap(*i, *j);
            reverse(i + 1, last);
            return 1;
        }
        if (i == first) {
            reverse(first, last);
            return 0;
        }
        i--;
    }
}

template <class Iterator, class Predicate>
Iterator partition(Iterator first, Iterator last, Predicate pred)
{
    while (1) {
        while (1) 
            if (first == last)
                return first;
            else if (pred(*first))
                first++;
            else
                break;
        last--;
        while (1)
            if (first == last)
                return first;
            else if (!pred(*last))
                last--;
            else
                break;
        swap(*first, *last);
        first++;
    }
}

template <class Iterator>
void inplaceRotate(Iterator first, Iterator middle, Iterator last)
{
    if (first == middle || middle == last) return;
    reverse(first, middle);
    reverse(middle, last);
    reverse(first, last);
}

template <class Iterator>
int isSorted(Iterator first, Iterator last)
{
    if (first == last) return 1;
    Iterator next = first;
    next++;
    while (!(next == last)) {
        if (*first > *next)
            return 0;
        next++;
        first++;
    }
    return 1;
} 