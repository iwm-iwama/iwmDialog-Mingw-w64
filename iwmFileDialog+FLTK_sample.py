#!python3
#coding:utf-8

import subprocess

CMD = "iwmFileDialog+FLTK.exe"
LN = "-" * 80

def SubDialog():
	while(True):
		child = subprocess.Popen(
			CMD + " -t=mf -cp=65001",
			shell=True,
			stdout=subprocess.PIPE,
			stderr=subprocess.PIPE
		)
		stdout, stderr = child.communicate()
		if(len(stdout) == 0):
			break
		print(stdout.decode("cp65001"), end="")

print(LN)
SubDialog()
print(LN)
