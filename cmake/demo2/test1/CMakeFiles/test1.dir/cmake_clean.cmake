FILE(REMOVE_RECURSE
  "CMakeFiles/test1.dir/test1.cpp.o"
  "libtest1.pdb"
  "libtest1.a"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang CXX)
  INCLUDE(CMakeFiles/test1.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
