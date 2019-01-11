import stdlib;

struct File
  filename = "";
  mode = "";
end

a = New(File);

a:filename = "/home/user/";
b = a:filename;