/*
 * User: cyberroadie
 * Date: 12/11/2011
 */
package main

import (
	"encoding/binary"
	"flag"
	"io"
	"log"
	"net"
	"os"
)

type Settings struct {
	Address *string
	Message *string
	Verbose *bool
	Sctp    *bool
}

var (
	settings = Settings{
		Address: flag.String("a", "127.0.0.1:4242", "address to listen on"),
		Message: flag.String("m", "recv", "message to send"),
		Sctp:    flag.Bool("s", false, "Use SCTP"),
		Verbose: flag.Bool("v", true, "extra logging"),
	}
)

func handleClient(conn net.Conn, message *string) {

	size, sizeBytes := uint32(0), make([]byte, 4)
	id, idBytes := uint32(0), make([]byte, 4)

	for {
		// Get message length
		if n, err := io.ReadFull(conn, sizeBytes); n == 0 && err == io.EOF {
			break
		} else if err != nil {
			log.Printf("read size: %s (after %d bytes)", err, n)
			break
		}
		// Get message id
		if n, err := io.ReadFull(conn, idBytes); n == 0 && err == io.EOF {
			break
		} else if err != nil {
			log.Printf("read id: %s (after %d bytes)", err, n)
			break
		}

		size = binary.BigEndian.Uint32(sizeBytes)
		data := make([]byte, size)
		if _, err := io.ReadFull(conn, data); err != nil {
			log.Printf("read data: %s", err)
			break
		}

		// Send message id back as ACK
		id = binary.BigEndian.Uint32(idBytes)
		//log.Printf("write data: %d", id)
		if _, err := conn.Write(idBytes); err != nil {
			log.Printf("write data: %d", id)
			log.Printf("write data: %s", err)
			break
		}
	}
}

func echoServer(Address *string, Message *string) {
	var netlisten net.Listener
	var err error

	if *settings.Sctp == true {
		log.Printf("Using SCTP")
		netlisten, err = net.Listen("sctp", *settings.Address)
	} else {
		log.Printf("Using TCP")
		netlisten, err = net.Listen("tcp", *settings.Address)
	}

	if err != nil {
		log.Printf("Error listening: %s", err)
		os.Exit(-1)
	}
	defer netlisten.Close()

	for {
		c, err := netlisten.Accept()
		if err != nil {
			log.Printf("Error accepting: %s", err)
			continue
		}
		go handleClient(c, Message)
	}
}

func main() {
	flag.Parse()
	echoServer(settings.Address, settings.Message)
}
