import qbs

Project {

    name: "App"

    QtGuiApplication {
        name: "Sagetea Mail"
        targetName: "sageteamail"

        Depends { name : "cpp" }
        Depends {
            name: "Qt"
            submodules: [
                "core",
                "quick",
                "qml",
                "gui",
                "quickcontrols2",
                "widgets"
            ]
        }
        Depends { name: "QmfClient" }
        Depends { name: "QmfServer" }
        Depends { name: "PlugMan" }
        Depends { name: "Network Lib" }
        Depends { name: "SnapStandardPaths" }

        cpp.optimization: qbs.buildVariant === "debug" ? "none" : "fast"
        cpp.debugInformation: qbs.buildVariant === "debug"
        cpp.cxxLanguageVersion: "c++11";
        cpp.cxxStandardLibrary: "libstdc++";
        cpp.includePaths: [ path ]

        cpp.defines: [
            "SNAP_MODE",
            "APP_NAME=\"sageteamail\"",
            "APP_ORG=\"sagetea\"",
            "SAGETEAMAIL_VERSION=\"" + project.version + "-" + project.revision + "\""
        ]

        Properties {
            condition: project.click
            cpp.defines: [
                "CLICK_MODE",
                "APP_NAME=\"sageteamail2.sagetea\"",
                "APP_ORG=\"sageteamail2.sagetea\"",
                "SAGETEAMAIL_VERSION=\"" + project.version + "-" + project.revision + "\""
            ]
        }

        Group {
            name: "C++ Sources"
            files: [
                "*.cpp"
            ]
        }

        Group {
            name: "C++ Headers"
            files: [
                "*.h"
            ]
        }

        Group {
            name: "Asset Configs"
            files: [
                "assets/configuration/**"
            ]
            qbs.install: true
            qbs.installDir: "/usr/share/sagetea/configuration"
            qbs.installSourceBase: "assets/configuration"
        }

        Group {
            name: "Asset Resources"
            files: ["assets/**/*.qrc"]
        }

        Group {
            name: "QML Resources"
            files: ["sageteamail.qrc"]
        }

        Group {
            name: "App Icon"
            files: [
                "assets/icons/dekko/sageteamail.png"
            ]
            qbs.install: project.click
            qbs.installDir: "/"
        }

        Group {
            qbs.install: true
            qbs.installDir: project.binDir
            fileTagsFilter: product.type
        }
    }
}
