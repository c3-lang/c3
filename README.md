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
    |> C3.compile()
  end

  def eval (c3, env, opts) when is_string(c3) do
    c3
    |> C3.parse(opts)
    |> C3.eval(env)
  end

  defguard is_c3 (x) do
    is_tuple(x)
  end

  def compile (c3, opts) when is_c3(c3) do
    c3
    |> C3.to_assembly(opts)
    |> Assembly.to_elf(opts)
  end

  def eval (c3, env) when is_c3(c3) do
    [...]
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
  - buf_sha256
  - buf_popen

# File format

On disk format :

         0               1               2               3               4
         0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF01
 0x0000  %{dump: 0x0000000000000100, log: 0x0000000000001000, format: :c3}
 0x0042  _comments_
_0x0100_ _count_ _dump_
 0x0900  _sha256_
_0x1000_ _log_
