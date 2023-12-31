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
    time.sleep(3)

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
    server_lists=[
        "./bazel-bin/profile/socket/server",
        "./bazel-bin/profile/workflow/sync_server",
        "./bazel-bin/profile/workflow/async_server",
        "./bazel-bin/profile/grpc/sync_server",
        "./bazel-bin/profile/grpc/async_server",
    ]
    client_lists=[
        "./bazel-bin/profile/socket/client",
        "./bazel-bin/profile/workflow/sync_client",
        "./bazel-bin/profile/workflow/async_client",
        "./bazel-bin/profile/grpc/sync_client",
        "./bazel-bin/profile/grpc/async_client",
    ]
    for i in range(len(server_lists)):
        server = server_lists[i]
        client = client_lists[i]
        benchmark(server, client, args.length, args.loop,args.port)