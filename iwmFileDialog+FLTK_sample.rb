#!ruby
#coding:utf-8

CMD = "iwmFileDialog+FLTK.exe"
LN = "-" * 80

def SubDialog()
	while true
		s1 = %x(#{CMD} -type=mf -cp=65001)
		break if s1.length == 0
		print s1
	end
end

puts LN
SubDialog()
puts LN
