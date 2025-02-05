#!python3
#coding:utf-8

import subprocess

CMD = ".\\iwmFileDialog+FLTK.exe -t=m"

def SubDialog():
	while(True):
		child = subprocess.Popen(
			CMD,
			shell=True,
			stdout=subprocess.PIPE,
			stderr=subprocess.PIPE
		)
		stdout, stderr = child.communicate()
		if(len(stdout) == 0):
			break
		# 重要!! 出力コード = cp65001
		print(stdout.decode("cp65001"), end="")

SubDialog()
