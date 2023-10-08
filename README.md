# eval_protocol

Evaluation of communication protocol performance 
通信协议（工具）性能评估


## 评估环境配置

测试使用docker，基于ubuntu的镜像，运行如下命令，启动环境（需要按照docker）

```shell
bash env.sh # or run `make env`
```

##  依赖安装

```shell
./bazel-5.4.0-installer-linux-x86_64.sh # install binary bazel 
apt-get update
apt-get install openssl
apt-get install libssl-dev
openssl version
```

## 评估内容

数据传输场景，多次的发送一定规模的数据（25000字节），评估其耗时。

- 评估grpc 同步传输(unary)
- 评估grpc 异步传输(unary)
- 评估socket 单连接
- 评估httplib post
- 评估workflow post

## 如何评估

```shell
make build
make benchmark
```

## proxy
```shell
export https_proxy="http://127.0.0.1:1080"
export http_proxy="http://127.0.0.1:1080"
```

## bazel

```shell
wget  https://github.com/bazelbuild/bazel/releases/download/6.1.1/bazel-6.1.1-installer-linux-x86_64.sh
sudo bash bazel-6.1.1-installer-linux-x86_64.sh
which bazel
```