FROM ubuntu:22.04

RUN apt update && apt install -y \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY *.cpp *.h ./

RUN g++ -Wall -Wextra -O2 -o program main.cpp

CMD ["./program"] 
