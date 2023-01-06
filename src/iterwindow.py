## https://github.com/sametmax/minibelt

from collections import deque
from itertools import islice
def window(iterable, size=2):
    iterable = iter(iterable)
    d = deque(islice(iterable, size), size)
    yield d
    for x in iterable:
        d.append(x)
        yield d

## This let you iterate with a sliding window :

for win in window('abcdefghijkl', 3):
    print('-'.join(win))

## a-b-c
## b-c-d
## c-d-e
## d-e-f
## e-f-g
## f-g-h
## g-h-i
## h-i-j
## i-j-k
## j-k-l

###########################

def window(iterable, size=2, cast=tuple):
    """
        Yields iterms by bunch of a given size, but rolling only one item
        in and out at a time when iterating.
        >>> list(window([1, 2, 3]))
        [(1, 2), (2, 3)]
        By default, this will cast the window to a tuple before yielding it;
        however, any function that will accept an iterable as its argument
        is a valid target.
        If you pass None as a cast value, the deque will be returned as-is,
        which is more performant. However, since only one deque is used
        for the entire iteration, you'll get the same reference everytime,
        only the deque will contains different items. The result might not
        be what you want :
        >>> list(window([1, 2, 3], cast=None))
        [deque([2, 3], maxlen=2), deque([2, 3], maxlen=2)]
    """
    iterable = iter(iterable)
    d = deque(islice(iterable, size), size)
    if cast:
        yield cast(d)
        for x in iterable:
            d.append(x)
            yield cast(d)
    else:
        yield d
        for x in iterable:
            d.append(x)
            yield d
