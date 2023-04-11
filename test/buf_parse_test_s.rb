
def one(out, su, bits, base_prefix, negative, digits, result)
  r = if result != 0 then "#{negative}#{result}" else result end
  output = Proc.new do |digits|
    zeros = 0
    spaces = ""
    digits.each do |digit|
      if digit != 0 || zeros >= digits.count - 1
        break
      end
      zeros += 1
    end
    i = zeros
    while i >= 0
      test_digits = digits[i..(digits.count - 1)]
      test_digits = test_digits.join
      spaces = " " * (i + ((result.zero? && negative == "-") ? 1 : 0))
      out.puts "  BUF_PARSE_TEST_#{su}(#{bits}, \"#{negative}#{base_prefix}#{test_digits}\", #{spaces}#{r});"
      i -= 1
    end
  end
  output.(digits)
  string_digits = false
  digits_upcase = digits.map do |digit|
    if digit.is_a?(String)
      string_digits = true
      digit.upcase
    else
      digit
    end
  end
  output.(digits_upcase) if string_digits
end

def count(out, su, bits, base, base_prefix, negative, max)
  result = 0
  base.each do |i|
    i = 0 if i == "0"
    base.each do |j|
      j = 0 if j == "0"
      base.each do |k|
        one(out, su, bits, base_prefix, negative, [i, j, k], result)
        result += 1
        if result > max
          return result
        end
      end
    end
  end
  return result
end

def number_to_base(m, base)
  digits = []
  radix = base.count
  while m > 0
    digits.push(base[m % radix])
    m = m / radix
  end
  digits.reverse
end

def integer_max(su, bits, negative)
  if negative == "-"
    2 ** (if su == "S" then bits - 1; else bits; end)
  else
    2 ** (if su == "S" then bits - 1; else bits; end) - 1
  end
end

BITS = [8, 16, 32, 64]

File.open("buf_parse_test_s.out", "w") do |out|
  ["S", "U"].each do |su|
    BITS.each do |bits|
      out.puts ""
      out.puts "void buf_parse_test_#{su.downcase}#{bits} ()"
      out.puts "{"
      [[[0, 1], "0b"],
       [[0, 1, 2, 3, 4, 5, 6, 7], "0o"],
       [[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, "a", "b", "c", "d", "e", "f"],
        "0x"],
       [[0, 1, 2, 3, 4, 5, 6, 7, 8, 9], ""]].each do |b|
        base = b[0]
        base_prefix = b[1]
        radix = base.count
        signs = if (su == "S")
                  ["-", ""]
                else
                  [""]
                end
        signs.each do |negative|
          max = integer_max(su, bits, negative)
          result = count(out, su, bits, base, base_prefix, negative,
                         max)
          BITS.each do |b|
            if b <= bits
              max = integer_max(su, b, negative)
              if result < max - 1
                digits = number_to_base(max - 1, base)
                one(out, su, bits, base_prefix, negative, digits, max - 1)
              end
              if result < max
                digits = number_to_base(max, base)
                one(out, su, bits, base_prefix, negative, digits, max)
              end
            end
          end
        end
      end
      out.puts "}"
    end
  end
end
