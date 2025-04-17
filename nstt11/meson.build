project(
  'nstt11',
  'cpp',
  default_options: ['cpp_std=c++17'],
)

gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')

tests = executable(
  'tests',
  sources: ['prime_test.cc'],
  dependencies: [gtest_dep, gtest_main_dep],
)
test('tests', tests)
