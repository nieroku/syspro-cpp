project(
  'nsst8',
  'cpp',
  default_options: ['cpp_std=c++17'],
)

lib = static_library('lib', sources: ['file_io.h', 'string_io.h'])
lib_dep = declare_dependency(link_with: lib)

gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')

tests = executable(
  'tests',
  sources: ['io_tests.cc'],
  dependencies: [
    lib_dep,
    gtest_dep,
    gtest_main_dep,
  ],
)
test('tests', tests)
