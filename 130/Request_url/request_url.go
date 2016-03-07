package main

import (
	
	"fmt"
	"net/http"
)

func main() {
		http.HandleFunc("/", func(res http.ResponseWriter, req *http.Request) {
			fmt.Fprintf(res, "RequestURI: %v\n", req.RequestURI)
		})

		http.ListenAndServe(":8060", nil)
	}