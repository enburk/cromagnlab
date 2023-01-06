(defun distance (px py qx qy)
  (sqrt (+ (square (- qx px))
           (square (- qy py)))))

(defun slope (px py qx qy)
  (if (= px qx)
      nil
      (/ (- qy py) (- qx px))))

(defun collinear (px py qx qy rx ry)
  (equalp (slope px py qx qy)
          (slope qx qy rx ry)))
		  
		  