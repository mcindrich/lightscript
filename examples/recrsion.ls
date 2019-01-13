import stdlib;

def fib(n) do
  if n == 0 || n == 1 do
    ret Copy(n);
  else
    ret fib(n - 1) + fib(n - 2);
  end
end

a = fib(11);

Print(a);