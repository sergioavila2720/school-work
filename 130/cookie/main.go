package main 

import (
	"io"
	"net/http"
	"strconv"
)

func cookie(res http.ResponseWriter, req *http.Request){
	if req.URL.Path != "/"{
		http.NotFound(res, req)
		return
	}
	cookie, err := req.Cookie("galleta")
	if err == http.ErrNoCookie{
		cookie = &http.Cookie{
			Name: "galleta",
			Value: "0",
		}
	}
	count, _ := strconv.Atoi(cookie.Value)
	count ++
	cookie.Value = strconv.Itoa(count)
	http.SetCookie(res, cookie)
	io.WriteString(res, cookie.Value)
}


func main() {

	http.HandleFunc("/", cookie)
	http.ListenAndServe(":8000", nil)
	
	
}