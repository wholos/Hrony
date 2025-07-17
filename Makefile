NIM=nim
NIMFLAGS=c -o:hrony

all:
	@$(NIM) $(NIMFLAGS) src/main.nim

clean:
	@rm hrony
