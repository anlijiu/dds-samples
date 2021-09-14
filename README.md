# dds-samples
cyclonedds samples

# build cyclonedds
sudo apt install libcunit1-dev libcunit1 libssh-dev
0.8版本编译可能出错， 原因是用到编译过程中生成的idlc  ，但是此时的libidl.so 还没到/usr/local/lib ， 所以执行会出错， 把build/lib下面的cpy到 /usr/local/lib就可以了
