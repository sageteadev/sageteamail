import QtQuick 2.4
import io.thp.pyotherside 1.5

Python {
    id: python

    signal ready()

    property var importPaths: []
// please unsure to import the libs
// this will for modules to be imported and fix crashing when when try to create or reply msg
    Component.onCompleted: {
        addImportPath(Qt.resolvedUrl("."))
        importModule('metadata')
        importModule('markdown')
        importModule('cssutils')
        importModule('jinja2')
        importModule('markupsafe')
        importModule('pynliner')
        importModule('pygments')
        importModule('html2text')
        importModule('importlib-metadata')
        importModule('zipp')
        importModule('soupsieve')
        ready()
    }
}
