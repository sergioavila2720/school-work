package main

import (
	"fmt"
	"net/http"
	)

func main() {
	http.HandleFunc("/",func(res http.ResponseWriter, req *http.Request){
		fmt.Fprintf(res, "localhost:8020/sergio%v", req.URL.Path)
	})
	http.ListenAndServe(":8020", nil)
}
/*
package main

import (
	"fmt"
	"net/http"
)

func Url_Path(res http.ResponseWriter, req *http.Request) {
	fmt.Fprintf(res, "localhost:8080/name%v", req.URL.Path)
}


func main() {
	http.HandleFunc("/", Url_Path)
	http.ListenAndServe(":8040", nil)
}

*/