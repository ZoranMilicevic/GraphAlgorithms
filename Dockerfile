FROM gcc:latest

RUN apt-get update && apt-get -y install cmake && apt-get -y install git-all

WORKDIR /usr/src/

RUN git clone https://github.com/ZoranMilicevic/GraphAlgorithms.git

WORKDIR /usr/src/GraphAlgorithms

RUN cmake -S /usr/src/GraphAlgorithms -B /usr/src/GraphAlgorithmsBuild

WORKDIR /usr/src/GraphAlgorithmsBuild

RUN make

CMD [ "./GraphAlgorithms" ]
