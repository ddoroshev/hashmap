FROM gcc:13.2

COPY . /usr/src/myapp

WORKDIR /usr/src/myapp

RUN make test

RUN make

CMD ["./bin/main"]
