VPATH=${SRC_PATH}:./src:./src/x64/
CC=gcc
CFLAGS=-Wall
ODIR=obj
_OBJ = main.o flags.o parser.o ast.o codegen.o codegen-x64.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c
	@mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

calc: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(ODIR)/*.o
