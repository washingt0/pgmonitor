package procs

// #include <unistd.h>
import "C"

import (
	"bytes"
	"io/ioutil"
	"path"
	"strconv"
)

// ProcessInfo contains a running process info
type ProcessInfo struct {
	PID        int
	Name       string
	UTime      uint64
	STime      uint64
	CUTime     uint64
	CSTime     uint64
	StartTime  uint64
	CPUPercent int
	VMSize     uint64
}

// GetProcessInfo retrieves data about a process by id
func GetProcessInfo(basePath string, pid int) (proc *ProcessInfo, err error) {
	var (
		seconds uint64
		uptime  float64
	)

	proc = &ProcessInfo{PID: pid}

	if err = getCPUInfo(&basePath, proc); err != nil {
		return
	}

	if uptime, err = getUptime(&basePath); err != nil {
		return
	}

	seconds = uint64(uptime) - (proc.StartTime / uint64(C.sysconf(C._SC_CLK_TCK)))

	proc.CPUPercent = int((100 * ((float64(proc.UTime+proc.STime+proc.CUTime+proc.CSTime) / float64(C.sysconf(C._SC_CLK_TCK))) / float64(seconds))))

	return
}

func getCPUInfo(basePath *string, proc *ProcessInfo) (err error) {
	var (
		f    []byte
		data [][]byte
	)

	f, err = ioutil.ReadFile(path.Join(*basePath, strconv.Itoa(proc.PID), "stat"))
	if err != nil {
		return
	}

	var s, e int
	for i := range f {
		switch f[i] {
		case '(':
			s = i
		case ')':
			e = i + 1
			break
		}
	}

	proc.Name = string(f[s:e])
	data = bytes.Split(f[e+1:], []byte(" "))

	if proc.UTime, err = strconv.ParseUint(string(data[11]), 10, 64); err != nil {
		return
	}

	if proc.STime, err = strconv.ParseUint(string(data[12]), 10, 64); err != nil {
		return
	}

	if proc.CUTime, err = strconv.ParseUint(string(data[13]), 10, 64); err != nil {
		return
	}

	if proc.CSTime, err = strconv.ParseUint(string(data[14]), 10, 64); err != nil {
		return
	}

	if proc.StartTime, err = strconv.ParseUint(string(data[19]), 10, 64); err != nil {
		return
	}

	if proc.VMSize, err = strconv.ParseUint(string(data[20]), 10, 64); err != nil {
		return
	}

	return
}

func getUptime(basePath *string) (uptime float64, err error) {
	var (
		f []byte
	)

	f, err = ioutil.ReadFile(path.Join(*basePath, "uptime"))
	if err != nil {
	}

	return strconv.ParseFloat(string(bytes.Split(f, []byte(" "))[0]), 64)
}
