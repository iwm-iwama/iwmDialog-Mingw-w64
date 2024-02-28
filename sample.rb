#!ruby
#coding:utf-8

LN = "-" * 80

def SubPrint(cmd)
	while true
		s1 = %x(#{cmd} -d=c:\\windows)
		break if s1.length == 0
		print s1.force_encoding("cp932")
	end
end

puts LN
SubPrint("iwmDirDialog.exe")
puts LN
SubPrint("iwmFileDialog.exe -l")
puts LN
