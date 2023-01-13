import qbs

DynamicLibrary {
    name: "Cache Manager Plugin"
    targetName: "sageteamail-cachemgr-plugin"
    Depends { name: "cpp" }
    Depends { name: "Qt.core" }
    Depends { name: "Qt.dbus" }
    Depends { name: "PlugMan" }
    Depends { name: "SnapStandardPaths" }
    Depends { name: "QmfClient" }
    Depends { name: "QmfServer" }
    Depends { name: "Mail Lib" }

    cpp.cxxLanguageVersion: "c++11";
    cpp.cxxStandardLibrary: "libstdc++";

    cpp.defines: [
        "SAGETEAMAIL_VERSION=\"" + project.version +"\""
    ]

    Group {
        name: "plugin"
        files: [
            "*.h",
            "*.cpp"
        ]
    }

    Group {
        qbs.install: true
        qbs.installDir: project.pluginDir
        fileTagsFilter: product.type
    }
}
