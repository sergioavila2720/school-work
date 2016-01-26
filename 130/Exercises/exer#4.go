package main 

import "fmt"

func remainder(x int, y int) int{
	return x % y
}

func main() {
	var small int
	var big int

	fmt.Print("Enter a small number and a big number\n")
	fmt.Scanf("%d", &small)
	fmt.Scanf("%d", &big)
	fmt.Println(remainder(big, small))
}