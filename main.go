package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
)

func compiler() string {
	return "g++ -std=c++20"
}

func cmd(c string) string {
	out, err := exec.Command("bash", "-c", c).Output()
	if err != nil {
		log.Fatal(err)
	}
	return string(out)
}

func split(s string) []string {
	var lines []string
	sc := bufio.NewScanner(strings.NewReader(s))
	for sc.Scan() {
		lines = append(lines, sc.Text())
	}
	return lines
}

func find(files []string) ([]string, []string, []string) {
	var all []string
	var cpp []string
	var hpp []string
	for _, c := range files {
		ext := filepath.Ext(c)
		if ext == ".cpp" {
			cpp = append(cpp, c)
			all = append(all, c)
		} else if ext == ".hpp" {
			hpp = append(hpp, c)
			all = append(all, c)
		}
	}
	return all, cpp, hpp
}

func binary() string {
	path, err := os.Getwd()
	if err != nil {
		log.Println(err)
	}
	_, name := filepath.Split(path)
	return name
}

func path(name string) string {
	return ".obj/" + name + ".o"
}

func tab(count int) string {
	o := "\t"
	for count > 1 {
		o += "\t"
		count--
	}
	return o
}

func main() {
	dep := make(map[string][]string)
	files := split(cmd("ls"))
	all, cpp, _ := find(files)
	for fi, f := range all {
		lines := split(cmd("cat " + f))
		var inc []string
		for _, l := range lines {
			for oi, o := range all {
				if fi != oi {
					if strings.Contains(l, o) {
						inc = append(inc, o)
						break
					}
				}
			}
		}
		dep[f] = inc
	}
	fmt.Println("# Generated with github.com/da0x/mmpp")
	fmt.Println("# Binary:")
	fmt.Printf(binary() + ":")
	fmt.Printf(" .obj")
	for _, f := range cpp {
		fmt.Printf(" %v", path(f))
	}
	fmt.Printf("\n\t" + compiler())
	for _, f := range cpp {
		fmt.Printf(" %v", path(f))
	}
	fmt.Printf(" -o %v", binary())
	fmt.Println()
	fmt.Println()
	fmt.Println(".obj:\n\tmkdir .obj\n")
	for _, c := range cpp {
		fmt.Printf(path(c) + ": " + c)
		for _, d := range dep[c] {
			fmt.Printf(" %v", d)
		}
		fmt.Println()
		fmt.Println(tab(1) + compiler() + " -o " + path(c) + " -c " + c)
		fmt.Println()
	}
	fmt.Println("run: " + binary())
	fmt.Println("\t./" + binary())
	fmt.Println("")
	fmt.Println("clean:")
	fmt.Println("\trm -rfv .obj")
	fmt.Println("")
	fmt.Println("install:")
	fmt.Println("\tsudo cp -v " + binary() + " /usr/local/bin/")
	fmt.Println("")
	fmt.Println("uninstall:")
	fmt.Println("\tsudo rm -v /usr/local/bin/" + binary())
}
