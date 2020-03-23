package main

import (
	"fmt"
	"os"
	"path/filepath"
)

type TypeStruct struct {
	numDir      int
	numSymLinks int
	numDevices  int
	numSockets  int
	numOthers   int
}

// scanDir stands for the directory scanning implementation
func scanDir(dir string) error {

	var fileTypes = TypeStruct{}

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

	fmt.Println("Directory Scanner Tool")
	fmt.Println("+-----------------------+")
	fmt.Println("|Path                   | " + dir)
	fmt.Println("+-----------------------+")
	fmt.Printf("|Directories            | %d\n", fileTypes.numDir)
	fmt.Printf("|Symbolic links         | %d\n", fileTypes.numSymLinks)
	fmt.Printf("|Devices                | %d\n", fileTypes.numDevices)
	fmt.Printf("|Sockets                | %d\n", fileTypes.numSockets)
	fmt.Printf("|Other files            | %d\n", fileTypes.numOthers)
	fmt.Println("+-----------------------+")

	return nil
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}

	scanDir(os.Args[1])

}
