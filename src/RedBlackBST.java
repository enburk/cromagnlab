/*
 * Algorithms, 4th edition by Robert Sedgewick and Kevin Wayne,
 * Addison-Wesley Professional, 2011, ISBN 0-321-57351-X.
 * http://algs4.cs.princeton.edu
 */

// Assuming that h is red and both h.left and h.left.left
// are black, make h.left or one of its children red.
private Node moveRedLeft(Node h) {
	assert (h != null);
	assert isRed(h) && !isRed(h.left) && !isRed(h.left.left);
	flipColors(h);
	if (isRed(h.right.left)) { 
		h.right = rotateRight(h.right);
		h = rotateLeft(h);
		flipColors(h);
	}
	return h;
}

// Return key in BST rooted at x of given rank.
// Precondition: rank is in legal range.
private Key select(Node x, int rank) {
	if (x == null) return null;
	int leftSize = size(x.left);
	if      (leftSize > rank) return select(x.left,  rank);
	else if (leftSize < rank) return select(x.right, rank - leftSize - 1); 
	else                      return x.key;
}

// number of keys less than key in the subtree rooted at x
private int rank(Key key, Node x) {
	if (x == null) return 0; 
	int cmp = key.compareTo(x.key); 
	if      (cmp < 0) return rank(key, x.left); 
	else if (cmp > 0) return 1 + size(x.left) + rank(key, x.right); 
	else              return size(x.left); 
} 


// is the tree rooted at x a BST with all keys strictly between min and max
// (if min or max is null, treat as empty constraint)
// Credit: Bob Dondero's elegant solution
private boolean isBST(Node x, Key min, Key max) {
	if (x == null) return true;
	if (min != null && x.key.compareTo(min) <= 0) return false;
	if (max != null && x.key.compareTo(max) >= 0) return false;
	return isBST(x.left, min, x.key) && isBST(x.right, x.key, max);	