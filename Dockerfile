FROM ubuntu:xenial
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y libc6-dev make gcc binutils g++ cpp cpp-5 gcc-5 libasan2 libatomic1 libc-dev-bin libc6-dev libcc1-0 libcilkrts5 libgcc-5-dev libgmp10 libgomp1 libisl15 libitm1 liblsan0 libmpc3 libmpfr4 libmpx0 libquadmath0 libtsan0 libubsan0 linux-libc-dev manpages manpages-dev
RUN apt-get install libboost-all-dev wget -y
RUN wget https://github.com/CrowCpp/Crow/releases/download/v1.0%2B5/crow-v1.0+5.deb
RUN dpkg -i crow-v1.0+5.deb
RUN useradd -rm -d /home/ubuntu -s /bin/bash -g root -G sudo -u 1001 ubuntu
USER ubuntu
WORKDIR /home/ubuntu
COPY . /home/ubuntu
CMD make clean && make && ./main
