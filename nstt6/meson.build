project(
  'nstt6',
  'cpp',
  default_options: ['cpp_std=c++17'],
)

lib = static_library('nstt6', sources: 'matrix.cc')
lib_dep = declare_dependency(link_with: lib)

gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')
tests_exe = executable(
  'tests',
  ['matrix_test.cc'],
  dependencies: [lib_dep, gtest_dep, gtest_main_dep],
)

test('tests', tests_exe)
