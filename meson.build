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

# Definitions
# add_project_arguments('-DSIO_TLS', language: 'cpp')
# add_project_arguments('-DSIO_ECHO', language: 'cpp')
# add_project_arguments('-DASIO_STANDALONE', language: 'cpp')
# # asio uses OpenSSL functions deprecated in OpenSSL 3.0 (suppress warnings)
# add_project_arguments('-Wno-deprecated-declarations', language: 'cpp')
# add_project_arguments('-DDEFAULT_SERVER="' + server + '"', language: 'cpp')
# add_project_arguments('-DDEFAULT_UUID="' + uuid + '"', language: 'cpp')

grpc_cc = custom_target(
    'grpc_cc',
    input: proto_file,
    output: [
        'suprema-gw.pb.h',
        'suprema-gw.pb.cc',
        'suprema-gw.grpc.pb.h',
        'suprema-gw.grpc.pb.cc'
    ],
    command: [
        protoc,
        '--grpc_out', build_dir,
        '--cpp_out', build_dir,
        '-I', proto_path,
        '--plugin=protoc-gen-grpc=' + proto_cpp_plugin.full_path(),
        proto_file
    ]
)

# Includes
inc = include_directories([
    'include',
    'include/biostar2sdk',
    'include/biostar2sdk/BSCommon',
    grpc_root / 'include'
])

# Sources
src = [
    'src/common.cpp',
    'src/server.cpp',
    'src/grpc.cpp',
    'src/app.cpp',
    grpc_cc
]

# Dependencies
deps = [
    dependency('libssl'),
    dependency('libcrypto'),
    dependency('threads'),
    dependency('protobuf'),
    dependency('grpc'),
    dependency('grpc++'),
    dependency('re2'),
    dependency('absl_flags'),
    dependency('absl_flags_parse'),
    dependency('absl_absl_check'),
    dependency('absl_absl_log'),
    dependency('absl_algorithm'),
    dependency('absl_base'),
    dependency('absl_bind_front'),
    dependency('absl_bits'),
    dependency('absl_btree'),
    dependency('absl_cleanup'),
    dependency('absl_cord'),
    dependency('absl_core_headers'),
    dependency('absl_debugging'),
    dependency('absl_die_if_null'),
    dependency('absl_dynamic_annotations'),
    dependency('absl_flat_hash_map'),
    dependency('absl_flat_hash_set'),
    dependency('absl_function_ref'),
    dependency('absl_hash'),
    dependency('absl_layout'),
    dependency('absl_log_initialize'),
    dependency('absl_log_severity'),
    dependency('absl_memory'),
    dependency('absl_node_hash_map'),
    dependency('absl_node_hash_set'),
    dependency('absl_optional'),
    dependency('absl_span'),
    dependency('absl_status'),
    dependency('absl_statusor'),
    dependency('absl_strings'),
    dependency('absl_synchronization'),
    dependency('absl_time'),
    dependency('absl_type_traits'),
    dependency('absl_utility'),
    dependency('absl_variant'),
    dependency('grpc++_reflaction'),
]



# Create executable
executable(
    meson.project_name(),
    sources : src,
    include_directories : inc,
    dependencies : deps,
    link_args : [
        '-Wl,libBS_SDK_V2.so',
        '-Wl,--no-as-needed'
    ]
)
