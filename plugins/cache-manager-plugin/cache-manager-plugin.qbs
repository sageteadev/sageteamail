import qbs

Project {
    name: "Cache Manager"

    property string installDir: pluginDir + "/cache-manager"

    references: [
        "plugin/plugin.qbs"
    ]

    Product {
        type: "cachemanagerplugin"
        name: "Cache Manager Settings"

        Group {
            name: "QML Sources"
            files: [
                "qml/*.qml",
                "qml/*.sageteamail-plugin"
            ]
            qbs.install: true
            qbs.installDir: project.installDir
            qbs.installSourceBase: "qml"
        }
    }

    DynamicLibrary {
        name: "Cache Manager I18n"
        type: "dynamiclibrary"

        Depends { name: "cpp" }
        Depends { name: "Qt.core" }

        Group {
            name: "ts files"
            prefix: "i18n/"
            files: [
                "*.ts"
            ]
        }

        Group {
            qbs.install: true
            qbs.installDir: project.installDir + "/i18n"
            fileTagsFilter: ["qm"]
        }
    }
}
