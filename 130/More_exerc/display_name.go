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
