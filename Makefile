LOOP ?= 10
LENGTH ?= 25000

build:
	bazel build //profile/workflow:sync_server
	bazel build //profile/workflow:sync_client
	bazel build //profile/workflow:async_server
	bazel build //profile/workflow:async_client
benchmark:
	python benchmark.py --loop=$(LOOP) --length=$(LENGTH)
.PHONY: workflow_sync_server workflow_sync_client benchmark