package main

import "fmt"


func main() {
	expression1 := (true && false) 
	expression2 := (false && true) 
	expression3 := !(false && false) 
	expression4 := (true && false) || (false && true) || !(false && false)

	fmt.Println("(true && false) = ", expression1)
	fmt.Println("(false && true) =", expression2)
	fmt.Println("!(false && false) = ", expression3)
	fmt.Println("(true && false) || (false && true) || !(false && false) = ", expression4)

}