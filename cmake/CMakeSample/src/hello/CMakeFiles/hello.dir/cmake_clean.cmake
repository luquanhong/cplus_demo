FILE(REMOVE_RECURSE
  "CMakeFiles/hello.dir/hello.cpp.o"
  "libhello.pdb"
  "libhello.a"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang CXX)
  INCLUDE(CMakeFiles/hello.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
