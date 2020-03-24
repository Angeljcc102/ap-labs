package main

import (
	"fmt"
	"os"
	"path/filepath"
	"strconv"
	"strings"
)

type typeStruct struct {
	numDir      int
	numSymLinks int
	numDevices  int
	numSockets  int
	numOthers   int
}

// scanDir stands for the directory scanning implementation
func scanDir(dir string) error {

	var fileTypes = typeStruct{}

	err := filepath.Walk(dir, func(path string, info os.FileInfo, err error) error {
		if err != nil {
			fmt.Printf("%q: %v\n", path, err)
			return err
		}

		fileMode := os.FileMode.String(info.Mode())

		if fileMode[0] == 100 {
			fileTypes.numDir++
		} else if fileMode[0] == 76 {
			fileTypes.numSymLinks++
		} else if fileMode[0] == 68 {
			fileTypes.numDevices++
		} else if fileMode[0] == 83 {
			fileTypes.numSockets++
		} else {
			fileTypes.numOthers++
		}

		return nil
	})

	if err != nil {
		fmt.Printf("error walking the path %q: %v\n", dir, err)
		return nil
	}

	var sizeString = len(dir) + 2 - len(strconv.Itoa(fileTypes.numDir))

	fmt.Println("Directory Scanner Tool")
	fmt.Println("+-----------------------+" + strings.Repeat("-", len(dir)+3) + "+")
	fmt.Printf("|Path                   |  %s |\n", dir)
	fmt.Println("+-----------------------+" + strings.Repeat("-", len(dir)+3) + "+")
	fmt.Printf("|Directories            | %d%s\n", fileTypes.numDir, strings.Repeat(" ", sizeString)+"|")
	sizeString = len(dir) + 2 - len(strconv.Itoa(fileTypes.numSymLinks))
	fmt.Printf("|Symbolic links         | %d%s\n", fileTypes.numSymLinks, strings.Repeat(" ", sizeString)+"|")
	sizeString = len(dir) + 2 - len(strconv.Itoa(fileTypes.numDevices))
	fmt.Printf("|Devices                | %d%s\n", fileTypes.numDevices, strings.Repeat(" ", sizeString)+"|")
	sizeString = len(dir) + 2 - len(strconv.Itoa(fileTypes.numSockets))
	fmt.Printf("|Sockets                | %d%s\n", fileTypes.numSockets, strings.Repeat(" ", sizeString)+"|")
	sizeString = len(dir) + 2 - len(strconv.Itoa(fileTypes.numOthers))
	fmt.Printf("|Other files            | %d%s\n", fileTypes.numOthers, strings.Repeat(" ", sizeString)+"|")
	fmt.Println("+-----------------------+" + strings.Repeat("-", len(dir)+3) + "+")

	return nil
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}

	scanDir(os.Args[1])

}
