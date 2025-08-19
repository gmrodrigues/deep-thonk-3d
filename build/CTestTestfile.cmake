# CMake generated Testfile for 
# Source directory: /home/glauber/arquivos/projetos/gamedev/deep-thonk-3d
# Build directory: /home/glauber/arquivos/projetos/gamedev/deep-thonk-3d/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[deepThonk3d_tests]=] "/home/glauber/arquivos/projetos/gamedev/deep-thonk-3d/build/tests/deepThonk3d_tests")
set_tests_properties([=[deepThonk3d_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/glauber/arquivos/projetos/gamedev/deep-thonk-3d/CMakeLists.txt;21;add_test;/home/glauber/arquivos/projetos/gamedev/deep-thonk-3d/CMakeLists.txt;0;")
subdirs("src")
subdirs("tests")
