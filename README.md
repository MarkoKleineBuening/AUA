1) clone llvm: https://github.com/llvm-mirror/llvm version 4.0 , then cd into subfolder tools and clone clang version 4.0 from https://github.com/llvm-mirror/clang
2) install llvm: https://llvm.org/docs/CMake.html
3) clone AUA, then go to subfolder cmake, niceMake and do git submodule init ; git submodule update
4) mkdir builds, cd builds, cmake -DCMAKE_PREFIX_PATH=/home/marko/Libraries/Installations/llvm_40/lib/cmake/llvm ~/Workspace/src/AUA/ (adjust path to your own paths and installations
5) create example file test.c and run ~/Libraries/Installations/llvm_40/bin/clang -c -emit-llvm test.c (again with your clang installation)
6) use the created .bc file as input for AUA


