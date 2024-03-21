project('cpp-jinja-enums', 'cpp',
  default_options: {
    'cpp_std': 'gnu++20',
    'warning_level': '2',
  },
)

add_project_arguments('-Wno-attributes', language: 'cpp')

enum_spec = declare_dependency(include_directories: ['src'])

from_string_script = files('src/from_string.py')
from_string_template = files('src/from_string.hh.jinja')

subdir('test')
