(define (list-copy list)
  (define (list-copy-loop l tail last)
    (cond ((null? l)
           tail)
          (else
            (set-cdr! last (cons (car l) nil))
            (list-copy-loop (cdr l) tail (cdr last)))))
  (if (pair? list)
      (let ((first (cons (car list) nil)))
        (list-copy-loop (cdr list) first first))))

(define (tree-copy tree)
  (define stack-of-cdrs '())
  (define (tree-copy-loop l)
    (cond ((pair? (car l))
           (if (pair? (cdr l))
               (set! stack-of-cdrs (cons l stack-of-cdrs)))
           (set-car! l (cons (caar l) (cdar l)))
           (tree-copy-loop (car l)))
          ((pair? (cdr l))
           (set-cdr! l (cons (cadr l) (cddr l)))
           (tree-copy-loop (cdr l)))
          ((pair? stack-of-cdrs)
           (let ((i stack-of-cdrs)
                 (j (car stack-of-cdrs)))
             (set! stack-of-cdrs (cdr stack-of-cdrs))
             (set-car! i (cadr j))
             (set-cdr! i (cddr j))
             (set-cdr! j i)
             (tree-copy-loop i)))))
  (if (pair? tree)
      (let ((n (cons (car tree) (cdr tree))))
        (tree-copy-loop n)
        n)
      tree))


;@
;;;==========================================
;;;
;;;         Random Graph Generators
;;;
;;;==========================================

(define (random-edge n length)
  (let loop ((i (random n))
             (j (random n)))
    (if (= i j)
        (loop (random n) (random n))
        (vector i j (random length)))))

(define (d-graph n m . r)
  (let* ((r (if (null? r) 100 (car r)))
         (graph (make-graph n))
         (add (graph 'add-directed-link))
         (add-random-link
           (lambda (x) (add (random-edge n r)))))
    (for-each-integer add-random-link m)
    graph))

(define (u-graph n m . r)
  (let* ((r (if (null? r) 100 (car r)))
         (graph (make-graph n))
         (add (graph 'add-undirected-link))
         (add-random-link
           (lambda (x) (add (random-edge n r)))))
    (for-each-integer add-random-link m)
    graph)) 




(define (mean l)
  (let loop ((result 0) (n 0) (l l))
    (if (null? l)
        (/ result n)
        (loop (+ result (car l)) (1+ n) (cdr l)))))

(define (variance l)
  (let ((m (mean l)))
    (let loop ((result 0) (n -1) (l l))
      (if (null? l)
          (/ result n)
          (loop (+ result (let ((i (- (car l) m))) (* i i)))
                (1+ n)
                (cdr l))))))

(define (standard-deviation l) (sqrt (variance l)))




	