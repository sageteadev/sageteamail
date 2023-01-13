import qbs

Product {
    name: "Click"
    type: "clickpackage"
    builtByDefault: project.click

    Group {
        name: "AppArmor"
        files: [
            "openstore/sageteamail2.apparmor",
            "openstore/sageteamaild.apparmor"
        ]
        fileTags: ["clickroot"]
    }

    Group {
        name: "Click Manifests"
        files: [
            "sageteamail2-content.json",
            "sageteamail2.url-dispatcher",
            "manifest.json",
        ]
        fileTags: ["clickroot"]
    }

    Group {
        name: "Launcher"
        files: [
            "sageteamaild-launch"
        ]
        fileTags: ["clickbin"]
    }

    Group {
        name: "Desktop Files"
        files: [
            "sageteamail2.desktop",
            "sageteamaild.desktop",
        ]
        fileTags: ["clickroot"]
    }

    Group {
        name: "Push Helper"
        files: [
            "sageteamailhelper-aa.json",
            "sageteamail-helper.json",
            "sageteamail-helper"
        ]
        fileTags: ["clickroot"]
    }

    Group {
        condition: project.click
        fileTagsFilter: "clickroot"
        qbs.install: product.type
        qbs.installDir: "/"
    }

    Group {
        condition: project.click
        fileTagsFilter: "clickbin"
        qbs.install: product.type
        qbs.installDir: project.binDir
    }
}
