import stdlib;
import string;

def func(arr) do
  Print(arr[4]);
end

a = 0;
arr = [];

while a < 20
do
  arr[a] = a * "*";
  a = a + 1;
end

str = "Hello";
a = StringLength(str);

Print(a);