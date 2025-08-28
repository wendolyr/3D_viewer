FROM ubuntu

RUN apt-get update && apt-get install -y \
    g++ \
    make \
    valgrind \
    libgtest-dev \
    cmake

COPY . /src
WORKDIR /src

CMD ["/bin/bash"]