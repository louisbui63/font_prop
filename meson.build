project('font_prop', 'cpp', default_options: ['cpp_std=c++23'])

srcs = [
  'dllMain.cpp',
  'font.cpp',
  'patcher.cpp',
  'prop.cpp',
  'proxy.cpp'
]

deps = []

incdir = include_directories('include')

def_file = join_paths(meson.project_source_root(), 'd2d1.def')


executable('d2d1.dll',
  srcs,
  dependencies: deps,
  link_args: ['-static', def_file], # not ideal because of ordering requirements but does work
  install: false,
  include_directories : incdir,
)
