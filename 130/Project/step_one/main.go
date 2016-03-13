package main 

import (
	"net/http"
	"html/template"
	"log"
)

func upload(res http.ResponseWriter, req *http.Request){
	tpl, err := template.ParseFiles("index.html")
	if err != nil {
		log.Fatalln(err)
	}

	tpl.Execute(res, nil)
}

func main() {
	http.HandleFunc("/", upload)
	http.ListenAndServe(":8000", nil)
}