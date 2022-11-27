# C3

C3 is a programming language project, inspired by C, Elixir and
Common Lisp. It could be described as C with Elixir macros, pattern
matching, and a semantic object system. The idea is to plug macros and
pattern matching into C89 and use them to implement the graph database.

The garbage collector is optional, like in C.

```
defmodule C3 do
  def compile (c3, opts) when is_string(c3) do
    c3
    |> C3.parse(opts)
    |> C3.to_assembly(opts)
    |> Assembly.compile(opts)
  end
end
```

## Structure

### libc3

C3 is implemented using libc3 a small C89 library implementing the core
of the language.

The library includes a parser and an interpreter for C3 code in C structures.

#### Parser

The parser is recursive descent for now.

#### AST

The AST is represented as C3 data structures and can be meta-programmed in C.

#### Interpreter

Under development.


### ic3

Interactive shell. Under development.


### c3s

Script interpreter. Under development.


## TODO

 - libc3
   - errors (setjmp, longjmp)
     - stacktrace
   - load
   - pattern matching (xor xor)
   - `test_file_compare`
   - ffi
     - types
     - funcall
   - functions
     - fn ->
     - & &1
     - defmodule
     - funcall
