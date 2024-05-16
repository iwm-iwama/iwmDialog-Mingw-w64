#!python3
#coding:utf-8

import subprocess

LN = "-" * 80

def SubPrint(cmd):
	while(True):
		child = subprocess.Popen(cmd + " -type=mf -cp=65001", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		stdout, stderr = child.communicate()
		if(len(stdout) == 0):
			break
		print(stdout.decode("cp65001"), end="")

print(LN)
SubPrint("iwmFileDialog+FLTK.exe")
print(LN)
