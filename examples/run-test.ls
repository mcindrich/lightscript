def func() do
  a = 1;
  while a < 40 do
    if a % 2 == 0 do
      CPrint(" ", "*" * a);
    else
      CPrint("*" * a);
    end
    a = a + 1;
  end
  ret "*" * a;
end

str = func();

CPrint("str: ", str);