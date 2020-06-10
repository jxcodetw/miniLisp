(define f (fun (x)
   (if (< x 2) x (+
                  (f (- x 1)) (f (- x 2)) ))))

(print-num (f 1))
(print-num (f 2))
(print-num (f 3))
(print-num (f 4))
(print-num (f 5))
(print-num (f 6))
(print-num (f 7))
(print-num (f 8))
(print-num (f 9))
(print-num (f 10))

