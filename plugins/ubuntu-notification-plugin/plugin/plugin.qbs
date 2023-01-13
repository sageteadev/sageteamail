import qbs

DynamicLibrary {
    name: "Notify Service Plugin"
    targetName: "sageteamail-notify-plugin"
    builtByDefault: project.click
    Depends { name: "cpp" }
    Depends { name: "Qt.core" }
    Depends { name: "PlugMan" }
    Depends { name: "SnapStandardPaths" }
    cpp.cxxLanguageVersion: "c++11";
    cpp.cxxStandardLibrary: "libstdc++";

    cpp.defines: [
        "SAGETEAMAIL_VERSION=\"" + project.version +"\""
    ]

    Group {
        name: "plugin"
        files: [
            "notifyserviceplugin.cpp",
            "notifyserviceplugin.h",
        ]
    }

    Group {
        qbs.install: project.click
        qbs.installDir: project.pluginDir
        fileTagsFilter: product.type
    }
}
