#!/usr/bin/env bash
SRC_DIR=/opt/libcc/bin/aarch64
DST_DIR=/opt/Mir2Server/bin/aarch64
cp -f -r ${SRC_DIR}/debug/libcc.dylib ${DST_DIR}/debug/
cp -f -r ${SRC_DIR}/release/libcc.dylib ${DST_DIR}/release/
cp -f -r ${SRC_DIR}/release/libcc.dylib ${DST_DIR}/