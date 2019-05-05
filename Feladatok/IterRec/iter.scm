(define (faktorialis n)
        (define (iter product counter)
                (if (> counter n)
                        product
                        (iter (* counter product) (+ counter 1))
                ))
        (iter 1 1))

(display (faktorialis 3))
