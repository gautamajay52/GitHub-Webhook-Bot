FROM ubuntu:20.04

RUN mkdir ./app
RUN chmod 777 ./app
WORKDIR /app

ENV PORT=18080

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Kolkata

RUN apt -qq update && apt -qq install -y build-essential libboost-all-dev curl

COPY . .

RUN g++ src/main.cpp -lpthread -lboost_system -lz -o git.out
RUN chmod +x git.out

EXPOSE 18080

CMD ["/app/git.out"]