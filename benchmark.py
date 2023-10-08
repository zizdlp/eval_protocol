import subprocess
import time
import argparse

def benchmark(server, client, length=None, loop=None,port=None):
    # 启动 sync_server 进程
    server_args = [server]
    if port is not None:
        server_args.append(f"-port={port}")
    server_process = subprocess.Popen(server_args)

    # 等待一段时间确保 sync_server 完全启动
    time.sleep(5)

    # 启动 sync_client 进程
    # 准备参数列表
    client_args = [client]
    if length is not None:
        client_args.append(f"-length={length}")
    if loop is not None:
        client_args.append(f"-loop={loop}")
    if port is not None:
        client_args.append(f"-port={port}")
    client_process = subprocess.Popen(client_args)

    # 等待 sync_client 进程结束
    client_process.wait()

    # 关闭 sync_server 进程
    server_process.terminate()
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Benchmarking tool")
    parser.add_argument("--length", type=int, help="Length parameter")
    parser.add_argument("--loop", type=int, help="loop parameter")
    parser.add_argument("--port", type=int, help="port parameter")

    args = parser.parse_args()

    server = "./bazel-bin/profile/workflow/sync_server"
    client = "./bazel-bin/profile/workflow/sync_client"
    benchmark(server, client, args.length, args.loop,args.port)