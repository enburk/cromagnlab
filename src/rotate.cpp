template <typename I> // BidirectionalIterator 

auto inplace_rotate (I f, I m, I l) -> I
{
    std::reverse(f, m);
    std::reverse(m, l);
    std::reverse(f, l);

    m = f + (l - m);

    return m;
}

// predated 1971