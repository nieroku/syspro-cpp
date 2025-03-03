project(
  'nstt5',
  'cpp',
  default_options: ['cpp_std=c++17'],
)

gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')
tests_exe = executable(
  'tests',
  ['resourse_guard_test.cc', 'scoped_pointer_test.cc'],
  dependencies: [gtest_dep, gtest_main_dep],
)

test('tests', tests_exe)
