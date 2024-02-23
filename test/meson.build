gtest = subproject('gtest')

prog_python = find_program('python3')

enum_from_string_to_string_h = custom_target(
  'enum_from_string_to_string.hh',
  output : 'enum_from_string_to_string.hh',
  input : [from_string_script, from_string_template, 'enum_from_string/enum_from_string.hh'],
  # FIXME: is it possible to pass flags from meson?
  command : [prog_python, '@INPUT0@', '@INPUT2@', '-std=gnu++20', '-o', '@OUTPUT@'],
)

test('all', executable('runTests', [
    'main.cc',
    'enum_from_string/enum_from_string.cc',
    enum_from_string_to_string_h,
  ],
  dependencies : [
    gtest.get_variable('gtest_dep'),
  ]
))
