#!/bin/bash

set -e

BUILD_VARIANT="release"
if [ "${DEBUG_BUILD}" == "1" ]; then
    BUILD_VARIANT="debug"
fi

if [ "${ARCH_TRIPLET}" == "arm-linux-gnueabihf" ]; then
    ARCH=armhf
elif [ "${ARCH_TRIPLET}" == "x86_64-linux-gnu" ]; then
    ARCH=amd64
elif [ "${ARCH_TRIPLET}" == "aarch64-linux-gnu" ]; then
	ARCH=arm64
else
    echo "unsupported target architecture ${ARCH_TRIPLET}"
    exit 1
fi

DIR=$(dirname "${BASH_SOURCE[0]}")
cat ${DIR}/sageteamail.qbs.in | sed "s/%HOST_ARCH%/${ARCH}/" > ${DIR}/sageteamail.qbs
cat ${DIR}/click/sageteamail2.desktop.in | sed "s/%HOST_ARCH_TRIPLET%/${ARCH_TRIPLET}/g" > ${DIR}/click/sageteamail2.desktop

function install_python_deps
{
    PYTHON_DEPS="bs4 cssutils encutils html2text jinja2 markdown markupsafe pygments pynliner soupsieve zipp"
    PYTHON_DEPS_DIR=pylibs/lib/python3.8/site-packages/
    SAGETEAMAIL_PYTHON_DIR=$CLICK_LD_LIBRARY_PATH/SageteaMail/Python/

    mkdir -p $SAGETEAMAIL_PYTHON_DIR
    for dep in $PYTHON_DEPS; do
        cp -r $PYTHON_DEPS_DIR/$dep $SAGETEAMAIL_PYTHON_DIR
    done
    
    cp -r $PYTHON_DEPS_DIR/importlib_metadata* $SAGETEAMAIL_PYTHON_DIR
}

ROOT="$( cd "$DIR" >/dev/null 2>&1 && pwd )"
cd $ROOT

if [ "$ARCH" == "amd64" ]; then
    qbs setup-toolchains /usr/bin/gcc gcc-$ARCH
else
    qbs setup-toolchains /usr/bin/${ARCH_TRIPLET}-gcc gcc-$ARCH
fi

qbs setup-qt /usr/lib/${ARCH_TRIPLET}/qt5/bin/qmake sageteamailqt5-$ARCH
qbs config profiles.sageteamailqt5-$ARCH.baseProfile gcc-$ARCH
qbs config profiles.sageteamailqt5-$ARCH.Qt.core.binPath /usr/lib/x86_64-linux-gnu/qt5/bin

export DIR_PREFIX=/lib/${ARCH_TRIPLET}
export BIN_DIR=$DIR_PREFIX/bin
export LIB_DIR=$DIR_PREFIX
export DATA_DIR=/usr/share/sageteamail
export QML_DIR=$LIB_DIR

qbs build -d $BUILD_DIR -f . --clean-install-root --show-progress config:build project.click:true project.binDir:$BIN_DIR project.libDir:$LIB_DIR project.qmlDir:$QML_DIR project.dataDir:$DATA_DIR profile:sageteamailqt5-$ARCH qbs.buildVariant:$BUILD_VARIANT qbs.installPrefix:/

install_python_deps

mv ${INSTALL_DIR}/lib/SageteaMail/Controls ${INSTALL_DIR}/lib/${ARCH_TRIPLET}/SageteaMail/
