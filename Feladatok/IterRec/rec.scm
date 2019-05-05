(define (faktorialis n)
        (if (= n 1)
                1
                (* n (faktorialis (- n 1) ))
        ))

(display (faktorialis 3))
