project('opengl-app', 'c', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++17', 'warning_level=3']
)

# Dependencies
glfw_dep = dependency('glfw3')
gl_dep = dependency('gl')
dl_dep = dependency('dl')
x11_dep = dependency('x11')
thread_dep = dependency('threads')

# GLAD as a separate library with reduced warnings
glad_lib = static_library('glad',
  'external/glad/src/glad.c',
  include_directories: 'external/glad/include',
  c_args: ['-Wno-pedantic']
)

inc_dirs = include_directories([
  'include',
  'external'
])

# Find cpp files
sources = []
cpp_files_output = run_command('sh', '-c', 'find src -name "*.cpp" -type f 2>/dev/null || true', check: true).stdout().strip()
if cpp_files_output != ''
  foreach file : cpp_files_output.split('\n')
    if file != ''
      sources += files(file)
    endif
  endforeach
endif

# Executable
executable('app',
  sources,
  include_directories: ['include', 'external/glad/include'],
  link_with: glad_lib,
  dependencies: [glfw_dep, gl_dep, dl_dep, x11_dep, thread_dep]
)
