package main

import (
	"flag"
	"log"
	"net"
	"os"
	"strconv"
)

type Settings struct {
	Address *string
	Message *string
	Verbose *bool
	Sctp    *bool
}

var (
	settings = Settings{
		Address: flag.String("a", "localhost:4242", "address to listen on"),
		Message: flag.String("m", "recv", "message to send"),
		Sctp:    flag.Bool("s", false, "Use SCTP"),
		Verbose: flag.Bool("v", false, "extra logging"),
	}
)

func echoServer(Address *string, Message *string) {
	addr, _ := net.ResolveSCTPAddr("sctp", *settings.Address)
	conn, err := net.ListenSCTP("sctp", addr)

	if err != nil {
		log.Printf("Error listening: %v", err)
		os.Exit(-1)
	}
	defer conn.Close()

	for {
		msg := make([]byte, 2048)
		log.Printf("Listening on %s", *settings.Address)
		aid, addr, sid, err := conn.ReadFromSCTP(msg)
		if err != nil {
			log.Printf("Error: %v ", err)
			break
		}
		println("********")
		println("Message: " + string(msg))
		println("Stream id: " + strconv.Itoa(int(sid)))
		println("Association id: " + strconv.Itoa(int(aid)))
		println("********")
		conn.WriteTo(msg, addr)
	}

}

func main() {
	flag.Parse()
	echoServer(settings.Address, settings.Message)
}
