#!/bin/bash

#cyclonedds
scp libddsc.so.0.9.0 libdds_security_ac.so libdds_security_auth.so libdds_security_crypto.so root@192.168.1.1:/usr/lib64/
ssh root@192.168.1.1
cd /usr/lib64
ln -sf libddsc.so.0.9.0 libddsc.so.0
ln -sf libddsc.so.0 libddsc.so

#cyclonedds-cxx
scp libddscxx.so.0.9.0  root@192.168.1.1:/usr/lib64/
ssh root@192.168.1.1
cd /usr/lib64
ln -sf libddscxx.so.0.9.0 libddscxx.so.0
ln -sf libddscxx.so.0 libddscxx.so

#dashboard-demo
scp dashboard-demo-cyclonedds root@192.168.1.1:/usr/bin
ssh root@192.168.1.1
mkdir /etc/cyclonedds/
scp config.xml root@192.168.1.1:/etc/cyclonedds/
