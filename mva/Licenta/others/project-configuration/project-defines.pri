CONFIG(debug, debug|release) {
    BUILD = debug
} else {
    BUILD = release
}

contains(QMAKE_TARGET.arch, x86_64) {
    OS_SPEC = x64
} else {
    OS_SPEC = x86
}

win32 {
    OS_NAME = win
}

unix:!macx {
    OS_NAME = linux
}

unix:macx {
    OS_NAME = mac
}
CROSSPLATFORM = $$OS_NAME/$$OS_SPEC/$$BUILD

# Copies the given files to the destination directory
defineTest(copyFiles) {
    src = $$1
    dest = $$2
    # Replace slashes in paths with backslashes for Windows
    win32:src ~= s,/,\\,g
    win32:dest ~= s,/,\\,g

    #win copy command
    win32 {
        QMAKE_POST_LINK +=$$quote(cmd /c copy /y $${src} $${dest}$$escape_expand(\n\t))
        export(QMAKE_POST_LINK)
    }

    #linux command
    unix:!macx {
        QMAKE_POST_LINK += $$quote(cp $${src} $${dest}$$escape_expand(\n\t))
        export(QMAKE_POST_LINK)
    }
}
