package main

import "fmt"

func main() {

	funExpr := func (x int) (int, bool){
		divided := x/2
		iseven := x % 2 == 0 

		return divided, iseven
	}
	fmt.Println(funExpr(10))
}