{
    "targets": [
        {
            "target_name": "vmware-vix",
            "sources": [
                "src/vmware.cpp"
            ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",
            ],
            'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
            'conditions': [
                ['OS=="win"', {
                    'include_dirs': [
                        "C:\\Program Files (x86)\\VMware\\VMware VIX"
                    ],
                    'libraries': [
                        "C:\\Program Files (x86)\\VMware\\VMware VIX\\Vix64AllProducts.lib",
                        "kernel32.lib",
                        "user32.lib",
                        "advapi32.lib",
                        "ole32.lib",
                        "oleaut32.lib",
                        "ws2_32.lib",
                        "shell32.lib",
                    ],
                    'msvs_settings': {
                        'VCCLCompilerTool': { 'AdditionalOptions': [ '/MT' ] },
                        'VCLinkerTool': { 'AdditionalOptions': ['/nodefaultlib:libcmt.lib'] }
                    }
                }],
                ['OS=="linux"', {
                    # I have not tested this yet
                    "cflags": ["-I/usr/include/vmware-vix", "-lvixAllProducts", "-ldl", "-lpthread"]
                }]
            ]
        }
    ]
} 