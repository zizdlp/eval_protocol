# eval_protocol

Evaluation of communication protocol performance 
通信协议（工具）性能评估


## 评估环境配置

测试使用docker，基于ubuntu的镜像，运行如下命令，启动环境（需要按照docker）

```shell
bash env.sh # or run `make env`
```


## proxy
```shell
export https_proxy="http://127.0.0.1:1080"
export http_proxy="http://127.0.0.1:1080"
```