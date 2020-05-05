// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"strings"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

var (
	entering      = make(chan client)
	leaving       = make(chan client)
	messages      = make(chan string) // all incoming client messages
	user          = make(chan string)
	nameUsers     = make(map[string]net.Conn)
	numUsers  int = 0
	admin     string
)

func broadcaster() {

	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn, who string) {
	ircString := "irc-server > "
	ch := make(chan string) // outgoing client messages
	nameUsers[who] = conn
	go clientWriter(conn, ch)

	ch <- ircString + "Welcome to the Simple IRC Server"
	ch <- ircString + "Your user " + who + " is successfully logged"

	fmt.Println(ircString + "New connected user " + who)
	if numUsers == 0 {
		fmt.Println(ircString + who + " was promoted as the channel ADMIN")
		admin = who
		ch <- ircString + "Congrats, you were the first user."
		ch <- ircString + "You're the new IRC Server ADMIN"
	}

	numUsers++

	messages <- ircString + "New connected user " + who
	entering <- ch

	input := bufio.NewScanner(conn)

	for input.Scan() {
		text := input.Text()
		if strings.HasPrefix(text, "/") {
			textSplit := strings.Split(text, " ")
			if text == "/users" {
				var usernames []string
				for users := range nameUsers {
					usernames = append(usernames, users)
				}
				ch <- ircString + strings.Join(usernames, ", ")
			} else if textSplit[0] == "/msg" && len(textSplit) > 2 {
				exist := false
				for users := range nameUsers {
					if users == textSplit[1] {
						exist = true
						message := strings.Replace(text, textSplit[0]+" "+textSplit[1], "", 1)
						fmt.Fprintln(nameUsers[users], ircString+"Direct message from "+who+":"+message)
					}
				}
				if exist == false {
					ch <- ircString + "User " + textSplit[1] + " not in the server , check usernames with /users"
				}
			} else if text == "/time" {
				timezone := "America/Mexico_City"
				loc, err := time.LoadLocation(timezone)
				if err != nil {
					log.Panic(timezone + " Is an invalid timezone")
				}
				ch <- ircString + "Local time: " + timezone + " " + time.Now().In(loc).Format("15:04")
			} else if textSplit[0] == "/user" && len(textSplit) == 2 {
				exist := false
				for users := range nameUsers {
					if users == textSplit[1] {
						exist = true
						ch <- ircString + "username: " + users + ", IP: " + nameUsers[users].RemoteAddr().String()
					}
				}
				if exist == false {
					ch <- ircString + "User " + textSplit[1] + " not in the server , check usernames with /users"
				}
			} else if textSplit[0] == "/kick" && len(textSplit) == 2 {
				if who != admin {
					ch <- ircString + "You don't have admin permissions"
				} else if who == textSplit[1] {
					ch <- ircString + "You can't kick yourself"
				} else {
					exist := false
					for users := range nameUsers {
						if users == textSplit[1] {
							exist = true
							fmt.Println(ircString + users + " was kicked")
							fmt.Fprintln(nameUsers[users], ircString+"You're kicked from this channel")
							fmt.Fprintln(nameUsers[users], ircString+"Bad language is not allowed on this channel")
							messages <- ircString + users + " was kicked from channel for bad language policy violation"
							nameUsers[users].Close()
						}
					}
					if exist == false {
						ch <- ircString + "User " + textSplit[1] + " not in the server , check usernames with /users"
					}
				}
			} else if text == "/help" {
				commands := "/users : List all connected users, /msg <user> <msg> : Direct message to user \n /time : Get server localtime, /user <user> : Get more details about a user "
				if admin == who {
					ch <- ircString + commands + ", /kick <user> : To kick an user from the channel "
				} else {
					ch <- ircString + commands
				}
			} else {
				ch <- ircString + "Incomplete or Incorrect command, check /help for commands"
			}
		} else if text == "" {
		} else {
			messages <- who + " > " + text
		}
	}

	delete(nameUsers, who)
	numUsers--
	leaving <- ch
	messages <- ircString + who + " left"
	if admin == who {
		for users := range nameUsers {
			admin = users
			fmt.Fprintln(nameUsers[admin], ircString+"you was promoted to channel admin")
			fmt.Println(ircString + admin + " was promoted as the channel ADMIN")
			break
		}
	}
	conn.Close()
	// NOTE: ignoring potential errors from input.Err()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {

	if len(os.Args) < 5 || len(os.Args) > 5 || (os.Args[1]) != "-host" || (os.Args[3]) != "-port" {
		fmt.Println("Usage : go run server.go -host {host} -port {Port}")
		os.Exit(1)
	}
	hostPort := os.Args[2] + ":" + os.Args[4]

	listener, err := net.Listen("tcp", hostPort)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println("irc-server > Simple IRC Server started at " + hostPort)
	fmt.Println("irc-server > Ready for receiving new clients")

	go broadcaster()
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		who := ""
		userScan := bufio.NewScanner(conn)
		for userScan.Scan() {
			who = userScan.Text()
			break
		}
		if _, checkName := nameUsers[who]; checkName {
			fmt.Fprintln(conn, "irc-server > Username "+who+" already in use ")
			conn.Close()
		} else {
			go handleConn(conn, who)
		}
	}
}

//!-main
