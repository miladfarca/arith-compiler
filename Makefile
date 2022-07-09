ifdef arm64
	VPATH=${SRC_PATH}:./src:./src/arm64/
else
	VPATH=${SRC_PATH}:./src:./src/x64/
endif	
CC=gcc
CFLAGS=-Wall
ODIR=obj
_OBJ = main.o test.o flags.o parser.o ast.o peephole.o codegen.o
ifdef arm64
	_OBJ += codegen-arm64.o
else
	_OBJ += codegen-x64.o
endif
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c
	@mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

calc: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(ODIR)/*.o
