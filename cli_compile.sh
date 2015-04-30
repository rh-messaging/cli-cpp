#!/bin/bash

basepath=$(readlink -f $(dirname $0))

cd ${basepath}

# MRG/M part
bash ${basepath}/../configlib.sh do_node_data_clients_mrgm_cpp_status
if [ "$?" != "0" ]; then
  bash ${basepath}/../configlib.sh do_node_data_clients_mrgm_init
  bash ${basepath}/../configlib.sh do_node_data_clients_mrgm_cpp_init
fi

# JBoss A-MQ part
# TODO

cd -

# eof