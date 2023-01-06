      subroutine dgefa(a,lda,n,ipvt,info)
      integer lda,n,ipvt(1),info
      double precision a(lda,1)
      double precision t
      integer idamax,j,k,kp1,l,nm1
c
c
c     gaussian elimination with partial pivoting
c
      info = 0
      nm1 = n - 1
      if (nm1 .lt. 1) go to 70
      do 60 k = 1, nm1
         kp1 = k + 1
c
c        find l = pivot index
c        
         l = idamax(n-k+1,a(k,k),1) + k - 1
         ipvt(k) = l
c        
c        zero pivot implies this column already triangularized
c        
         if (a(l,k) .eq. 0.0d0) go to 40
c        
c           interchange if necessary
c           
            if (l .eq. k) go to 10
               t = a(l,k)
               a(l,k) = a(k,k)
               a(k,k) = t
   10       continue
c           
c           compute multipliers
c           
            t = -1.0d0/a(k,k)
            call dscal(n-k,t,a(k+1,k),1)
c           
c           row elimination with column indexing
c           
            do 30 j = kp1, n
               t = a(l,j)
               if (l .eq. k) go to 20
                  a(l,j) = a(k,j)
                  a(k,j) = t
   20          continue
               call daxpy(n-k,t,a(k+1,k),1,a(k+1,j),1)
   30       continue
         go to 50
   40    continue
            info = k
   50    continue
   60 continue
   70 continue
      ipvt(n) = n
      if (a(n,n) .eq. 0.0d0) info = n
      return
      end
c
c     LINPACK variant (Fortran 66 coding), 1970s
c
