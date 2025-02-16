project(
  'nstt1',
  'cpp',
  default_options: ['cpp_std=c++17'],
)

avl_tree = static_library('avl_tree', sources: 'avl_tree.cc')
avl_tree_dep = declare_dependency(link_with: avl_tree)

gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')
tests_exe = executable(
  'tests',
  ['avl_tree_test.cc'],
  dependencies: [avl_tree_dep, gtest_dep, gtest_main_dep],
)

test('tests', tests_exe)
