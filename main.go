package main

import (
	"log"
	"pgmonitor/procs"
)

func main() {
	log.SetFlags(log.LstdFlags | log.Lshortfile)
	var (
		err error
	)

	if err = procs.WalkProcess("/proc", 35, 1073741824); err != nil {
		log.Fatal(err)
	}
}
