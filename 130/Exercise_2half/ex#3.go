package main

import "fmt"


func maxNum(numbers ...int) int {

	greater := 0
	for _, i := range numbers {
		if i > greater {
			greater = i
		}
	}
	return greater
}

func main() {

	fmt.Println(maxNum(4,87,2,6,8,7,87,89,2,65,14,22))
	
}