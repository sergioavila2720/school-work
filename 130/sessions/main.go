
package main 

import (
	"net/http"
	//"github.com/nu7hatch/gouuid"
	//"log"
	"github.com/nu7hatch/gouuid"
)
func session(res http.ResponseWriter, req *http.Request){

		id, err := uuid.NewV4()

		logError(err)

		cookie := &http.Cookie{
			Name:     "session",
			Value:    id.String(),
			HttpOnly: true,
		}
		http.SetCookie(res, cookie)
	
}

func main() {
	
	http.HandleFunc("/", session)
	http.ListenAndServe(":8020", nil)
	
}