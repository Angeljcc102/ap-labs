package main

import (
	"flag"
	"fmt"
	"os"
	"time"
)

var comm int

//pinger and ponger retrieved from: https://riptutorial.com/go/example/6056/ping-pong-with-two-goroutines
func pinger(pinger <-chan int, ponger chan<- int) {
	for {
		<-pinger
		comm++
		ponger <- 0
	}
}

func ponger(pinger chan<- int, ponger <-chan int) {
	for {
		<-ponger
		comm++
		pinger <- 1
	}
}

//avoid deadlocks
func start(pinger chan int) {
	pinger <- 1
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage : go run exercise9_5.go -time={Integer time in seconds}")
		os.Exit(1)
	}

	var seconds *int

	ping := make(chan int)
	pong := make(chan int)

	seconds = flag.Int("time", 0, "time limit in seconds")
	flag.Parse()

	time := time.NewTimer(time.Duration(*seconds) * time.Second)

	report, err := os.Create("exercise9_5.txt")
	if err != nil {
		panic(err)
	}

	go start(ping)
	go pinger(ping, pong)
	go ponger(ping, pong)

	<-time.C

	report.WriteString("Exercise9.5 report: \n")
	report.WriteString(fmt.Sprintf("Total number of communications between goroutines in %d seconds: %d \n", *seconds, comm))
	report.WriteString(fmt.Sprintf("Average of communications between goroutines in a second: %d \n", comm/(*seconds)))

}
