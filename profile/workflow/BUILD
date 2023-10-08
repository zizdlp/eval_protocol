

cc_binary(
    name = 'sync_server',
    srcs = ['sync_server.cc'],
    deps = ["@workflow//:http",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings:str_format",
        ],
)
cc_binary(
    name = 'sync_client',
    srcs = ['sync_client.cc'],
    deps = ["@workflow//:http",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings:str_format",
        ],
)

cc_binary(
    name = 'async_server',
    srcs = ['async_server.cc'],
    deps = ["@workflow//:http",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings:str_format",
        ],
)
cc_binary(
    name = 'async_client',
    srcs = ['async_client.cc'],
    deps = ["@workflow//:http",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings:str_format",
        ],
)