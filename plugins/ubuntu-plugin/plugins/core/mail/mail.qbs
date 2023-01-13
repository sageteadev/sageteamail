import qbs

Project {
    name: "Mail"

    Product {
        name: "Mail UI"
        condition: project.enabled

        Depends { name: "cpp" }
        Depends { name: "Qt.core" }

        cpp.optimization: qbs.buildVariant === "debug" ? "none" : "fast"
        cpp.debugInformation: qbs.buildVariant === "debug"
        cpp.cxxLanguageVersion: "c++11";
        cpp.cxxStandardLibrary: "libstdc++";
        cpp.includePaths: [ path ]

        Group {
            name: "QML components"
            files: [
                "*.qml",
                "**/*.qml"
            ]
            fileTags: ["mail-ui-plugin"]
        }

        Group {
            name: "JavaScript modules"
            files: [
                "**/*.js"
            ]
            fileTags: ["mail-ui-plugin"]
        }

        Group {
            name: "Plugin specs"
            files: [
                "**/*.sageteamail-plugin"
            ]
            fileTags: ["mail-ui-plugin"]
        }

        Group {
            condition: project.ui === "ubuntu"
            fileTagsFilter: "mail-ui-plugin"
            qbs.install: project.ui === "ubuntu"
            qbs.installDir: project.binDir + "/plugins/ui/mail"
        }

        Group {
            name: "C++ Headers"
            files: "**/*.h"
        }

        Group {
            name: "C++ Sources"
            files: "**/*.cpp"
        }
    }
}
