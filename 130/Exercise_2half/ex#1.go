package main

import "fmt"

func divide(x int) (int, bool){
	divided := x/2
	iseven := x % 2 == 0 

	return divided, iseven
}

func main() {
	h, even := divide(6)
	fmt.Println(h, even)
}