#!/opt/bin/python3


import os
import sys
import logging

logging.basicConfig(filename='mtfcoding2.log', format='%(asctime)s: %(message)s', level=logging.DEBUG)

MAGIC_NUMBER1 = [chr(0xFA) , chr(0xDE) , chr(0xFA) , chr(0xDE)]
MAGIC_NUMBER2 = [chr(0xFA) , chr(0xDE) , chr(0xFA) , chr(0xDF)]
class encode:
	
	def __init__(self, in_filename, dest = "", prog=False):
		print("In encode_main")
		log("In encode_main", False)
		checkin(in_filename, ".txt")
		name = in_filename
		file_in = open(name, 'r')
		if file_in == None:
			reason = 'Nothing read in from file: ' + name
			log (reason, True)
		reason = 'Name of file read is ' +name
		log(reason, False)
		send = "Encoding: " +name
		log(send, False)
		out = name.replace('txt','mtf', 1)
		out = dest + out
		file_out = open(file=out , mode="wb")
		reason = 'Output file named ' + out + ' created'
		log(reason, False)
		words = []
		count = 1
		uniquecount = 0
		totalcount = 0
		logcount = 1
		file_out.write(b'\xfa\xce\xfa\xdf')
		log("Wrote mtf magic number", False)
		for line in file_in:
			if prog == True:
				reason = "Encoding line " + str(logcount) + " of in file"
				log(reason, False)
			logcount = logcount + 1
			umm = line
			l = umm.split()
			for k in l:	 
				the = k
				totalcount = totalcount +1
				if k not in words:
					foo = to_byte(count)
					file_out.write(foo)
					file_out.write(the.encode('latin1'))
					words.insert(0,the)
					count = count +1
					uniquecount = uniquecount + 1
				else:
					pos = words.index(the) + 1
					file_out.write(to_byte(pos))
					words.remove(k)
					words.insert(0, the)
			file_out.write('\n'.encode('latin1'));
		log('Completed file encoding', False)
		print("Output file name is: " +out)
		print ("File encoded successfully")
		log("File encoded successfully", False)
		send = "Number of lines encoded: "+str(logcount-1)
		log(send, False)
		send = "Number of total words encoded: " + str(totalcount)
		log(send, False)
		send = "Number of total unique words encoded: " + str(uniquecount)
		log(send, False)
		file_in.close()
		file_out.close()

def to_byte(foo):
	out = bytearray()
	if foo < 121:
		foo = foo + 128
		out.append(foo)
		return out
	elif foo < 376:
		front = 249
		back = foo - 121
		out.append(front)
		out.append(back)
		return out
	elif foo < 65913:
		front = 250
		mid = ((foo-376)//256)
		end = ((foo-376)%256)
		out.append(front)
		out.append(mid)
		out.append(end)
		return out
	else:
		log("Too many unique words in file", True)




def log(reason, error):
	if error == True:
		logging.warning("Error reason: "+reason)
		assert False, "Error found, check mtfcoding.log for details"
	else:
		logging.info(reason)
	

class decode:

	def __init__(self, filename, dest="", prog=False):
		print("In decode_main")
		log("In decode_main", False)
		checkin(filename, ".mtf")
		name = filename
		send = "Decoding: " +name
		log(send, False)
		file_in = open(name, encoding='latin1', mode='r', newline="")
		if file_in == None:
			log('Nothing read from input file', True)
		log('Input file read', False)
		out = name.replace('mtf','txt', 1)
		out = dest + out
		file_out = open(out, 'w')
		print ("Output file name now equals = "+out)
		reason = 'Output file name is: ' +out
		log(reason, False)
		words = []
		wud = ""
		ca = file_in.read(4)
		check_mag(ca)
		nums = []
		allwords = []
		skip1 = False
		skip2 = False
		seen = False
		whole = file_in.read()
		whole = bytearray(whole.encode('latin1'))
		for j in range(len(whole)):
			if skip1 == True:
				skip1 = False
				continue
			elif skip2 == True:
				skip2 = False
				continue
			else:
				char = ord(chr(whole[j]))
				if char > 128:
					if wud != "":
						allwords.append(wud)
						wud = ""
					if char == 249:
						num = to_int([char, ord(chr(whole[j+1]))])
						skip1 = True
						skip2 = False
						
					elif char == 250:
						num = to_int([char, ord(chr(whole[j+1])), ord(chr(whole[j+2]))])
						skip1 = True
						skip2 = True
					else:
						num = char - 128
					nums.append(num)
					continue
				elif char == 10:
					if wud != "":
						allwords.append(wud)
						wud = ""
					nums.append(chr(char))
				else:
					wud = wud +chr(whole[j])	
		words = []
		uniquelen = len(allwords)
		totalcount = 0
		used = 0
		linecount = 0
		for k in range(len(nums)):
			if prog == True:
				send = "Decoding at position " +str(k) 
				log(send, False)
			if nums[k] == '\n':
				file_out.write(nums[k])
				linecount = linecount + 1
			elif nums[k] > used:
				foo = allwords.pop(0)
				words.insert(0, foo)
				file_out.write(foo)
				used = used + 1
				totalcount = totalcount + 1
				if nums[k+1] != '\n':
					file_out.write(" ")
			else:
				pos = nums[k] - 1
				file_out.write(words[pos])
				if nums[k+1] != '\n':
					file_out.write(" ")
				repl = words.pop(pos)
				words.insert(0, repl)
				totalcount = totalcount + 1
			

		print ("File decoded successfully")
		log("File decoded successfully", False)
		send = "Number of lines decoded: " + str(linecount)
		log(send, False)
		send = "Number of total words decoded: " + str(totalcount)
		log(send, False)
		send = "Number of total unique words decoded: " + str(uniquelen)
		log(send, False)
		file_in.close()
		file_out.close()



def checkin(got, expected):
	if got.endswith(expected):
		log("Is proper input file", False)
	else:
		log("Is not proper input file", True)


def check_mag(foo):
	if foo[0] == MAGIC_NUMBER1[0] or foo[0] == MAGIC_NUMBER2[0]:		
		if foo[1] == MAGIC_NUMBER1[1] or foo[1] == MAGIC_NUMBER2[1]:		
			if foo[2] == MAGIC_NUMBER1[2] or foo[2] == MAGIC_NUMBER2[2]:		
				if foo[3] == MAGIC_NUMBER1[3] or foo[3] == MAGIC_NUMBER2[3]:		
					print("Correct magic numbers")
					log("Correct magic numbers", False)
	else:
		log ("Incorrect Magic Numbers", True)
	


def to_int(fun):	
	code = fun[1]
	if len(fun) == 2:
		out = code + 121
		return out
	code2 = fun[2]
	if len(fun) == 3:
		ret = int((((code)*256)+376)) + code2
		#if fun[2] == 0:
		#	return ((code*256)+376)
		return ret
		
		


command = os.path.basename(__file__)
if __name__ == "__main__" and command == "mtfencode.py":
	if len(sys.argv) < 2:
		log("No file arguments passed in", True)
	encode_main()
elif __name__ == "__main__" and command == "mtfdecode.py":
	if len(sys.argv) < 2:
		log("No File arguments passed in", True)
	decode_main()
