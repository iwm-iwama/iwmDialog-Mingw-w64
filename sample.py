#!python3
#coding:utf-8

import subprocess

LN = "-" * 80

def SubPrint(cmd):
	subprocess.run(cmd + " -h", shell=True)
	while(True):
		child = subprocess.Popen(cmd + " -d=c:\\windows", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		stdout, stderr = child.communicate()
		if(len(stdout) == 0):
			break
		print(stdout.decode(), end="")
		print(LN)

SubPrint("iwmDirDialog.exe")
SubPrint("iwmFileDialog.exe")
