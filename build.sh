
#! /usr/bin/env bash

clean_flag="N"
build_type="Release"
check_memory="N"
CMAKE_BUILD_PATH=cmake-build


usage() {
    echo "usage: $0 [cdmh]"
    echo -e "\t-h: help"
    echo -e "\t-c: to clean"
    echo -e "\t-d: set build type: Debug"
    echo -e "\t-m: enbale check memory"
}

OPTIND=1
while getopts "cdmh" arg;do
    case "$arg" in
        c)
            clean_flag="Y"
            ;;
        d)
            build_type="Debug"
            ;;
        m)
            check_memory="Y"
            ;;
        h)
            usage
            exit 0
            ;;
        ?)
            usage
            exit -1
            ;;
    esac
done

if [[ $clean_flag == "Y" ]];then
echo "make clean"
rm -rf $CMAKE_BUILD_PATH
exit 0
fi

echo "build type: $build_type"
mkdir -p $CMAKE_BUILD_PATH

cd $CMAKE_BUILD_PATH
cmake -DCMAKE_BUILD_TYPE=$build_type -DCHECK_MEMORY=$check_memory ..
make -j3
cd -
