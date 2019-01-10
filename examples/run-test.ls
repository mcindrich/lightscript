def func(arr) do
  CPrint(arr[4]);
end

a = 0;
arr = [];

while a < 20
do
  arr[a] = a * "*";
  a = a + 1;
end


CPrint(arr[10]);