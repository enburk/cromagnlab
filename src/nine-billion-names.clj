; http://rosettacode.org/wiki/9_billion_names_of_God_the_integer

(defn nine-billion-names [row column]
  (cond (<= row 0) 0
        (<= column 0) 0
        (< row column) 0
        (= row 1) 1
        :else (let [addend (nine-billion-names (dec row) (dec column))
                    augend (nine-billion-names (- row column) column)]
	            (+ addend augend))))
 
(defn print-row [row]
  (doseq [x (range 1 (inc row))]
    (print (nine-billion-names row x) \space)) 
    (println))
 
(defn print-triangle [rows]
  (doseq [x (range 1 (inc rows))]
    (print-row x)))
 
(print-triangle 25)

;Output:
;   1
;   1   1
;   1   1   1
;   1   2   1   1
;   1   2   2   1   1
;   1   3   3   2   1   1
;   1   3   4   3   2   1   1
;   1   4   5   5   3   2   1   1
;   1   4   7   6   5   3   2   1   1
;   1   5   8   9   7   5   3   2   1   1
;   1   5  10  11  10   7   5   3   2   1   1
;   1   6  12  15  13  11   7   5   3   2   1   1
;   1   6  14  18  18  14  11   7   5   3   2   1   1
;   1   7  16  23  23  20  15  11   7   5   3   2   1   1
;   1   7  19  27  30  26  21  15  11   7   5   3   2   1   1
;   1   8  21  34  37  35  28  22  15  11   7   5   3   2   1   1
;   1   8  24  39  47  44  38  29  22  15  11   7   5   3   2   1   1
;   1   9  27  47  57  58  49  40  30  22  15  11   7   5   3   2   1   1
;   1   9  30  54  70  71  65  52  41  30  22  15  11   7   5   3   2   1   1
;   1  10  33  64  84  90  82  70  54  42  30  22  15  11   7   5   3   2   1   1
;   1  10  37  72 101 110 105  89  73  55  42  30  22  15  11   7   5   3   2   1   1
;   1  11  40  84 119 136 131 116  94  75  56  42  30  22  15  11   7   5   3   2   1   1
;   1  11  44  94 141 163 164 146 123  97  76  56  42  30  22  15  11   7   5   3   2   1   1
;   1  12  48 108 164 199 201 186 157 128  99  77  56  42  30  22  15  11   7   5   3   2   1   1
;   1  12  52 120 192 235 248 230 201 164 131 100  77  56  42  30  22  15  11   7   5   3   2   1   1