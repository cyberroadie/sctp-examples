package main

import "net"

func main() {
	addr, _ := net.ResolveSCTPAddr("sctp", "localhost:4242")
	conn, _ := net.DialSCTP("sctp", nil, addr)
	defer conn.Close()
	message := []byte("paard")
	conn.WriteTo(message, addr)
}
