project('font_prop','cpp', default_options: ['cpp_std=c++23'])

srcs = [
  'dllMain.cpp',
  'font.cpp',
  'patcher.cpp',
  'prop.cpp',
  'proxy.cpp'
]

# spdlog_dep = dependency('spdlog', required : true)
#
# if not spdlog_dep.found()
#   spdlog_proj = subproject('spdlog')
#   spdlog_dep = spdlog_proj.get_variable('spdlog')
# endif
#
# deps = [spdlog_dep]
deps = []

incdir = include_directories('include')

def_file = join_paths(meson.project_source_root(), 'd2d1.def')

linka = []
if meson.has_external_property('is_clang_mingw')
  linka = ['-static', def_file]
elif meson.has_external_property('is_clang_msvc')
  linka = ['-Wl,/DEF:'+def_file]
endif



executable('d2d1.dll',
  srcs,
  dependencies: deps,
  link_args: linka, # not ideal because of ordering requirements but does work
  install: false,
  include_directories : incdir,
)
