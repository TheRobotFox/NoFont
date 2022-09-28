from PIL import Image
import sys
import numpy

if len(sys.argv)!=3:
	print("Usage: [INPUT IMG] [CHARSET]")
	exit(0)

name = (sys.argv[1].replace("\\","/").split("/")[-1].split(".")[0]).lower()
path = "/".join(sys.argv[0].replace("\\","/").split("/")[:-1])
output = "Font."+name+".h"

settings = {"h_kern" : ["Horizontal Kerning multiplier", 1.4],
			"v_kern" : ["Vertical Kerning multiplier", 1.4],
			"space_width" : ["Space width in units", 3]}

def getChar(img, start):
	char = []
	count = 0
	x=start
	while x < len(img[0]):
		for y in range(len(img)):
			if not img[y][x]:
				count=0
				break
		else:
			count+=1
			if count==2:
				break

		x+=1
	else:
		print("ERROR: INPUT FALFOREMD")
		exit(0)
	return x+1

try:
	img = numpy.asarray(Image.open(sys.argv[1]).convert("1"))
except Exception as e:
	print(e)
	print("Error: Could not open Image!")
	exit(0)

try:
	header = open(output, "w") 
except Exception as e:
	print(e)
	print("Error: Could not open Header!")
	exit(0)


chars = []
Bytes = []

start = 0
while start<len(img[0])-1:
	end = getChar(img, start)-2
	print(end)
	Bytes_start = len(Bytes)
	num = 0
	b_index = 0
	for y in range(len(img)):
		for x in range(start, end):
			num+=int(not img[y][x])

			b_index+=1
			if b_index>=8:
				Bytes.append(hex(num))
				b_index=0
				num=0
				continue
			num<<=1

	if b_index:
		Bytes.append(hex(num))

	char = {"width" : end-start, "data" : len(Bytes)-Bytes_start}
	chars.append(char)
	start=end+2



if len(chars)!=len(sys.argv[2]):
	print(f"Length does not match: Chars[{len(chars)}] CHARSET[{len(sys.argv[2])}]")


#header guard
header.write("#pragma once\n\n")

#write Settings
for key in settings:
	header.write(f"#define {key.upper()} {str(settings[key][1])} \t// {settings[key][0]}\n")

#include driver
header.write(f"\n#include \"{path}/Font_Driver.h\"\n")


#write raw buffer
header.write("const unsigned char raw[" + str(len(Bytes)) + "] = {")
header.write(", ".join(Bytes))
header.write("};\n\n")

offset = 0
out_chars = []
for C in chars:
	out_chars.append("\t{" + str(C["width"]) + ", " + str(len(img)) + ", raw+"+ str(offset) + "}")
	offset+=C["data"]

#define chars
header.write("const struct Font_Char charset["+str(len(chars))+"] = {\n")
header.write(",\n".join(out_chars))
header.write("};\n\n")


#create charset map
print([i for i in sys.argv[2]])
ords = [ord(i) for i in sys.argv[2]]
largest = max(ords)

maping = []
for i in range(largest+1):
	if i in ords:
		maping.append(f"charset+{ords.index(i)} /* " + chr(i) + "|" + str(i) + " */")
	else:
		maping.append("0")


header.write("struct Font "+name+" = {charset, {")
header.write(", ".join(maping)+"}, "+ str(len(img))+", ")
header.write(", ".join([i.upper() for i in settings]))
header.write("};\n\n")

#Preproc cleanup
for key in settings:
	header.write(f"#undef {key.upper()}\n")

header.write("#undef raw\n")
header.write("#undef charset\n")
header.write("#undef map\n")


header.close()

print("Created " + output)
print("edit font settings in header file")