import qbs

Project {
    id: project
    name: "Upstream"

    SubProject {
        filePath: "plugman/plugman.qbs"
        Properties {
            libDir: project.libDir
            qmlDir: project.qmlDir
        }
    }

    SubProject {
        filePath: "controls/controls.qbs"
        Properties {
            installDir: project.qmlDir
        }
    }

    SubProject {
        filePath: "ubuntu-plugin/ubuntu-ui.qbs"
        inheritProperties: true
        Properties {
            enabled: project.ui === "ubuntu"
        }
    }

    SubProject {
        filePath: "ubuntu-notification-plugin/notify-service.qbs"
        inheritProperties: true
    }

    SubProject {
        filePath: "cache-manager-plugin/cache-manager-plugin.qbs"
        inheritProperties: true
    }
}
