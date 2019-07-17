#!/bin/sh

# mkdir ../out
rm -rf ./out/*
cp ../../out/target/product/generic_arm64/system/vendor/bin/* ../out/
cp ../../out/target/product/generic_arm64/system/vendor/lib64/* ../out/
