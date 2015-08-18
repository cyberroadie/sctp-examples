package main

import (
	"fmt"
	"log"
	"net"
	"os"
)


func main() {

	saddr := "127.0.0.1:4242"
	c, err := net.ListenPacket("sctp", saddr)
	if err != nil {
		log.Printf("Error listening: %v", err)
		os.Exit(-1)
	}

	defer c.Close()

	for {
		msg := make([]byte, 100)
		length, _, err := c.ReadFrom(msg)
		log.Printf("[ Receive echo (%x bytes): stream = hu, " +
				"flags = hx, ppid = u ]\n", length, )

//		printf("[ Receive echo (%u bytes): stream = %hu, "
//		"flags = %hx, ppid = %u ]\n", length,
//		rinfo->rcv_sid, rinfo->rcv_flags,
//		rinfo->rcv_ppid);
//
		if err != nil {
			log.Printf("Error: %v ", err)
			break
		}
		fmt.Println("Message: " + string(msg))
	}

}
