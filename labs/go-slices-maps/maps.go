package main

import (
	"golang.org/x/tour/wc"
	"strings"
)

func WordCount(s string) map[string]int {
	word := strings.Fields(s)
	sMap := make(map[string]int)
	for i := 0;i < len(word); i++ {
		sMap[word[i]]++
	}
	return sMap
}

func main() {
	wc.Test(WordCount)
}
