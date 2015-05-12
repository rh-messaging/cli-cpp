#!/bin/bash

basepath=$(readlink -f $(dirname $0))

pushd ${basepath}/..

# MRG/M part
bash ../configlib.sh do_node_data_clients_mrgm_cpp_status
if [ "$?" != "0" ]; then
  bash ../configlib.sh do_node_data_clients_mrgm_init
  bash ../configlib.sh do_node_data_clients_mrgm_cpp_init
fi

# JBoss A-MQ part
bash ../configlib.sh do_node_data_clients_jamq_cpp_status
if [ "$?" != "0" ]; then
  bash ../configlib.sh do_node_data_clients_jamq_init
  bash ../configlib.sh do_node_data_clients_jamq_cpp_init
fi

popd

# eof