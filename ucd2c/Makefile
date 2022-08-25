
all: ucd.c

UCD: UCD.zip
	rm -rf UCD
	mkdir UCD
	cd UCD && unzip ../UCD.zip

UCD.zip:
	ftp "https://www.unicode.org/Public/UCD/latest/ucd/UCD.zip" -o UCD.zip

license.txt:
	ftp "https://www.unicode.org/license.txt" -o license.txt

ucd.c: UCD UCD/UnicodeData.txt ucd2c
	./ucd2c < UCD/UnicodeData.txt > ucd.c

include config.mk
