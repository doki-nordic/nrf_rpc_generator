#!/bin/bash

/dk/apps/clang/bin/clang -Xclang -ast-dump -fsyntax-only -D_RP_SER_GENERATOR=1 api.c
