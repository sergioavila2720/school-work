package main 

import (
	"net/http"
	"html/template"
	"log"
	"github.com/nu7hatch/gouuid"
)

func upload(res http.ResponseWriter, req *http.Request){
	tpl, err := template.ParseFiles("index.html")
	if err != nil {
		log.Fatalln(err)
	}
	cookie, err := req.Cookie("session")

	id, _ := uuid.NewV4()

		logError(err)

		cookie := &http.Cookie{
			Name:     "session",
			Value:    id.String(),
			HttpOnly: true,
		}
		http.SetCookie(res, cookie)

	tpl.Execute(res, nil)
}

func main() {
	http.HandleFunc("/", upload)
	http.ListenAndServe(":8080", nil)
}