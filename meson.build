project(
    'bio2server', 'cpp',
    version : '0.1.0',
    meson_version : '>= 0.55.0',
    default_options: [
        'cpp_std=c++17'
    ])

grpc_root = get_option('grpc_root')
message('grpc_root=', grpc_root)
# uuid = get_option('uuid')
# message('uuid=', uuid)

grpc_bin = grpc_root / 'bin'
protoc = grpc_bin / 'protoc'
cpp_plugin_def = grpc_bin / 'grpc_cpp_plugin'
build_dir = meson.current_build_dir()
src_dir = meson.current_source_dir()
proto_path = src_dir / 'files' / 'proto'
proto_file = proto_path / 'suprema-gw.proto'
proto_cpp_plugin = find_program ('grpc_cpp_plugin', cpp_plugin_def)


# Header file config
version_arr = meson.project_version().split('.')
conf_data = configuration_data()
conf_data.set('version_major', version_arr[0])
conf_data.set('version_minor', version_arr[1])
conf_data.set('version_rev',   version_arr[2])
configure_file(
    input         : 'src/app.h.in',
    output        : 'app.h',
    configuration : conf_data
)

configure_file(
    input  : 'lib/biostar2sdk/x64/libBS_SDK_V2.so',
    output : 'libBS_SDK_V2.so',
    command : ['cp', '@INPUT@', '@OUTPUT@'],
)

# Includes
inc = include_directories([
    'include',
    'include/biostar2sdk',
    'include/biostar2sdk/BSCommon',
])

# Sources
src = [
    'src/common.cpp',
    'src/server.cpp',
    'src/app.cpp',
]

# Dependencies
deps = [
    dependency('libssl'),
    dependency('libcrypto'),
    dependency('threads'),
]



# Create executable
executable(
    meson.project_name(),
    sources : src,
    include_directories : inc,
    dependencies : deps,
    link_args : [
        '-Wl,libBS_SDK_V2.so',
    ]
)
