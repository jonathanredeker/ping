CC	= lcc -Wa-l -Wl-m -Wl-j

BINS	= bin/main.gb

all:	$(BINS)

make.bat: Makefile
	@echo "REM Automatically generated from Makefile" > make.bat
	@make -sn | sed y/\\//\\\\/ | grep -v make >> make.bat

# Compile and link single file in one pass
bin/%.gb:	src/%.c
	$(CC) -o $@ $<

clean:
	cd bin && rm -f *.o *.lst *.map *.gb *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm *.noi

