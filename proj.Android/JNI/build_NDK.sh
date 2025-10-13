#!/bin/bash
sourcePath="$0"
# resolve $sourcePath until the file is no longer a symlink
while [ -h "$sourcePath"  ]; do
basePath="$( cd -P "$( dirname "$sourcePath"  )" && pwd  )"
sourcePath="$(readlink "$sourcePath")"
# if $sourcePath was a relative symlink,
# we need to resolve it relative to the path where the symlink file was located
[[ $sourcePath != /*  ]] && sourcePath="$basePath/$sourcePath"

done
basePath="$( cd -P "$( dirname "$sourcePath"  )" && pwd  )"
export NDK_HOME=~/Library/Android/sdk/ndk/29.0.14033849/
echo "NDK_HOME = $NDK_HOME"
echo "APP_ROOT = $basePath"

cd $basePath
bash $NDK_HOME/ndk-build
