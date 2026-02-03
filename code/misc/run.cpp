# Open & set C++ build system in Sublime (Linux)

1) Open Sublime Text
2) Menu → Tools → Build System → New Build System
3) Paste this code:

{
  "cmd": ["bash", "-c", "g++ \"$file\" -std=gnu++17 -O2 -o a.out && ./a.out < input.txt"],
  "selector": "source.c, source.c++",
  "working_dir": "$file_path"
}

4) Ctrl + S → save as: cpp.sublime-build
5) Menu → Tools → Build System → cpp
6) Open main.cpp → Ctrl + B
# input from input.txt, output in terminal
