// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
	"time"
)

type Point struct{ x, y float64 }

func (p Point) X() float64 {
	return p.x
}

func (p Point) Y() float64 {
	return p.y
}

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := range path {
		if i > 0 {
			sum += path[i-1].Distance(path[i])
		}
	}
	return sum
}

func onSegment(p Point, q Point, r Point) bool {

	if q.X() <= math.Max(p.X(), r.X()) && q.X() >= math.Min(p.X(), r.X()) && q.Y() <= math.Max(p.Y(), r.Y()) && q.Y() >= math.Min(p.Y(), r.Y()) {
		return true
	}

	return false
}

func orientation(p Point, q Point, r Point) int {
	val := (q.Y()-p.Y())*(r.X()-q.X()) - (q.X()-p.X())*(r.Y()-q.Y())
	if val == 0 {
		return 0
	}

	if val > 0 {
		return 1
	}

	return 2
}

func doIntersect(p1 Point, q1 Point, p2 Point, q2 Point) bool {
	o1 := orientation(p1, q1, p2)
	o2 := orientation(p1, q1, q2)
	o3 := orientation(p2, q2, p1)
	o4 := orientation(p2, q2, q1)

	if o1 != o2 && o3 != o4 {
		return true
	}

	if o1 == 0 && onSegment(p1, p2, q1) {
		return true
	}
	if o2 == 0 && onSegment(p1, q2, q1) {
		return true
	}
	if o3 == 0 && onSegment(p2, p1, q2) {
		return true
	}
	if o4 == 0 && onSegment(p2, q1, q2) {
		return true
	}

	return false
}

func vertexGenerator(vertex Path, numSides int) []Point {

	source := rand.NewSource(time.Now().UnixNano())
	rand := rand.New(source)

	for i := 0; i < numSides-1; i++ {
		signx := rand.Intn(2)
		if signx == 1 {
			vertex[i].x = rand.Float64() * 100
		} else {
			vertex[i].x = rand.Float64() * -100
		}
		signy := rand.Intn(2)
		if signy == 1 {
			vertex[i].y = rand.Float64() * 100
		} else {
			vertex[i].y = rand.Float64() * -100
		}
	}
	vertex[numSides-1] = vertex[0]
	return vertex
}

func main() {

	if len(os.Args) < 2 || len(os.Args) > 2 {
		fmt.Println("Usage ./geometry [number of points]")
		os.Exit(1)
	}

	numSides, err := strconv.Atoi(os.Args[1])
	if numSides <= 2 {
		fmt.Printf("- A figure with [%d] sides can't be generated\n", numSides)
	} else if err == nil {
		fmt.Printf("- Generating a [%d] sides figure\n", numSides)
		fmt.Println("- Figure's vertices")

		vertex := make(Path, numSides+1)
		vertex = vertexGenerator(vertex, numSides+1)

		for i := 0; i < numSides-3; i++ {
			if doIntersect(vertex[i], vertex[i+1], vertex[i+2], vertex[i+3]) == true {
				println("Intersect between line segments")
				os.Exit(1)
			} else {

			}
		}

		for j := 0; j < numSides; j++ {
			fmt.Printf(" - (  %f,   %f)\n", vertex[j].X(), vertex[j].Y())
		}

		fmt.Println("- Figure's Perimeter")
		fmt.Printf("- %f\n", vertex.Distance())
	}

}
