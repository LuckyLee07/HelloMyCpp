#!/bin/bash

# 调用预构建脚本
./tools/premake/premake5 --os=macosx --file=premake.lua xcode4 "./"

echo "Script executed successfully."
