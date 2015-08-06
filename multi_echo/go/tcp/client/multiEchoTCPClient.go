// EchoClient.go
package main

import (
	"encoding/binary"
	"flag"
	"fmt"
	"io"
	"log"
	"net"
)

type Settings struct {
	Address   *string
	Message   *string
	Count     *int
	Instances *int
	Verbose   *bool
}

var (
	settings = Settings{
		Address:   flag.String("a", "127.0.0.1:4242", "address to listen on"),
		Message:   flag.String("m", "recv", "message to send"),
		Count:     flag.Int("c", 1, "number of messages"),
		Instances: flag.Int("n", 1, "number of threads"),
		Verbose:   flag.Bool("v", true, "extra logging"),
	}
)

func messageSender(address *string, message *string, count int, done chan bool) {

	var conn net.Conn
	conn, err := net.Dial("tcp", *address)
	if *settings.Verbose {
		fmt.Printf("Connecting to %s\n", *address)
	}
	if err != nil {
		log.Printf("error connecting: %s", err)
		return
	}
	defer conn.Close()

	if err := conn.(*net.TCPConn).SetNoDelay(true); err != nil {
		log.Printf("error setting socket option: %s", err)
		return
	}

	idBytes := make([]byte, 4)
	idReceived, idBytesReceived := uint32(0), make([]byte, 4)
	size, sizeBytes := uint32(len(*message)), make([]byte, 4)
	binary.BigEndian.PutUint32(sizeBytes, uint32(size))

	for i := 0; i < count; i++ {
		// Write data length
		if _, err := conn.Write(sizeBytes); err != nil {
			log.Printf("error writing id: %s", err)
			return
		}

		// Write id
		binary.BigEndian.PutUint32(idBytes, uint32(i))
		if _, err := conn.Write(idBytes); err != nil {
			log.Printf("error writing id: %s", err)
			return
		}

		if _, err := conn.Write([]byte(*message)); err != nil {
			log.Printf("error writing id: %s", err)
			return
		}

		// Get message id
		if n, err := io.ReadFull(conn, idBytesReceived); n == 0 && err == io.EOF {
			break
		} else if err != nil {
			log.Printf("read id: %s (after %d bytes)", err, n)
			break
		}
		if *settings.Verbose {
			idReceived = binary.BigEndian.Uint32(idBytesReceived)
			log.Printf("id received: %d", idReceived)
		}
	}
	done <- true
	return
}

func echoClient(address *string, message *string, count *int, instances *int) {
	done := make(chan bool)
	for i := 0; i < *instances; i++ {
		go messageSender(address, message, *count, done)
	}
	finished := 0
	for {
		if finished == *instances {
			return
		}
		<-done
		finished++
	}
}

func main() {
	flag.Parse()
	log.Printf("Connecting to %s and starting %d goroutines, sending %d messages",
		*settings.Address,
		*settings.Instances,
		*settings.Count)
	echoClient(settings.Address, settings.Message, settings.Count, settings.Instances)
}
