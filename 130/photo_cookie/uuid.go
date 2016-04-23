package mem

import (
	"net/http"
	"github.com/nu7hatch/gouuid"
)

func getID(w http.ResponseWriter, r * http.Request) (string){

	var userUUID string
	var cookie *http.Cookie

	cookie, err := r.Cookie("session-id")

	if(err == http.ErrNoCookie){
		userUUID = r.FormValue("id")

		if(userUUID == "") {
			uuid, _ := uuid.NewV4()
			userUUID = uuid.String()
			http.Redirect(w, r, "/?id=" + userUUID,303)
		}

		modelForCookie := model{
			Name: "session-id",
			State: false,
			Pictures: []string{
				"one.jpg",
			},
			ID: userUUID,
		}

			makeCookie(modelForCookie, userUUID, r)
			http.SetCookie(w, cookie)

			return userUUID

	}
	return cookie.Value
}