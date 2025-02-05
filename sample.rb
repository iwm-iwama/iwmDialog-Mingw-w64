#!ruby
#coding:utf-8

CMD = ".\\iwmFileDialog+FLTK.exe -t=m"

def SubDialog()
	while true
		s1 = %x(#{CMD})
		break if s1.length == 0
		print s1
	end
end

SubDialog()
