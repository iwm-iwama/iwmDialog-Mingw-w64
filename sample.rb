#!ruby
#coding:utf-8

LN = "-" * 80

def SubPrint(cmd)
	print %x(#{cmd} -h)
	while true
		s1 = %x(#{cmd} -d=c:\\windows)
		break if s1.length == 0
		print s1
		puts LN
	end
end

SubPrint("iwmDirDialog.exe")
SubPrint("iwmFileDialog.exe")
