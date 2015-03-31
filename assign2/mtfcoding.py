#!/opt/bin/python3


import os
import sys
import logging

logging.basicConfig(filename='mtfcoding.log',filemode='w', format='%(asctime)s: %(message)s', level=logging.DEBUG)


def log(reason, error):
	if error == True:
		logging.warning("Error reason: "+reason)
		assert False, "Error found, check mtfcoding.log for details"
	else:
		logging.info(reason)
	

def encode_main():
	print("In encode_main")
	log("In encode_main", False)
	name = sys.argv[1]
	file_in = open(name, 'r')
	if file_in == None:
		reason = 'Nothing read in from file: ' + name
		log (reason, True)
	reason = 'Name of file read is ' +name
	log(reason, False)
	out = name.replace('txt','mtf', 1)
	out = "../testresults/" + out
	file_out = open(file=out, encoding='latin-1' , mode="w")
	reason = 'Output file named ' + out + ' created'
	log(reason, False)
	words = []
	count = 1
	logcount = 1
	file_out.write('\xfa\xce\xfa\xde')
	log("Wrote mtf magic number", False)
	for line in file_in:
		reason = "Encoding line " + str(logcount) + " of in file"
		log(reason, False)
		logcount = logcount + 1
		umm = line
		l = umm.split()
		for k in l:	 
			the = k
			if k not in words:
				foo = count+128
				file_out.write(chr(foo))
				file_out.write(the)
				words.insert(0,the)
				count = count +1
			else:
				pos = words.index(the) + 129
				file_out.write(chr(pos))
				words.remove(k)
				words.insert(0, the)
		file_out.write('\n');
	log('Completed file encoding', False)
	print("Output file name is: " +out)
	print ("File encoded successfully")
	log("File encoded successfully", False)
	file_in.close()
	file_out.close()

def decode_main():
	print("In decode_main")
	log("In decode_main", False)
	name = sys.argv[1]
	file_in = open(name, 'rb')
	if file_in == None:
		log('Nothing read from input file', True)
	log('Input file read', False)
	out = name.replace('mtf','txt', 1)	
	out = "../testresults/" + out
	file_out = open(out, 'w')
	print ("Output file name now equals = "+out)
	reason = 'Output file name is: ' +out
	log(reason, False)
	words = []
	count = 1
	wud = ""
	logcount = 1
	check = file_in.read(4)
	if check == b'\xfa\xce\xfa\xde':
		log("Checked input file contains correct mtf magic numbers", False)
	else:
		log ("The input file was not a not a mtf file, cannot decode", True)
	for line in file_in:
		reason = "Decoding line "+ str(logcount) + " of in file"
		log(reason, False)
		logcount = logcount+1
		umm = line
		i = bytearray(umm)
		lencount = 1
		for j in i:
			tots = len(i) - 1
			num = ord(chr(j))
			if num > 128:
				if wud != "":
					words.insert(0,wud)
					if lencount == tots+1:
						file_out.write(wud)
					else:
						file_out.write(wud + " ")
					wud = ""
					count = count + 1
				pos = num - 128
				if pos < count:	
					temp = words[pos-1]
					words.remove(words[pos-1])
					if lencount == tots:
						file_out.write(temp)
					else:
						file_out.write(temp + " ")
					words.insert(0, temp)
			elif num < 128 and num != 10:
				wud += str(chr(j))
			else:
				if wud != "":
					words.insert(0,wud)
					file_out.write(wud)
					count = count + 1
					wud = ""
			lencount = lencount + 1
		file_out.write('\n')
	print ("File decoded successfully")
	log("File decoded successfully", False)
	file_in.close()
	file_out.close()
	


command = os.path.basename(__file__)
if __name__ == "__main__" and command == "mtfencode.py":
	if len(sys.argv) < 2:
		log("No file arguments passed in", True)
	encode_main()
elif __name__ == "__main__" and command == "mtfdecode.py":
	if len(sys.argv) < 2:
		log("No File arguments passed in", True)
	decode_main()
