FROM debian:bookworm

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libopencv-dev \
    v4l-utils \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN cmake -S . -B build
RUN cmake --build build

CMD ["./build/vanttecUSV"]