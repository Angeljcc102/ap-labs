package main

import (
	"fmt"
	"log"
	"net"
	"os"
	"time"
)

func read(response []byte, conn net.Conn, wait chan int) {
	defer conn.Close()
	for {
		_, err := conn.Read(response)
		if err != nil {
			log.Fatal("Server disconnection")
		}
		fmt.Print(string(response))
		time.Sleep(1 * time.Second)
	}
	wait <- 1
}

func main() {

	wait := make(chan int)
	for i := 1; i < len(os.Args); i++ {
		address := os.Args[i]
		conn, err := net.DialTimeout("tcp", address, time.Second)
		if err != nil {
			log.Fatal(" Invalid host(s) or timeout exceeded ")
		}
		response := make([]byte, 30)

		go read(response, conn, wait)
	}

	<-wait
	close(wait)
}
