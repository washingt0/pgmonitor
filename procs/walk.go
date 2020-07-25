package procs

import (
	"io/ioutil"
	"log"
	"os"
	"regexp"
	"strconv"
)

// WalkProcess a proc-like directory and get process info
func WalkProcess(basePath string, cpu int, mem uint64) (err error) {
	var (
		files       []os.FileInfo
		re          *regexp.Regexp = regexp.MustCompile(`[0-9]{1,}`)
		_pid, _ppid int            = os.Getpid(), os.Getppid()
	)

	if files, err = ioutil.ReadDir(basePath); err != nil {
		return
	}

	for i := range files {
		if !(files[i].IsDir() && re.Match([]byte(files[i].Name()))) {
			continue
		}

		var (
			proc *ProcessInfo
			pid  int
		)

		if pid, err = strconv.Atoi(string(files[i].Name())); err != nil {
			return
		}

		if pid == _pid || pid == _ppid {
			continue
		}

		if proc, err = GetProcessInfo(basePath, pid); err != nil {
			return
		}

		if !(proc.CPUPercent > cpu || proc.VMSize > mem) {
			continue
		}

		log.Printf("%s %d%% %dM\n", proc.Name, proc.CPUPercent, proc.VMSize/1024/1024)
	}

	return
}
