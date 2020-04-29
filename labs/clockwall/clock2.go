// Clock2 is a concurrent TCP server that periodically writes the time.
package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"strings"
	"time"
)

func handleConn(c net.Conn, timezone string, location *time.Location) {
	defer c.Close()
	for {
		_, err := io.WriteString(c, timezone+strings.Repeat(" ", 15-len(timezone))+": "+time.Now().In(location).Format("15:04:05\n"))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {

	if len(os.Args) < 3 || len(os.Args) > 3 || (os.Args[1]) != "-port" {
		fmt.Println("Usage TZ={Timezone} ./clock2 -port {Port}")
		os.Exit(1)
	}

	port := os.Args[2]
	timezone := os.Getenv("TZ")
	if len(timezone) == 0 {
		fmt.Println("Usage TZ={Timezone} ./clock2 -port {Port}")
		os.Exit(1)
	}

	listener, err := net.Listen("tcp", "localhost:"+port)

	loc, errt := time.LoadLocation(timezone)

	if errt != nil {
		log.Panic(timezone + " Is an invalid timezone")
	}
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn, timezone, loc) // handle connections concurrently
	}
}
