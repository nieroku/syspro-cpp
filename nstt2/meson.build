project(
  'nstt2',
  'cpp',
  default_options: ['cpp_std=c++17'],
)

avl_tree = static_library('avl_tree', sources: 'angem.cc')
avl_tree_dep = declare_dependency(link_with: avl_tree)

gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')
tests_exe = executable(
  'tests',
  ['angem_test.cc'],
  dependencies: [avl_tree_dep, gtest_dep, gtest_main_dep],
)

test('tests', tests_exe)
