project(
    'neosent', 'c', 
    default_options: ['c_std=c11', 'buildtype=release', 'b_ndebug=true'], 
    version: '1.2'
)

cc = meson.get_compiler('c')


deps = [
    dependency('sdl2'),
    dependency('SDL2_image'),
    dependency('SDL2_ttf'),
    cc.find_library('m'),
]
cwalk_include = include_directories('vendor/cwalk/include')

executable('nsent', 
    ['src/main.c', 'src/parser.c',
    'src/render.c', 'src/utils.c',
    'src/vector.c', 'src/hashmap.c', 
    'vendor/cwalk/src/cwalk.c'],
    dependencies: deps,
    include_directories: cwalk_include
)


