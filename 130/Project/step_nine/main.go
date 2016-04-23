package main

import (
	"crypto/hmac"
	"crypto/sha256"
	"encoding/base64"
	"encoding/json"
	"fmt"
	"github.com/nu7hatch/gouuid"
	"io"
	"net/http"
	"strings"
)

type User struct {
	Name string
	Age  string
	Log  string
}

func cookie(w http.ResponseWriter, r *http.Request) {
	cookie, err := r.Cookie("logged-in")
	if err == http.ErrNoCookie {
		cookie = &http.Cookie{
			Name:  "logged-in",
			Value: "0",
			HttpOnly: true,
		}
	}

	if r.Method == "POST" {

		password := r.FormValue("password")

		if password == "abcd" {
			person := User{
				Name: r.FormValue("userName"),
				Age:  r.FormValue("age"),
				Log:  "1",
			}
			b, _ := json.Marshal(person)
			encode := base64.StdEncoding.EncodeToString(b)
			id, _ := uuid.NewV4()

			cookie = &http.Cookie{
				Name:  "logged-in",
				Value: "1" + "|" + id.String() + "|" + getCode(id.String()) + "|" + encode + "|" + getCode(encode),
				HttpOnly: true,
			}
		}
	}
	if r.URL.Path == "/logout" {
		cookie = &http.Cookie{
			Name:   "logged-in",
			Value:  "0",
			MaxAge: -1,
			HttpOnly: true,
		}
		http.SetCookie(w, cookie)
		http.Redirect(w, r, "/", 302)
	}
	http.SetCookie(w, cookie)
	cv := strings.Split(cookie.Value, "|")

	var html string
	if cv[0] == "0" {
		html = `
				<!DOCTYPE html>
				<html lang="en">
				<head>
						<meta charset="UTF-8">
						<title>Project</title>
				</head>
				<body>
				<h1>LOG IN</h1>
				<form method= "Post">
						<h4>User Name:</h4><input type="text" name="userName"><br>
						<h4>Age:<h4><input type="text" name="age"><br>
						<h4>Password:</h4><input type="text" name="password"><br>
						<input type="submit">
				</form>
				</body>
				</html>`
	}
	if cv[0] == "1" {
		http.Redirect(w, r, "/login", 302)

	}
	io.WriteString(w, html)
}

func File(w http.ResponseWriter, r *http.Request) {
w.Header().Set("Content-Type", "text/html")
	fmt.Fprint(w, `
		<h1><a href= "/logout">LOG OUT</a></h1>
		<form method = "POST"  enctype="multipart/form-data">
		<input type="file" name="name"><br><br>
		<input type="submit">
		</form>`)
	if r.Method == "POST" {
		key := "name"
		_, header, err := r.FormFile(key)
		if err != nil {
			fmt.Println(err)
		}
		inFile, err := header.Open()
		if err != nil {
			fmt.Println(err)
		}
		io.Copy(w, inFile)
	}
}
func getCode(data string) string {
	h := hmac.New(sha256.New, []byte("key"))
	io.WriteString(h, data)
	return fmt.Sprintf("%x", h.Sum(nil))
}
func main() {
	http.HandleFunc("/", cookie)
	http.HandleFunc("/login", File)
	http.ListenAndServe(":8080", nil)
}	