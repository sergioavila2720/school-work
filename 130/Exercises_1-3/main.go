package main

import "fmt"

func main() {
	var myName string
   // fmt.Printf("Hello, my name is " + myName + "\n")
	fmt.Print("Enter your name\n")
	fmt.Scanf("%s", &myName) // i read that i can also use %v for the default format
	fmt.Print("Hello ", myName, "\n")
}