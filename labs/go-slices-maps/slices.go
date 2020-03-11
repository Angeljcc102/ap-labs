package main

import "golang.org/x/tour/pic"

func Pic(dx, dy int) [][]uint8 {
	var a [][]uint8
	a = make([][]uint8,dx,dy)
	for i:= 0; i < dx; i++{
		a[i] = make([]uint8,dx,dy)
		for j:= 0; j < dy; j++{
			a[i][j] = uint8(i^j)
		}
	}
	return a
}

func main() {
	pic.Show(Pic)
}
