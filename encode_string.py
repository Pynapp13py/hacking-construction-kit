import	os
import	base64
import	sys
import	fileinput
import	re

# This script will encode strings in .cpp et .h with RC4
# it will first search for the key CRYPTED_STRING_KEY in the sources
# then it'll search for SnE("mystring", "")and translate it in SnE("aedqsdqsdf", "mystring")
# so in the final .exe the strings will be encrypted


def	replaceAll(file, searchExp, key):
	for	line in	fileinput.input(file, inplace=	1):
		mo = re.search(searchExp, line)
		if mo:
			source = mo.group(1)
			encodedAsBytes = WikipediaARC4(key).crypt(source)
			encodedAsB64 = encode(encodedAsBytes)
			replaceExp = "SnE(\"" + str(encodedAsB64, "utf-8") + "\", \"" + repr(source) + "\")"
			line = re.sub(searchExp, replaceExp, line)
		sys.stdout.write(line)

def	searchInFile(file, 	searchExp):
	for	line in	fileinput.input(file):
		mo = re.search(searchExp, line)
		if mo:
			fileinput.close()
			return mo.group(1)


class WikipediaARC4:
	def __init__(self, key=None):
		self.state = list(range(256))
		self.x = self.y = 0

		if key is not None:
			self.init(key)

	# Key schedule
	def init(self, key):
		for i in range(256):
			self.x = (ord(key[i % len(key)]) + self.state[i] + self.x) & 0xFF
			self.state[i], self.state[self.x] = self.state[self.x], self.state[i]
			self.x = 0

	def crypt(self, inputs):
		output = [None] * len(inputs)
		for i in range(len(inputs)):
			self.x = (self.x + 1) & 0xFF
			self.y = (self.state[self.x] + self.y) & 0xFF
			self.state[self.x], self.state[self.y] = self.state[self.y], self.state[self.x]
			temp = (ord(inputs[i]) ^ self.state[(self.state[self.x] + self.state[self.y]) & 0xFF])
			output[i] = chr((ord(inputs[i]) ^ self.state[(self.state[self.x] + self.state[self.y]) & 0xFF]))
		return "".join(output)

def encode(sh):
	b = bytes(sh, "latin-1")
	b64 = base64.b64encode(b)
	return b64



def	walkDir(key):
	for	root, dirs, files in os.walk("."):
		for	file in	files:
			if	file.endswith(".cpp"):
				strng	 = os.path.join(root, file)
				print("parsed: " + strng)
				found =	searchInFile(strng, "SnE\(\"(.*)\", \"\"\)");
				if found:
					replaceAll(strng, "SnE\(\"(.*)\", (\"\")\)", key)


def	searchForKey():
	for	root, dirs, files in os.walk("."):
		for	file in files:
			if file.endswith(".h"):
				strng = os.path.join(root, file)
				found =	searchInFile(strng, "#define CRYPTED_STRING_KEY \"(\w*)\"$");
				if found:
					print("key for string encoding: " + found)
					return found


if	__name__	 == 	"__main__":
	print("start encoding strings")
	key = searchForKey()
	if key:
		walkDir(key)
