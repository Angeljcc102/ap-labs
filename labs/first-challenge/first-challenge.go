package main

import "fmt"

func main() {
	fmt.Print("Enter String: ")
	var word string
	fmt.Scanln(&word)
	var length = 1
	var cont = 0
	var maxLength = 0

	for i := 1; i < len(word); i++ {
		for j := cont; j < i; j++ {
			if word[i] != word[j] {
				if word[j] == word[i-1] {
					length++
				}
				if word[i] == word[len(word)-1] {
					maxLength = length
				}
			} else {
				cont = i
				j = i
				if length > maxLength {
					maxLength = length
				}
				length = 1
			}
		}
	}
	fmt.Println(maxLength)
}
