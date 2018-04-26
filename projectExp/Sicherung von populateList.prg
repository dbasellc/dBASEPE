alist = new array()
aList.dir("C:\Program Files (x86)\dBASE\Plus12\inno\Images\*.png")
cArray = "ARRAY { "
?aList[1,1]
?alist.size
for x = 1 to alist.size
cname = aList[x,1]
cArray += ["]+cname+[",]
endfor
cArray = LEFT(cArray,LEN(cArray)-1)
//?cArray