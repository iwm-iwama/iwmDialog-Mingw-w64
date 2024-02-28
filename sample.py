#!python3
#coding:utf-8

import subprocess

LN = "-" * 80

def SubPrint(cmd):
	while(True):
		child = subprocess.Popen(cmd + " -d=c:\\windows", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		stdout, stderr = child.communicate()
		if(len(stdout) == 0):
			break
		print(stdout.decode("cp932"), end="")

print(LN)
SubPrint("iwmDirDialog.exe")
print(LN)
SubPrint("iwmFileDialog.exe -l")
print(LN)
