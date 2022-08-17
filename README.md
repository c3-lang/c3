# C3

```
defmodule C3 do
  def compile (c3, opts) when is_string(c3) do
    |> C3.parse(c3, opts)
    |> C3.to_c89(opts)
    |> C89.compile(opts)
  end
end
```
