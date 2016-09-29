FILE(REMOVE_RECURSE
  "CMakeFiles/hello.dir/main.cpp.o"
  "CMakeFiles/hello.dir/test.c.o"
  "hello.pdb"
  "hello"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang C CXX)
  INCLUDE(CMakeFiles/hello.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
