project('opengl-app', 'c', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++17', 'warning_level=3']
)

# Platform detection
host_system = host_machine.system()

# Dependencies
glfw_dep = dependency('glfw3', static: true)
thread_dep = dependency('threads')

# Platform-specific dependencies
if host_system == 'windows'
  gl_dep = dependency('opengl32', required: false)
  if not gl_dep.found()
    gl_dep = meson.get_compiler('cpp').find_library('opengl32')
  endif
  platform_deps = [gl_dep]
elif host_system == 'darwin'
  gl_dep = dependency('gl', required: false)
  if not gl_dep.found()
    gl_dep = dependency('appleframeworks', modules: ['OpenGL'])
  endif
  platform_deps = [gl_dep]
else
  # Linux and other Unix-like systems
  gl_dep = dependency('gl')
  dl_dep = dependency('dl')
  x11_dep = dependency('x11')
  platform_deps = [gl_dep, dl_dep, x11_dep]
endif

# GLAD as a separate library with reduced warnings
glad_c_args = ['-Wno-pedantic']
if host_system == 'windows' and meson.get_compiler('c').get_id() == 'msvc'
  glad_c_args = ['/wd4996']
endif

glad_lib = static_library('glad',
  'external/glad/src/glad.c',
  include_directories: 'external/glad/include',
  c_args: glad_c_args
)

inc_dirs = include_directories([
  'include',
  'external/glad/include',
  'external/stb_image',
])

# Find cpp files
sources = []
sources += files('external/stb_image/stb_image_impl.c')

# Cross-platform file discovery
if host_system == 'windows'
  # Windows: use PowerShell or fallback to manual listing
  cpp_files_output = run_command(
    'powershell', '-NoProfile', '-Command',
    'Get-ChildItem -Path src -Filter *.cpp -Recurse | Select-Object -ExpandProperty FullName',
    check: false
  ).stdout().strip()
  
  if cpp_files_output != ''
    foreach file : cpp_files_output.split('\n')
      if file != ''
        # Convert Windows paths to forward slashes for Meson
        file = file.replace('\\', '/')
        # Make path relative to project root
        if file.contains('src/')
          file = file.split('src/')[1]
          sources += files('src/' + file.strip())
        endif
      endif
    endforeach
  endif
else
  # Unix-like systems (Linux, macOS)
  cpp_files_output = run_command('sh', '-c', 'find src -name "*.cpp" -type f 2>/dev/null || true', check: true).stdout().strip()
  
  if cpp_files_output != ''
    foreach file : cpp_files_output.split('\n')
      if file != ''
        sources += files(file)
      endif
    endforeach
  endif
endif

# Executable
executable('app',
  sources,
  include_directories: inc_dirs,
  link_with: glad_lib,
  dependencies: [glfw_dep, thread_dep] + platform_deps
)

# Copy compile_commands.json (for IDE integration)
if host_system == 'windows'
  run_command('powershell', '-NoProfile', '-Command',
    'if (Test-Path build/compile_commands.json) { Copy-Item build/compile_commands.json compile_commands.json }',
    check: false
  )
else
  run_command('sh', '-c', 'cp build/compile_commands.json compile_commands.json 2>/dev/null || true', check: false)
endif
