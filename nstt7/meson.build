project(
  'nstt7',
  'cpp',
  default_options: ['cpp_std=c++23'],
)

lib = static_library('nstt7', sources: ['lang.cc', 'lexer.cc', 'parser.cc'])
lib_dep = declare_dependency(link_with: lib)

gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')

test_exe = executable(
  'tests',
  sources: ['lang_tests.cc', 'lexer_tests.cc', 'parser_tests.cc'],
  dependencies: [
    lib_dep,
    gtest_dep,
    gtest_main_dep,
  ],
)
test('tests', test_exe)

executable(
  'repl',
  sources: ['repl.cc'],
  dependencies: [lib_dep],
)
