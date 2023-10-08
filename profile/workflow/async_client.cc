
#include <signal.h>
#include <workflow/HttpMessage.h>
#include <workflow/HttpUtil.h>
#include <workflow/WFTaskFactory.h>
#include <workflow/WFFacilities.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

ABSL_FLAG(std::string, target, "http://localhost", "Server address");
ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");
ABSL_FLAG(uint16_t, loop, 1, "client call loop times");
ABSL_FLAG(uint32_t, length, 25000, "send data bytes each time");

static int counter = 0;
static WFFacilities::WaitGroup* wait_group;
void sig_handler(int signo){
	wait_group->done();
}
void callback(WFHttpTask *httpTask){
	int state = httpTask->get_state();
	int error = httpTask->get_error();
	switch (state){
		case WFT_STATE_SYS_ERROR:
			fprintf(stderr, "system error: %s\n", strerror(error));
			break;
		case WFT_STATE_DNS_ERROR:
			fprintf(stderr, "DNS error: %s\n", gai_strerror(error));
			break;
		case WFT_STATE_SUCCESS:
			break;
	}
	if (state != WFT_STATE_SUCCESS){
		fprintf(stderr, "Failed. Press Ctrl-C to exit.\n");
		return;
	}
	counter+=1;
	wait_group->done();
}
int main(int argc, char *argv[]){
	absl::ParseCommandLine(argc, argv);

	std::string target = absl::GetFlag(FLAGS_target);
	uint16_t port = absl::GetFlag(FLAGS_port);
	int loop = absl::GetFlag(FLAGS_loop);
	int length = absl::GetFlag(FLAGS_length);

	std::string server_address = absl::StrFormat("%s:%d",target, port);
    std::string send_data(length, 'a');

	WFFacilities::WaitGroup wait_group_instance(loop);
    wait_group = &wait_group_instance;



	// std::cout<<"=== workflow sync server is:"<<server_address<<" ==="<<std::endl;
	auto s = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch())
			.count();
	for(int i =0;i<loop;++i){
		auto httpTask = WFTaskFactory::create_http_task(server_address, 0, 0,callback);
		protocol::HttpRequest *req = httpTask->get_req();

		req->set_method("POST");
		req->append_output_body_nocopy(send_data.c_str(), length); /* nocopy */
		req->add_header_pair("Accept", "*/*");
		req->add_header_pair("User-Agent", "TestAgent");
		req->add_header_pair("Connection", "close");
		httpTask->start();
	}
	wait_group->wait();
	auto e = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch())
				.count();
	std::cout << "=== workflow async;"<<"port:"<<port<<";loop:"<<loop<<";length:"<<length<<";time:" << e - s << "us ===" << std::endl;
	return 0;
}
