env:
	bash env.sh
server:
	bazel run //profile:server
echo:
	bazel run //profile:echo_server 5001
post_server:
	bazel run //profile:post_server 5001
profile_server:
	bazel run //profile:profile_server 5002
client:
	bazel run //profile:client localhost:5001
post_client:
	bazel run //profile:post_client localhost:5001
profile_client:
	bazel run //profile:profile_client localhost:5002
file:
	bazel run //profile:file 5001
.PHONY: env