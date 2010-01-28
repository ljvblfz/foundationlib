#!/bin/sh
#debug
cd debug
make clean
make 
echo "###################make debug end"
cd ..
#pthreadWrap
cd pthreadWrap
make clean
make 
echo "###################make pthreadWrap end"
cd ..
#netApiWrap
cd netApiWrap
make clean
make 
echo "###################make netApiWrap end"
cd ..
#ioApiWrap
cd ioApiWrap
make clean
make 
echo "###################make ioApiWrap end"
cd ..
#timeApiWrap
cd timeApiWrap
make clean
make 
echo "###################make timeApiWrap end"
cd ..
#sysTset
cd sysTest 
make clean
make 
echo "###################make sysTest end"
cd ..






