import qbs

QtGuiApplication {
    name: "Notification Watcher"
    targetName: "sageteamaild-notify"

    Depends { name: "cpp" }
    Depends {
        name: "Qt"
        submodules: [
            "core",
            "gui",
            "widgets",
            "dbus"
        ]
    }

    Depends { name: "QmfClient" }
    Depends { name: "QmfServer" }
    Depends { name: "AccountsLib" }

    cpp.optimization: qbs.buildVariant === "debug" ? "none" : "fast"
    cpp.debugInformation: qbs.buildVariant === "debug"
    cpp.cxxLanguageVersion: "c++11";
    cpp.cxxStandardLibrary: "libstdc++";
    cpp.includePaths: [ path ]
    cpp.defines: []

    Group {
        name: "C++ Sources"
        prefix: path + "/"
        files: [
            "*.cpp"
        ]
    }

    Group {
        name: "C++ Headers"
        prefix: path + "/"
        files: [
            "*.h"
        ]
    }

    Group {
        qbs.install: project.click
        qbs.installDir: project.installDir
        fileTagsFilter: product.type
    }
}
