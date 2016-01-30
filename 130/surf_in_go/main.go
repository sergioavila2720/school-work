package main

import (
	//"io"
	"net/http"
	"html/template"
	"log"
)

func surf(res http.ResponseWriter, req *http.Request){
	tpl, err := template.ParseFiles("tmp.gohtml")
	if err != nil {
		log.Fatalln(err)
	}

	tpl.Execute(res, nil)
}
func main() {

	http.HandleFunc("/", surf)
	http.Handle("/assets/", http.StripPrefix("/assets", http.FileServer(http.Dir("./assets"))))
	http.Handle("/cssss/", http.StripPrefix("/css", http.FileServer(http.Dir("./css"))))

	http.ListenAndServe(":8090", nil)
}