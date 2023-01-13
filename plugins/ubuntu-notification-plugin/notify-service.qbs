import qbs

Project {
    name: "Ubuntu Notification Service"

    property string installDir: pluginDir + "/notify/"

    references: [
        "plugin/plugin.qbs",
        "service/service.qbs"
    ]

    Product {
        type: "settingsplugin"
        name: "Notify Service Settings"

        Group {
            name: "QML Sources"
            files: [
                "qml/*.qml",
                "qml/*.sageteamail-plugin"
            ]
            qbs.install: project.click
            qbs.installDir: project.installDir
            qbs.installSourceBase: "qml"
        }
    }

    DynamicLibrary {
        name: "Notify Service I18n"
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
            qbs.install: project.click
            qbs.installDir: project.installDir + "/i18n"
            fileTagsFilter: ["qm"]
        }
    }
}
