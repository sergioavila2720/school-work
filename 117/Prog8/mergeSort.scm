; In this program we will be implementing the merge sort algorithm 
; This assignment is interesting because we will be implementing this 
; in a functional language, Scheme. I already have some experience with the functional language Haskell
; this program still requires different way of thinking because we are so used to the more popular anguages 


; I will have three main procedures or functions called Splits, merge, and merger-sort

; I think i will be making two helper functions to help me choose eved and odd numbers
; I do this because if i choose to so consecutive numbers, the implementation will be much complex.

; I ran this program through the terminal in ubuntu. output examples will be in a separate file.

;helper function for odd nubers
(define (odd x)
	(if (null? x)'()
		(if (null? (cdr x)) (list (car x))
			(cons (car x) (odd (cddr x)))))) 
;helper function for even numbers. 
(define (even x)
	(if (null? x) '()
		(if (null? (cdr x)) '()
			(cons (cadr x) (even (cddr x))))))

; ready to implement the splits function
; splits a list in two with even and odd places like Dr. Park example
(define (splits x)
	(cons (odd x) (cons (even x) '())))

;merge function
; this merges two lists together, but does not sort them 
(define (merge x y)   ; L = x   M = y
	(if (null? x) y
		(if (null? y) x
			(if (< (car x) (car y))
				(cons (car x) (merge (cdr x) y))
				(cons (car y) (merge (cdr y) x))))))


(define (mergeSort x)
	(if (null? x) x
		(if (null? (cdr x)) x
			(merge (mergeSort (car (splits x))) (mergeSort (cadr (splits x)))))))