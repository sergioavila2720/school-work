package main

import (
	"html/template"
	"net/http"

	"golang.org/x/net/context"
	"google.golang.org/appengine"
	"google.golang.org/appengine/log"
	"google.golang.org/cloud/storage"
)

func init() {
	http.Handle("/css/", http.StripPrefix("/css", http.FileServer(http.Dir("./css"))))
	http.Handle("/img/", http.StripPrefix("/img", http.FileServer(http.Dir("img"))))
	http.HandleFunc("/", handler)
}

const gcsBucket = "nikolich-project-1.appspot.com"

func handler(res http.ResponseWriter, req *http.Request) {
	if req.URL.Path != "/" {
		http.NotFound(res, req)
		return
	}

	ctx := appengine.NewContext(req)

	client, err := storage.NewClient(ctx)
	if err != nil {
		log.Errorf(ctx, "ERROR handler NewClient: ", err)
		return
	}

	defer client.Close()

	tpl := template.Must(template.ParseFiles("index.html"))
	err = tpl.Execute(res, getImages(ctx, client))

}

func getImages(ctx context.Context, client *storage.Client) []string {
	query := &storage.Query{
		Delimiter: "/",
		Prefix:    "photos/",
	}

	objs, err := client.Bucket(gcsBucket).List(ctx, query)
	if err != nil {
		log.Errorf(ctx, "%v", err)
	}

	var outString []string
	for _, obj := range objs.Results {
		outString = append(outString, obj.Name)
	}
	return outString
}