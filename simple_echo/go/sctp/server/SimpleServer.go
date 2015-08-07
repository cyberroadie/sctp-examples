package main
import "net"

func main() {
  conn, _ := net.ListenPacket("sctp", "localhost:4242")
  defer conn.Close()
  message := make([]byte, 5)
  conn.ReadFrom(message)
  print(string(message))
}
