package main
import (
	"log"
	"os"
	"net"
)

func main() {

	saddr := "127.0.0.1:4242"
	addr, err := net.ResolveSCTPAddr("sctp", saddr)
	if err != nil {
		println(err)
		os.Exit(-1)
	}

	conn, err := net.DialSCTP("sctp", nil, addr)
	if err != nil {
		println("Error listening " + err.Error())
		os.Exit(-1)
	}

	defer conn.Close()

	var message = "hello"
	bmessage := []byte(message)

	_, err = conn.WriteTo(bmessage, addr)
	if err != nil {
		log.Printf("WriteTo error: %v", err)
	}

}
