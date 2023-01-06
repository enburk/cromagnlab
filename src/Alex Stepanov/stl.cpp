template <class InputIterator, class OutputIterator, class T>
OutputIterator __unique_copy(InputIterator first, InputIterator last,
                             OutputIterator result, T*) {
    T value = *first;
    *result = value;
    while (++first != last)
        if (value != *first) {
            value = *first;
            *++result = value;
        }
    return ++result;
}

template <class ForwardIterator, class Distance>
void __rotate(ForwardIterator first, ForwardIterator middle,
              ForwardIterator last, Distance*, forward_iterator_tag) {
    for (ForwardIterator i = middle; ;) {
        iter_swap(first++, i++);
        if (first == middle) {
            if (i == last) return;
            middle = i;
        } else if (i == last)
            i = middle;
    }
}

template <class BidirectionalIterator, class Distance>
void __rotate(BidirectionalIterator first, BidirectionalIterator middle,
              BidirectionalIterator last, Distance*,
              bidirectional_iterator_tag) {
    reverse(first, middle);
    reverse(middle, last);
    reverse(first, last);
}

template <class EuclideanRingElement>
EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n)
{
    while (n != 0) {
        EuclideanRingElement t = m % n;
        m = n;
        n = t;
    }
    return m;
}

template <class RandomAccessIterator, class Distance, class T>
void __rotate_cycle(RandomAccessIterator first, RandomAccessIterator last,
                    RandomAccessIterator initial, Distance shift, T*) {
    T value = *initial;
    RandomAccessIterator ptr1 = initial;
    RandomAccessIterator ptr2 = ptr1 + shift;
    while (ptr2 != initial) {
        *ptr1 = *ptr2;
        ptr1 = ptr2;
        if (last - ptr2 > shift)
            ptr2 += shift;
        else
            ptr2 = first + (shift - (last - ptr2));
    }
    *ptr1 = value;
}

template <class RandomAccessIterator, class Distance>
void __rotate(RandomAccessIterator first, RandomAccessIterator middle,
              RandomAccessIterator last, Distance*,
              random_access_iterator_tag) {
    Distance n = __gcd(last - first, middle - first);
    while (n--)
        __rotate_cycle(first, last, first + n, middle - first,
                       value_type(first));
}

template <class ForwardIterator>
inline void rotate(ForwardIterator first, ForwardIterator middle,
                   ForwardIterator last) {
    if (first == middle || middle == last) return;
    __rotate(first, middle, last, distance_type(first),
             iterator_category(first));
}

template <class BidirectionalIterator, class Predicate>
BidirectionalIterator partition(BidirectionalIterator first,
                                BidirectionalIterator last, Predicate pred) {
    while (true) {
        while (true)
            if (first == last)
                return first;
            else if (pred(*first))
                ++first;
            else
                break;
        --last;
        while (true)
            if (first == last)
                return first;
            else if (!pred(*last))
                --last;
            else
                break;
        iter_swap(first, last);
        ++first;
    }
}

template <class ForwardIterator, class Pointer, class Predicate,
          class Distance, class T>
ForwardIterator __stable_partition_adaptive(ForwardIterator first,
                                            ForwardIterator last,
                                            Predicate pred, Distance len,
                                            Pointer buffer,
                                            Distance buffer_size,
                                            Distance& fill_pointer, T*) {
    if (len <= buffer_size) {
        len = 0;
        ForwardIterator result1 = first;
        Pointer result2 = buffer;
        while (first != last && len < fill_pointer)
            if (pred(*first))
                *result1++ = *first++;
            else {
                *result2++ = *first++;
                ++len;
            }
        if (first != last) {
            raw_storage_iterator<Pointer, T> result3 = result2;
            while (first != last)           
                if (pred(*first))
                    *result1++ = *first++;
                else {
                    *result3++ = *first++;
                    ++len;
                }
            fill_pointer = len;
        }
        copy(buffer, buffer + len, result1);
        return result1;
    }
    ForwardIterator middle = first;
    advance(middle, len / 2);
    ForwardIterator first_cut = __stable_partition_adaptive
        (first, middle, pred, len / 2, buffer, buffer_size, fill_pointer, 
         (T*)0);
    ForwardIterator second_cut = __stable_partition_adaptive
        (middle, last, pred, len - len / 2, buffer, buffer_size, 
         fill_pointer, (T*)0);
    rotate(first_cut, middle, second_cut);
    len = 0;
    distance(middle, second_cut, len);
    advance(first_cut, len);
    return first_cut;
}

template <class RandomAccessIterator>
void __inplace_stable_sort(RandomAccessIterator first,
                           RandomAccessIterator last) {
    if (last - first < 15) {
        __insertion_sort(first, last);
        return;
    }
    RandomAccessIterator middle = first + (last - first) / 2;
    __inplace_stable_sort(first, middle);
    __inplace_stable_sort(middle, last);
    __merge_without_buffer(first, middle, last, middle - first, last - middle);
}

template <class RandomAccessIterator, class T>
void __partial_sort(RandomAccessIterator first, RandomAccessIterator middle,
                    RandomAccessIterator last, T*) {
    make_heap(first, middle);
    for (RandomAccessIterator i = middle; i < last; ++i)
        if (*i < *first) 
          __pop_heap(first, middle, i, T(*i), distance_type(first));
    sort_heap(first, middle);
}

template <class RandomAccessIterator, class T>
void __nth_element(RandomAccessIterator first, RandomAccessIterator nth,
                   RandomAccessIterator last, T*) {
    while (last - first > 3) {
        RandomAccessIterator cut = __unguarded_partition
            (first, last, T(__median(*first, *(first + (last - first)/2),
                                     *(last - 1))));
        if (cut <= nth)
            first = cut;
        else 
            last = cut;
    }
    __insertion_sort(first, last);
}

template <class ForwardIterator, class T, class Distance>
ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last,
                              const T& value, Distance*,
                              forward_iterator_tag) {
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIterator middle;

    while (len > 0) {
        half = len / 2;
        middle = first;
        advance(middle, half);
        if (*middle < value) {
            first = middle;
            ++first;
            len = len - half - 1;
        } else
            len = half;
    }
    return first;
}

template <class RandomAccessIterator, class T, class Distance>
RandomAccessIterator __lower_bound(RandomAccessIterator first,
                                   RandomAccessIterator last, const T& value,
                                   Distance*, random_access_iterator_tag) {
    Distance len = last - first;
    Distance half;
    RandomAccessIterator middle;

    while (len > 0) {
        half = len / 2;
        middle = first + half;
        if (*middle < value) {
            first = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return first;
}

template <class ForwardIterator, class T, class Distance>
ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last,
                              const T& value, Distance*,
                              forward_iterator_tag) {
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIterator middle;

    while (len > 0) {
        half = len / 2;
        middle = first;
        advance(middle, half);
        if (value < *middle)
            len = half;
        else {
            first = middle;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}

template <class RandomAccessIterator, class T, class Distance>
RandomAccessIterator __upper_bound(RandomAccessIterator first,
                                   RandomAccessIterator last, const T& value,
                                   Distance*, random_access_iterator_tag) {
    Distance len = last - first;
    Distance half;
    RandomAccessIterator middle;

    while (len > 0) {
        half = len / 2;
        middle = first + half;
        if (value < *middle)
            len = half;
        else {
            first = middle + 1;
            len = len - half - 1;
        }
    }
    return first;
}

template <class RandomAccessIterator, class T, class Distance>
pair<RandomAccessIterator, RandomAccessIterator>
__equal_range(RandomAccessIterator first, RandomAccessIterator last,
              const T& value, Distance*, random_access_iterator_tag) {
    Distance len = last - first;
    Distance half;
    RandomAccessIterator middle, left, right;

    while (len > 0) {
        half = len / 2;
        middle = first + half;
        if (*middle < value) {
            first = middle + 1;
            len = len - half - 1;
        } else if (value < *middle)
            len = half;
        else {
            left = lower_bound(first, middle, value);
            right = upper_bound(++middle, first + len, value);
            return pair<RandomAccessIterator, RandomAccessIterator>(left,
                                                                    right);
        }
    }
    return pair<RandomAccessIterator, RandomAccessIterator>(first, first);
}


template <class BidirectionalIterator, class Distance>
void __merge_without_buffer(BidirectionalIterator first,
                            BidirectionalIterator middle,
                            BidirectionalIterator last,
                            Distance len1, Distance len2) {
    if (len1 == 0 || len2 == 0) return;
    if (len1 + len2 == 2) {
        if (*middle < *first) iter_swap(first, middle);
        return;
    }
    BidirectionalIterator first_cut = first;
    BidirectionalIterator second_cut = middle;
    Distance len11 = 0;
    Distance len22 = 0;
    if (len1 > len2) {
        len11 = len1 / 2;
        advance(first_cut, len11);
        second_cut = lower_bound(middle, last, *first_cut);
        distance(middle, second_cut, len22);
    } else {
        len22 = len2 / 2;
        advance(second_cut, len22);
        first_cut = upper_bound(first, middle, *second_cut);
        distance(first, first_cut, len11);
    }
    rotate(first_cut, middle, second_cut);
    BidirectionalIterator new_middle = first_cut;
    advance(new_middle, len22);
    __merge_without_buffer(first, first_cut, new_middle, len11, len22);
    __merge_without_buffer(new_middle, second_cut, last, len1 - len11,
                           len2 - len22);
}

template <class InputIterator, class OutputIterator>
OutputIterator __borland_bugfix_copy(InputIterator first, InputIterator last,
                    OutputIterator result) {
// this is used in __rotate_adaptive to work around some obscure Borland
// bug. It is the same as copy, but with a different (and appropriate) name.
    while (first != last) *result++ = *first++;
    return result;
}

template <class BidirectionalIterator1, class BidirectionalIterator2,
          class Distance>
BidirectionalIterator1 __rotate_adaptive(BidirectionalIterator1 first,
                                         BidirectionalIterator1 middle,
                                         BidirectionalIterator1 last,
                                         Distance len1, Distance len2,
                                         BidirectionalIterator2 buffer,
                                         Distance buffer_size) {
    BidirectionalIterator2 buffer_end;
    if (len1 > len2 && len2 <= buffer_size) {
        buffer_end = __borland_bugfix_copy(middle, last, buffer);
        copy_backward(first, middle, last);
        return copy(buffer, buffer_end, first);
    } else if (len1 <= buffer_size) {
        buffer_end = __borland_bugfix_copy(first, middle, buffer);
        copy(middle, last, first);
        return copy_backward(buffer, buffer_end, last);
    } else  {
        rotate(first, middle, last);
        advance(first, len2);
        return first;
    }
}

template <class BidirectionalIterator1, class BidirectionalIterator2,
          class BidirectionalIterator3>
BidirectionalIterator3 __merge_backward(BidirectionalIterator1 first1,
                                        BidirectionalIterator1 last1,
                                        BidirectionalIterator2 first2,
                                        BidirectionalIterator2 last2,
                                        BidirectionalIterator3 result) {
    if (first1 == last1) return copy_backward(first2, last2, result);
    if (first2 == last2) return copy_backward(first1, last1, result);
    --last1;
    --last2;
    while (true) {
        if (*last2 < *last1) {
            *--result = *last1;
            if (first1 == last1) return copy_backward(first2, ++last2, result);
            --last1;
        } else {
            *--result = *last2;
            if (first2 == last2) return copy_backward(first1, ++last1, result);
            --last2;
        }
    }
}

template <class InputIterator1, class InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, InputIterator2 last2) {
    while (first1 != last1 && first2 != last2)
        if (*first2 < *first1)
            return false;
        else if(*first1 < *first2) 
            ++first1;
        else
            ++first1, ++first2;

    return first2 == last2;
}

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, InputIterator2 last2,
                         OutputIterator result) {
    while (first1 != last1 && first2 != last2)
        if (*first1 < *first2)
            *result++ = *first1++;
        else if (*first2 < *first1)
            *result++ = *first2++;
        else {
            *result++ = *first1++;
            first2++;
        }
    return copy(first2, last2, copy(first1, last1, result));
}

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result) {
    while (first1 != last1 && first2 != last2)
        if (*first1 < *first2)
            ++first1;
        else if (*first2 < *first1)
            ++first2;
        else {
            *result++ = *first1++;
            ++first2;
        }
    return result;
}

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2,
                              OutputIterator result) {
    while (first1 != last1 && first2 != last2)
        if (*first1 < *first2)
            *result++ = *first1++;
        else if (*first2 < *first1)
            ++first2;
        else {
            ++first1;
            ++first2;
        }
    return copy(first1, last1, result);
}

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first1,
                                        InputIterator1 last1,
                                        InputIterator2 first2,
                                        InputIterator2 last2,
                                        OutputIterator result) {
    while (first1 != last1 && first2 != last2)
        if (*first1 < *first2)
            *result++ = *first1++;
        else if (*first2 < *first1)
            *result++ = *first2++;
        else {
            ++first1;
            ++first2;
        }
    return copy(first2, last2, copy(first1, last1, result));
}

template <class ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
    if (first == last) return first;
    ForwardIterator result = first;
    while (++first != last) 
        if (*result < *first) result = first;
    return result;
}

template <class BidirectionalIterator>
bool next_permutation(BidirectionalIterator first,
                      BidirectionalIterator last) {
    if (first == last) return false;
    BidirectionalIterator i = first;
    ++i;
    if (i == last) return false;
    i = last;
    --i;

    for(;;) {
        BidirectionalIterator ii = i--;
        if (*i < *ii) {
            BidirectionalIterator j = last;
            while (!(*i < *--j));
            iter_swap(i, j);
            reverse(ii, last);
            return true;
        }
        if (i == first) {
            reverse(first, last);
            return false;
        }
    }
}


template <class InputIterator, class OutputIterator, class T>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last, 
                                     OutputIterator result, T*) {
    T value = *first;
    while (++first != last) {
        T tmp = *first;
        *++result = tmp - value;
        value = tmp;
    }
    return ++result;
}

template <class InputIterator, class OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, 
                                   OutputIterator result) {
    if (first == last) return result;
    *result = *first;
    return __adjacent_difference(first, last, result, value_type(first));
}


template <class ForwardIterator, class T>
void iota(ForwardIterator first, ForwardIterator last, T value) {
    while (first != last) *first++ = value++;
}



// Sean Parent, "Generic Programming", Pacific++ talk, 2018

template <
class I, // I models ForwardIterator
class T  // T is value_type(I)
>
auto lower_bound (I f, I l, const T& v) -> I
{
    while (f != l) {
        auto m = next(f, distance(f, l) / 2);
        if (*m < v) f = next(m);
        else l = m;
    }
    return f;
}


// Sean Parent - C++ Seasoning [GN2013]
// Sean Parent - Better Code: Data Structures [CppCon 2015]

template <
typename I, // random iterator
typename P  // unary predicate
>
auto stable_partition (I f, I l, P p) -> I
{
    auto n = l - f;
    if (n == 0) return f;
    if (n == 1) return f + p (*f);
    
    auto m = f + (n / 2);

    return rotate
    (
        stable_partition(f, m, p), m,
        stable_partition(m, l, p)
    );
}

// Sean Parent, "Generic Programming", Pacific++ talk, 2018
// Elements of Programming, Alexander Stepanov, Paul McJones
template <typename I, typename P>
    requires(Mutable(I) && ForwardIterator(I) &&
        UnaryPredicate(P) && ValueType(I) == Domain(P))
I partition_semistable(I f, I l, P p) {
    // Precondition: mutable_bounded_range(f, l)
    I i = find_if(f, l, p);
    if (i == l) return i;
    I j = successor(i);
    while (true) {
        j = find_if_not(j, l, p);
        if (j == l) return i;
        swap_step(i, j);
    }
}

// end of algo.h

template <class T>
inline T* allocate(ptrdiff_t size, T*) {
    set_new_handler(0);
    T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
    if (tmp == 0) {
        cerr << "out of memory" << endl; 
        exit(1);
    }
    return tmp;
}


template <class RandomAccessIterator, class Distance, class T>
void __push_heap(RandomAccessIterator first, Distance holeIndex,
                 Distance topIndex, T value) {
    Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && *(first + parent) < value) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }    
    *(first + holeIndex) = value;
}

template <class RandomAccessIterator, class Distance, class T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex,
                   Distance len, T value) {
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (*(first + secondChild) < *(first + (secondChild - 1)))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, value);
} 

template <class RandomAccessIterator, class T, class Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*,
                 Distance*) {
    if (last - first < 2) return;
    Distance len = last - first;
    Distance parent = (len - 2)/2;
    
    while (true) {
        __adjust_heap(first, parent, len, T(*(first + parent)));
        if (parent == 0) return;
        parent--;
    }
} 

template <class OutputIterator, class T>
class raw_storage_iterator : public output_iterator {
protected:
    OutputIterator iter;
public:
    raw_storage_iterator(OutputIterator x) : iter(x) {}
    raw_storage_iterator<OutputIterator, T>& operator*() { return *this; }
    raw_storage_iterator<OutputIterator, T>& operator=(const T& element) {
        construct(iter, element);
        return *this;
    }        
    raw_storage_iterator<OutputIterator, T>& operator++() {
        ++iter;
        return *this;
    }
    raw_storage_iterator<OutputIterator, T> operator++(int) {
        raw_storage_iterator<OutputIterator, T> tmp = *this;
        ++iter;
        return tmp;
    }
}; 

template <class T, class Distance> // Distance == ptrdiff_t
class istream_iterator : public input_iterator<T, Distance> {
friend bool operator==(const istream_iterator<T, Distance>& x,
                       const istream_iterator<T, Distance>& y);
protected:
    istream* stream;
    T value;
    bool end_marker;
    void read() {
        end_marker = (*stream) ? true : false;
        if (end_marker) *stream >> value;
        end_marker = (*stream) ? true : false;
    }
public:
    istream_iterator() : stream(&cin), end_marker(false) {}
    istream_iterator(istream& s) : stream(&s) { read(); }
    const T& operator*() const { return value; }
    istream_iterator<T, Distance>& operator++() { 
        read(); 
        return *this;
    }
    istream_iterator<T, Distance> operator++(int)  {
        istream_iterator<T, Distance> tmp = *this;
        read();
        return tmp;
    }
};

template <class T, class Distance>
bool operator==(const istream_iterator<T, Distance>& x,
                const istream_iterator<T, Distance>& y) {
    return x.stream == y.stream && x.end_marker == y.end_marker ||
        x.end_marker == false && y.end_marker == false;
}

template <class T>
class ostream_iterator : public output_iterator {
protected:
    ostream* stream;
    char* string;
public:
    ostream_iterator(ostream& s) : stream(&s), string(0) {}
    ostream_iterator(ostream& s, char* c) : stream(&s), string(c)  {}
    ostream_iterator<T>& operator=(const T& value) { 
        *stream << value;
        if (string) *stream << string;
        return *this;
    }
    ostream_iterator<T>& operator*() { return *this; }
    ostream_iterator<T>& operator++() { return *this; } 
    ostream_iterator<T>& operator++(int) { return *this; } 
};


template <class T>
class list { 

   // ...
   
protected:
    void transfer(iterator position, iterator first, iterator last) {
        (*(link_type((*last.node).prev))).next = position.node;
        (*(link_type((*first.node).prev))).next = last.node;
        (*(link_type((*position.node).prev))).next = first.node;  
        link_type tmp = link_type((*position.node).prev);
        (*position.node).prev = (*last.node).prev;
        (*last.node).prev = (*first.node).prev; 
        (*first.node).prev = tmp;
    }
public:
    void splice(iterator position, list<T>& x) {
        if (!x.empty()) {
            transfer(position, x.begin(), x.end());
            length += x.length;
            x.length = 0;
        }
    }
    void splice(iterator position, list<T>& x, iterator i) {
        iterator j = i;
        if (position == i || position == ++j) return;
        transfer(position, i, j);
        ++length;
        --x.length;
    }
    void splice(iterator position, list<T>& x, iterator first, iterator last) {
        if (first != last) {
            if (&x != this) {
                difference_type n = 0;
                    distance(first, last, n);
                    x.length -= n;
                    length += n;
            }
            transfer(position, first, last);
        }
    }
};

template <class T>
void list<T>::unique() {
    iterator first = begin();
    iterator last = end();
    if (first == last) return;
    iterator next = first;
    while (++next != last) {
        if (*first == *next)
            erase(next);
        else
            first = next;
        next = first;
    }
}

template <class T>
void list<T>::merge(list<T>& x) {
    iterator first1 = begin();
    iterator last1 = end();
    iterator first2 = x.begin();
    iterator last2 = x.end();
    while (first1 != last1 && first2 != last2)
        if (*first2 < *first1) {
            iterator next = first2;
            transfer(first1, first2, ++next);
            first2 = next;
        } else
            ++first1;
    if (first2 != last2) transfer(last1, first2, last2);
    length += x.length;
    x.length= 0;
}

template <class T>
void list<T>::reverse() {
    if (size() < 2) return;
    for (iterator first = ++begin(); first != end();) {
        iterator old = first++;
        transfer(begin(), old, first);
    }
}    

template <class T>
void list<T>::sort() {
    if (size() < 2) return;
    list<T> carry;
    list<T> counter[64];
    int fill = 0;
    while (!empty()) {
        carry.splice(carry.begin(), *this, begin());
        int i = 0;
        while(i < fill && !counter[i].empty()) {
            counter[i].merge(carry);
            carry.swap(counter[i++]);
        }
        carry.swap(counter[i]);         
        if (i == fill) ++fill;
    } 

    for (int i = 1; i < fill; ++i) counter[i].merge(counter[i-1]);
    swap(counter[fill-1]);
}



template <class T>
vector<T>& vector<T>::operator=(const vector<T>& x) {
    if (&x == this) return *this;
    if (x.size() > capacity()) {
        destroy(start, finish);
        static_allocator.deallocate(start);
        start = static_allocator.allocate(x.end() - x.begin());
        end_of_storage = uninitialized_copy(x.begin(), x.end(), start);
    } else if (size() >= x.size()) {
        vector<T>::iterator i = copy(x.begin(), x.end(), begin());
        destroy(i, finish);
        // work around for destroy(copy(x.begin(), x.end(), begin()), finish);
    } else {
        copy(x.begin(), x.begin() + size(), begin());
        uninitialized_copy(x.begin() + size(), x.end(), begin() + size());
    }
    finish = begin() + x.size();
    return *this;
}

template <class T>
void vector<T>::insert_aux(iterator position, const T& x) {
    if (finish != end_of_storage) {
        construct(finish, *(finish - 1));
        copy_backward(position, finish - 1, finish);
        *position = x;
        ++finish;
    } else {
        size_type len = size() ? 2 * size() 
            : static_allocator.init_page_size();
        iterator tmp = static_allocator.allocate(len);
        uninitialized_copy(begin(), position, tmp);
        construct(tmp + (position - begin()), x);
        uninitialized_copy(position, end(), tmp + (position - begin()) + 1); 
        destroy(begin(), end());
        static_allocator.deallocate(begin());
        end_of_storage = tmp + len;
        finish = tmp + size() + 1;
        start = tmp;
    }
}

template <class T>
void vector<T>::insert(iterator position, size_type n, const T& x) {
    if (n == 0) return;
    if (end_of_storage - finish >= n) {
        if (end() - position > n) {
            uninitialized_copy(end() - n, end(), end());
            copy_backward(position, end() - n, end());
            fill(position, position + n, x);
        } else {
            uninitialized_copy(position, end(), position + n);
            fill(position, end(), x);
            uninitialized_fill_n(end(), n - (end() - position), x);
        }
        finish += n;
    } else {
        size_type len = size() + max(size(), n);
        iterator tmp = static_allocator.allocate(len);
        uninitialized_copy(begin(), position, tmp);
        uninitialized_fill_n(tmp + (position - begin()), n, x);
        uninitialized_copy(position, end(), tmp + (position - begin() + n));
        destroy(begin(), end());
        static_allocator.deallocate(begin());
        end_of_storage = tmp + len;
        finish = tmp + size() + n;
        start = tmp;
    }
}

template <class T>
void vector<T>::insert(iterator position, 
                       const_iterator first, 
                       const_iterator last) {
    if (first == last) return;
    size_type n = 0;
    distance(first, last, n);
    if (end_of_storage - finish >= n) {
        if (end() - position > n) {
            uninitialized_copy(end() - n, end(), end());
            copy_backward(position, end() - n, end());
            copy(first, last, position);
        } else {
            uninitialized_copy(position, end(), position + n);
            copy(first, first + (end() - position), position);
            uninitialized_copy(first + (end() - position), last, end());
        }
        finish += n;
    } else {
        size_type len = size() + max(size(), n);
        iterator tmp = static_allocator.allocate(len);
        uninitialized_copy(begin(), position, tmp);
        uninitialized_copy(first, last, tmp + (position - begin()));
        uninitialized_copy(position, end(), tmp + (position - begin() + n));
        destroy(begin(), end());
        static_allocator.deallocate(begin());
        end_of_storage = tmp + len;
        finish = tmp + size() + n;
        start = tmp;
    }
}


template <class Key, class Value, class KeyOfValue, class Compare>
class rb_tree { 

    // ...
    class iterator : public bidirectional_iterator<Value, difference_type> {
                // ...
        iterator& operator++() {
            if (right(node) != NIL) {
                node = right(node);
                while (left(node) != NIL)
                    node = left(node);
            } else {
                link_type y = parent(node);
                while (node == right(y)) {
                    node = y;
                    y = parent(y);
                }
                if (right(node) != y) // necessary because of rightmost 
                    node = y;
            }
            return *this;
        }
                // ...
        }                
};

template <class Key, class Value, class KeyOfValue, class Compare>
rb_tree<Key, Value, KeyOfValue, Compare>::iterator
rb_tree<Key, Value, KeyOfValue, Compare>::
__insert(link_type x, link_type y, const Value& v) {
    ++node_count;
    link_type z = get_node();
    construct(value_allocator.address(value(z)), v);
    if (y == header || x != NIL || key_compare(KeyOfValue()(v), key(y))) {
        left(y) = z;  // also makes leftmost() = z when y == header
        if (y == header) {
            root() = z;
            rightmost() = z;
        } else if (y == leftmost())
            leftmost() = z;   // maintain leftmost() pointing to minimum node
    } else {
        right(y) = z;
        if (y == rightmost())
            rightmost() = z;   // maintain rightmost() pointing to maximum node
    }
    parent(z) = y;
    left(z) = NIL;
    right(z) = NIL;
    x = z;  // recolor and rebalance the tree
    color(x) = red;
    while (x != root() && color(parent(x)) == red) 
        if (parent(x) == left(parent(parent(x)))) {
            y = right(parent(parent(x)));
            if (color(y) == red) {
                color(parent(x)) = black;
                color(y) = black;
                color(parent(parent(x))) = red;
                x = parent(parent(x));
            } else {
                if (x == right(parent(x))) {
                    x = parent(x);
                    rotate_left(x);
                }
                color(parent(x)) = black;
                color(parent(parent(x))) = red;
                rotate_right(parent(parent(x)));
            }
        } else {
            y = left(parent(parent(x)));
            if (color(y) == red) {
                color(parent(x)) = black;
                color(y) = black;
                color(parent(parent(x))) = red;
                x = parent(parent(x));
            } else {
                if (x == left(parent(x))) {
                    x = parent(x);
                    rotate_right(x);
                }
                color(parent(x)) = black;
                color(parent(parent(x))) = red;
                rotate_left(parent(parent(x)));
            }
        }
    color(root()) = black;
    return iterator(z);
}

template <class Key, class Value, class KeyOfValue, class Compare>
rb_tree<Key, Value, KeyOfValue, Compare>::pair_iterator_bool
rb_tree<Key, Value, KeyOfValue, Compare>::insert(const Value& v) {
    link_type y = header;
    link_type x = root();
    bool comp = true;
    while (x != NIL) {
        y = x;
        comp = key_compare(KeyOfValue()(v), key(x));
        x = comp ? left(x) : right(x);
    }
    if (insert_always)
        return pair_iterator_bool(__insert(x, y, v), true);
    iterator j = iterator(y);   
    if (comp)
        if (j == begin())     
            return pair_iterator_bool(__insert(x, y, v), true);
        else
            --j;
    if (key_compare(key(j.node), KeyOfValue()(v)))
        return pair_iterator_bool(__insert(x, y, v), true);
    return pair_iterator_bool(j, false);
}  

         
template <class Key, class Value, class KeyOfValue, class Compare>
void rb_tree<Key, Value, KeyOfValue, Compare>::erase(iterator position) {
    link_type z = position.node;
    link_type y = z;
    link_type x;
    if (left(y) == NIL)
        x = right(y);
    else
        if (right(y) == NIL) 
            x = left(y);
        else {
            y = right(y);
            while (left(y) != NIL)
                y = left(y);
            x = right(y);
        }
    if (y != z) { // relink y in place of z
        parent(left(z)) = y; 
        left(y) = left(z);
        if (y != right(z)) {
            parent(x) = parent(y); // possibly x == NIL
            left(parent(y)) = x;   // y must be a left child
            right(y) = right(z);
            parent(right(z)) = y;
        } else
            parent(x) = y;  // needed in case x == NIL
        if (root() == z)
            root() = y;
        else if (left(parent(z)) == z)
            left(parent(z)) = y;
        else 
            right(parent(z)) = y;
        parent(y) = parent(z);
        ::swap(color(y), color(z));
        y = z;
                       // y points to node to be actually deleted
    } else {  // y == z
        parent(x) = parent(y);   // possibly x == NIL
        if (root() == z)
            root() = x;
        else 
            if (left(parent(z)) == z)
                left(parent(z)) = x;
            else
                right(parent(z)) = x;
        if (leftmost() == z) 
            if (right(z) == NIL)  // left(z) must be NIL also
                leftmost() = parent(z);
                // makes leftmost() == header if z == root()
        else
            leftmost() = minimum(x);
        if (rightmost() == z)  
            if (left(z) == NIL) // right(z) must be NIL also
                rightmost() = parent(z);  
                // makes rightmost() == header if z == root()
        else  // x == left(z)
            rightmost() = maximum(x);
    }
    if (color(y) != red) { 
        while (x != root() && color(x) == black)
            if (x == left(parent(x))) {
                link_type w = right(parent(x));
                if (color(w) == red) {
                    color(w) = black;
                    color(parent(x)) = red;
                    rotate_left(parent(x));
                    w = right(parent(x));
                }
                if (color(left(w)) == black && color(right(w)) == black) {
                    color(w) = red;
                    x = parent(x);
                } else {
                    if (color(right(w)) == black) {
                        color(left(w)) = black;
                        color(w) = red;
                        rotate_right(w);
                        w = right(parent(x));
                    }
                    color(w) = color(parent(x));
                    color(parent(x)) = black;
                    color(right(w)) = black;
                    rotate_left(parent(x));
                    break;
                }
            } else {  // same as then clause with "right" and "left" exchanged
                link_type w = left(parent(x));
                if (color(w) == red) {
                    color(w) = black;
                    color(parent(x)) = red;
                    rotate_right(parent(x));
                    w = left(parent(x));
                }
                if (color(right(w)) == black && color(left(w)) == black) {
                    color(w) = red;
                    x = parent(x);
                } else {
                    if (color(left(w)) == black) {
                        color(right(w)) = black;
                        color(w) = red;
                        rotate_left(w);
                        w = left(parent(x));
                    }
                    color(w) = color(parent(x));
                    color(parent(x)) = black;
                    color(left(w)) = black;
                    rotate_right(parent(x));
                    break;
                }
            }
        color(x) = black;
    }
    destroy(value_allocator.address(value(y)));
    put_node(y);
    --node_count;
}         

template <class Key, class Value, class KeyOfValue, class Compare>
rb_tree<Key, Value, KeyOfValue, Compare>::iterator 
rb_tree<Key, Value, KeyOfValue, Compare>::find(const Key& k) {
   link_type y = header; /* Last node which is not less than k. */
   link_type x = root(); /* Current node. */

   while (x != NIL) 
     if (!key_compare(key(x), k))
       y = x, x = left(x);
   else
       x = right(x);

   iterator j = iterator(y);   
   return (j == end() || key_compare(k, key(j.node))) ? end() : j;
}

template <class Key, class Value, class KeyOfValue, class Compare>
rb_tree<Key, Value, KeyOfValue, Compare>::iterator 
rb_tree<Key, Value, KeyOfValue, Compare>::lower_bound(const Key& k) {
   link_type y = header; /* Last node which is not less than k. */
   link_type x = root(); /* Current node. */

   while (x != NIL) 
     if (!key_compare(key(x), k))
       y = x, x = left(x);
     else
       x = right(x);

   return iterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare>
inline void 
rb_tree<Key, Value, KeyOfValue, Compare>::rotate_left(link_type x) {
    link_type y = right(x);
    right(x) = left(y);
    if (left(y) != NIL)
        parent(left(y)) = x;
    parent(y) = parent(x);
    if (x == root())
        root() = y;
    else if (x == left(parent(x)))
        left(parent(x)) = y;
    else
        right(parent(x)) = y;
    left(y) = x;
    parent(x) = y;
}

template <class Key, class Value, class KeyOfValue, class Compare>
inline void 
rb_tree<Key, Value, KeyOfValue, Compare>::rotate_right(link_type x) {
    link_type y = left(x);
    left(x) = right(y);
    if (right(y) != NIL)
        parent(right(y)) = x;
    parent(y) = parent(x);
    if (x == root())
        root() = y;
    else if (x == right(parent(x)))
        right(parent(x)) = y;
    else
        left(parent(x)) = y;
    right(y) = x;
    parent(x) = y;
} 
