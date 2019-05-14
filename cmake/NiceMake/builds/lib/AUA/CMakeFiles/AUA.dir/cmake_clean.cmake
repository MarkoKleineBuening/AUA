file(REMOVE_RECURSE
  "../libAUA.pdb"
  "../libAUA.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/AUA.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
