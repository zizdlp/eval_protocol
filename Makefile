LOOP ?= 10
LENGTH ?= 25000
RANDOM_PORT := $(shell shuf -i 50001-50099 -n 1)
PORT ?= $(RANDOM_PORT)
build:
	bazel build //profile/workflow:sync_server
	bazel build //profile/workflow:sync_client
	bazel build //profile/workflow:async_server
	bazel build //profile/workflow:async_client
	bazel build //profile/socket:server
	bazel build //profile/socket:client
benchmark:
	python benchmark.py --loop=$(LOOP) --length=$(LENGTH) --port=$(PORT)
.PHONY: workflow_sync_server workflow_sync_client benchmark