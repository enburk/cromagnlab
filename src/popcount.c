/* {{{ *//* }}} */

/**
 * Counting 1-bits, “divide and conquer” strategy
 * 
 * x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
 * x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
 * x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
 * x = (x & 0x00FF00FF) + ((x >> 8) & 0x00FF00FF);
 * x = (x & 0x0000FFFF) + ((x >>16) & 0x0000FFFF);
 */

int popcount(unsigned x) {
   x = x - ((x >> 1) & 0x55555555);
   x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
   x = (x + (x >> 4)) & 0x0F0F0F0F;
   x = x + (x >> 8);
   x = x + (x >> 16);
   return x & 0x0000003F;
}

/* {{{ *//* }}} */

/* Comparing popcount(x) with popcount(y) */

int popCmpr(unsigned xp, unsigned yp) {
    unsigned x, y;
    x = xp & ~yp; // Clear bits where
    y = yp & ~xp; // both are 1.
    while (1) {
        if (x == 0) return y | -y;
        if (y == 0) return 1;
        x = x & (x - 1); // Clear one bit
        y = y & (y - 1); // from each.
    }
}
