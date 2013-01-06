#! /bin/bash #bash path

#can add more options like delete folder if exist

DIR=caseOF/
INSTALLDIR=/opt/cmeshfoil
path=$(pwd)/

if [ -d  "$DIR" ]; then
    cp ${INSTALLDIR}/filesOF/icoFoamFiles/0/* ${path}${DIR}/0
    cp ${INSTALLDIR}/filesOF/icoFoamFiles/constant/* ${path}${DIR}/constant
    cp ${INSTALLDIR}/filesOF/icoFoamFiles/system/* ${path}${DIR}/system
fi
