package main


import (
	"os"
	"io"
	"path/filepath"
	"net/http"
	"html/template"
	"log"
)
func fileP(res http.ResponseWriter, req *http.Request){
	tpl, err := template.ParseFiles("index.gohtml")
	if err != nil {
		log.Fatalln(err)
	}
	tpl.Execute(res, nil)

}

func upload(res http.ResponseWriter, req *http.Request){
	if req.Method == "POST" {
		file, _, err := req.FormFile("n")
		if err != nil {
			http.Error(res, err.Error(), 500)
			return
		}
		defer file.Close()

		src := io.LimitReader(file, 400)

		dst, err := os.Create(filepath.Join(".", "file.txt"))
		if err != nil {
			http.Error(res, err.Error(), 500)
			return
		}
		defer dst.Close()

		io.Copy(dst, src)
	}

}

func main() {

	http.HandleFunc("/", fileP)
	http.ListenAndServe(":8020", nil)
	
}