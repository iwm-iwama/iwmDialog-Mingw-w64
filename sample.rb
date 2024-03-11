#!ruby
#coding:utf-8

LN = "-" * 80

def SubPrint(cmd)
	while true
		s1 = %x(#{cmd} -cp=65001)
		break if s1.length == 0
		print s1
	end
end

puts LN
SubPrint("iwmDirDialog.exe")
puts LN
SubPrint("iwmFileDialog.exe")
puts LN
