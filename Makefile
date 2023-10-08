LOOP ?= 1
LENGTH ?= 25000

build:
	bazel build //profile/workflow:sync_server
	bazel build //profile/workflow:sync_client
benchmark:
	python benchmark.py --loop=$(LOOP) --length=$(LENGTH)
.PHONY: workflow_sync_server workflow_sync_client benchmark