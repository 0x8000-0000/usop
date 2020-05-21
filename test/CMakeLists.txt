
foreach (test_file segment_test pool_test polymorphic_test conversion_test)

   add_executable (${test_file} ${test_file}.cpp geometry.cpp)

   target_link_libraries (${test_file} PUBLIC CONAN_PKG::gtest)
   target_link_libraries (${test_file} PUBLIC project_options)
   target_link_libraries (${test_file} PUBLIC project_warnings)
   target_link_libraries (${test_file} PUBLIC sbit)

   gtest_discover_tests (${test_file})

endforeach (test_file)   

add_executable (unique_poly unique_poly.cpp)
