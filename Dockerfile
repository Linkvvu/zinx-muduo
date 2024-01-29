FROM vvhao/muduo:latest
WORKDIR /zinx-muduo

RUN sed -i 's/https:\/\/mirrors.aliyun.com/http:\/\/mirrors.cloud.aliyuncs.com/g' /etc/apt/sources.list

RUN apt-get update &&  \
    apt-get install -y wget build-essential cmake && \
    # for libProtobuf and protoc
    apt-get install -y protobuf-compiler libprotobuf-dev && \
    # for json-resolver     
    wget -P /usr/local/include/nlohmann/ https://mirror.ghproxy.com/https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp

COPY . .
    
RUN ./build.sh

CMD [ "./build/release/bin/MMO" ]
