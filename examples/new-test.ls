import stdlib;

struct File
  filename = "";
  mode = "";

  def File(self, fn, m) do
    filename = fn + "";
    mode = m + "";
  end

end

a = New(File, "config.conf", "r");

Print(a:filename);