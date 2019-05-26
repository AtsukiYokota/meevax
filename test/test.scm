(load "../setup.scm")

(define x 42)
x
(set! x 100)
x

(define y 'hoge)
y
(set! y 100)
y

(define accumulator
  (lambda (n)
    (lambda ()
      (set! n (+ n 1)))))

(define acc (accumulator x))
(acc)
(acc)
(acc)

(define a '(1 2 3))
(transform '(a b c)); => (cons 'a (cons 'b (cons 'c '())))
(transform '(,a b c)); => (cons a (cons 'b (cons 'c '())))
(transform '(,a ,@b c)); => (cons a (append b (cons 'c '())))
(transform '(,(car a) ,@(cdr b) c)); => (cons (car a) (append (cdr b) (cons 'c '())))

`(a b c); => (a b c)
`(,a b c); => ((1 2 3) b c)
`(,@a b c); => (1 2 3 b c)

(define map
  (lambda (callee &list)
    (if (null? &list)
        '()
        (cons (callee (car &list))
              (map callee (cdr &list))))))

(define let
  (macro (bindings . body)
    ; (append (list (append (list 'lambda (map car args)) body)
    ;   (map cadr args)
    ;  ))

    (display bindings)
    (display body)
    (display list)
    ; 'fuck
    (+ 1 2)
    (display (current-global-environment))
    (display (current-lexical-environment))
    ; (list 'car)
    ; (list 'car 'bindings)
    ; (list 'map 'car bindings)
))

(let ((a 1)
      (b 2))
  (+ a b)
  (* a b))

; ((lambda (x y)
;    (+ x y))
;  1 2)


