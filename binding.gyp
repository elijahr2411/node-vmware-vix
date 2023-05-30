{
    "targets": [
        {
            "target_name": "vmware-vix",
            "sources": [
                "src/vmware.cpp"
            ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")"
            ],
            'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
        }
    ]
} 