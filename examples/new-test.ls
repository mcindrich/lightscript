import stdlib;

struct File
  filename = "";
  mode = "";

  def File(self, fn, m) do
    filename = fn + "";
    mode = m + "";
  end

end

d = 200;

c = d;
c = 300;

Print(d);

a = New(File, "config.conf", "r");

Print(a:mode);