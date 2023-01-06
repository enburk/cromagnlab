/**
 * Implements a top-down splay tree.
 * Available at http://www.link.cs.cmu.edu/splay/
 * Author: Danny Sleator <sleator@cs.cmu.edu>
 * This code is in the public domain.
 */

    private void splay(Comparable key) {
	BinaryNode l, r, t, y;
	l = r = header;
	t = root;
	header.left = header.right = null;
	for (;;) {
	    if (key.compareTo(t.key) < 0) {
		if (t.left == null) break;
		if (key.compareTo(t.left.key) < 0) {
		    y = t.left;                        /* rotate right */
		    t.left = y.right;
		    y.right = t;
		    t = y;
		    if (t.left == null) break;
		}
		r.left = t;                            /* link right */
		r = t;
		t = t.left;
	    } else if (key.compareTo(t.key) > 0) {
		if (t.right == null) break;
		if (key.compareTo(t.right.key) > 0) {
		    y = t.right;                       /* rotate left */
		    t.right = y.left;
		    y.left = t;
		    t = y;
		    if (t.right == null) break;
		}
		l.right = t;                           /* link left */
		l = t;
		t = t.right;
	    } else {
		break;
	    }
	}
	l.right = t.left;                          /* assemble */
	r.left = t.right;
	t.left = header.right;
	t.right = header.left;
	root = t;
    }

/**
 * Splay tree: Daniel Sleator and Robert Tarjan, 1985
 */
	
