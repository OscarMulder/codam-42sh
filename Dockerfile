FROM ubuntu:14.04
LABEL maintainer="Oscar Mulder <oscar.mulder@live.nl>"
RUN apt-get update
RUN apt-get install -y software-properties-common
RUN add-apt-repository -y ppa:snaipewastaken/ppa
RUN apt-get update
RUN apt-get install make
RUN apt-get install -y gcc
RUN apt-get install -y criterion-dev
RUN apt-get install -y libncurses5-dev libncursesw5-dev
WORKDIR /app
COPY . /app
CMD make test
