package main

//import "fmt"
import (
	"log"
	"os"
	"text/template"
)
type person struct{
	Name string
}

type conditions struct {
	person 
	IsYou bool

}

func main() {

	p1 := conditions{
		person: person{
			Name: "akdns" ,
		},
		IsYou:false,
	}

	if p1.Name == "sergio" {
		p1.IsYou = true
	}

	tmp, err := template.ParseFiles("tmp.gohtml")
	if err != nil {
		log.Fatalln(err)
	}

	err = tmp.Execute(os.Stdout, p1)
	if err != nil {
		log.Fatalln(err)
	}
}