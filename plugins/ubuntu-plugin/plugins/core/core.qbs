import qbs

Project {
    name: "Core"

    DynamicLibrary {
        name: "WebContext"
        targetName: "sageteamailwebcontextplugin"

        Depends { name: "cpp" }
        Depends {
            name: "Qt"
            submodules: [
                "qml",
                "webengine"
            ]
        }

        cpp.optimization: qbs.buildVariant === "debug" ? "none" : "fast"
        cpp.debugInformation: qbs.buildVariant === "debug"
        cpp.cxxLanguageVersion: "c++11";
        cpp.cxxStandardLibrary: "libstdc++";
        cpp.includePaths: [ path ]

        Group {
            name: "QML directory"
            files: "mail/webview/qmldir"
            fileTags: ["sageteamail-webcontext-plugin"]
        }

        Group {
            name: "C++ Headers"
            files: [
                "mail/**/*.h"
            ]
        }

        Group {
            name: "C++ Sources"
            files: [
                "mail/**/*.cpp"
            ]
        }

        Group {
            condition: project.ui === "ubuntu"
            fileTagsFilter: "sageteamail-webcontext-plugin"
            qbs.install: project.ui === "ubuntu"
            qbs.installDir: project.qmlDir + "/SageteaMail/" + product.name
        }

        Group {
            condition: project.ui === "ubuntu"
            fileTagsFilter: product.type
            qbs.install: project.ui === "ubuntu"
            qbs.installDir: project.qmlDir + "/SageteaMail/" + product.name
        }
    }

    Product {
        name: "Mail"
        condition: project.enabled

        Depends { name: "cpp" }
        Depends { name: "Qt.core" }

        Group {
            name: "QML components"
            files: [
                "mail/*.qml",
                "mail/**/*.qml"
            ]
            fileTags: ["mail-ui-plugin"]
        }

        Group {
            name: "JavaScript modules"
            files: [
                "mail/**/*.js"
            ]
            fileTags: ["mail-ui-plugin"]
        }

        Group {
            name: "Plugin specs"
            files: [
                "mail/**/*.sageteamail-plugin"
            ]
            fileTags: ["mail-ui-plugin"]
        }

        Group {
            condition: project.ui === "ubuntu"
            fileTagsFilter: "mail-ui-plugin"
            qbs.install: project.ui === "ubuntu"
            qbs.installDir: project.binDir + "/plugins/ubuntu/mail"
            qbs.installSourceBase: "mail"
        }

    }

    Product {
        name: "Settings"
        condition: project.enabled

        Depends { name: "cpp" }
        Depends { name: "Qt.core" }

        Group {
            name: "QML components"
            files: [
                "settings/*.qml",
                "settings/**/*.qml"
            ]
            fileTags: ["settings-ui-plugin"]
        }

        Group {
            name: "JavaScript modules"
            files: [
                "settings/**/*.js"
            ]
            fileTags: ["settings-ui-plugin"]
        }

        Group {
            name: "Plugin specs"
            files: [
                "settings/**/*.sageteamail-plugin"
            ]
            fileTags: ["settings-ui-plugin"]
        }

        Group {
            condition: project.ui === "ubuntu"
            fileTagsFilter: "settings-ui-plugin"
            qbs.install: project.ui === "ubuntu"
            qbs.installDir: project.binDir + "/plugins/ubuntu/settings"
            qbs.installSourceBase: "settings"
        }
    }

//    SubProject {
//        filePath: "mail/mail.qbs"
//        inheritProperties: true
//    }
}
