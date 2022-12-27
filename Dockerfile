FROM ubuntu:latest
COPY . .
RUN apt-get update && apt-get -y --no-install-recommends install \
    build-essential \
    clang \
    cmake \
    gdb \
    wget
RUN gcc IoTProject_1/src/randValToCClient.c -o randValToCClient
ENTRYPOINT ["./randValToCClient"]
