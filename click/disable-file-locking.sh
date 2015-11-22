#!/bin/bash

# $1 is the temp folder where the packages have been extracted
TEMP_PATH=$1
LO_PATH=$TEMP_PATH/opt/libreoffice/lib/libreoffice
LO_REGISTRY=$LO_PATH/share/registry

THIS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "Disable file locking in LibreOffice"
cp $THIS_DIR/disable-file-locking.xcd $LO_REGISTRY
