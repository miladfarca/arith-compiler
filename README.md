# Arithmetic Compiler
Implementing an `Arithmetic Interpreter and Just in time (JIT) Compiler`.

In this project we will be writing an Interpreter/Compiler in C for doing basic
arithmetic computation.

We will follow the methods explained in `the dragon book (Compilers: Principles, Techniques, and Tools)`. 
We will start by writing a lexical analyzer and a `Recursive Decent`, top down parser and create a `Abstract Syntax Tree`.
Then we will traverse the binary tree using `Depth-first search, Post-order traversal`, visit each node and either interpret it or generate code. Code generation must include a registrar allocation phase as we only have a limited number of registers to work with on any architecture.

## Supported architectures
```
x64
arm64
```

## Build
Checkout the project and simply run `make` to compile the src files on an `x64` host.
You can build on other supported architectures by passing the name as a command line argument.
```
git checkout https://github.com/miladfarca/arith-compiler.git
cd arith-compiler
make
# or 
make <arch=1> # i.e `make arm64=1` 
```

## Run
Once compiled you will have an executable called `./calc`, to see the available flags simply run `./calc --help`:
```
$ ./calc --help

flags:
--print-tokens       Print the terminal symbols (Tokens) of the input
--print-ast          Print the generated Abstract Syntax Tree (AST)
--print-ast-json     Print the generated Abstract Syntax Tree (AST) in JSON format
--print-reg-alloc    Print allocation and de-allocation of registers
--print-code         Print the generated instructions
--print-all          Print all the above
--no-output          Do not print the final output
--jitless            Interpret the result and do not generated code
--inline             Enter the input as an argument
--test               Run unit tests
```
You can do basic arithmetic using calc:
```
$./calc 
5+4
= 9
```

### Run unit tests
```
$./calc --test
```

## Syntax Tree format
Using the `--print-ast` flag you will be able to see the generated binary tree from your input. As an example `5+4` will produce the following:
```
--- Syntax Tree ---

               1
          *
               4
     +
          0
+
          1
     *
          5
```

Notice that the root node is at the very left of the screen `+`, the above is equivalent to this tree:
```
           +
       /       \  
      *         +
    /   \     /   \
   5     1   0     * 
                 /   \
                4     1
```        
