FROM gcc:12.2

COPY . /usr/src/myapp

WORKDIR /usr/src/myapp

RUN make CC=gcc test

RUN make CC=gcc

CMD ["./bin/main"]
