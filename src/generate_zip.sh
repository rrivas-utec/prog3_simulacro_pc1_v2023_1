#!/usr/bin/env bash

project_name='prog3_simulacro_PC1_v2023_1'
source_code='
        P1.h P1.cpp
        P2.h P2.cpp
  '
rm -f ${project_name}.zip
zip -r -S ${project_name} ${source_code}