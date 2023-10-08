
#include <signal.h>
#include <workflow/HttpMessage.h>
#include <workflow/HttpUtil.h>
#include <workflow/WFTaskFactory.h>
#include <workflow/WFFacilities.h>
#include <iostream>
#include <chrono>
#include <atomic>
static int loop=1000;
static WFFacilities::WaitGroup wait_group(loop);
void sig_handler(int signo){
	wait_group.done();
}
int counter = 0;
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
    // auto *resp = httpTask->get_resp();
    // std::cout<<resp->get_status_code()<<std::endl;
    // const void *body;
    // size_t body_len;
    // resp->get_parsed_body(&body, &body_len);

    // std::cout<< static_cast<const char *>(body)<<std::endl;

	// fprintf(stderr, "success\n");
	wait_group.done();
	counter+=1;
	// wait_condition.notify_one();
}

int main(int argc, char *argv[]){
	std::string url = "http://";
	url.append(argv[1]);
	signal(SIGINT, sig_handler);
	int length = 25000;
    std::string send_data(length, 'a');
	
	auto s = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch())
			.count();
	for(int i =0;i<loop;++i){
		auto httpTask = WFTaskFactory::create_http_task(url, 0, 0, callback);
		protocol::HttpRequest *req = httpTask->get_req();

		req->set_method("POST");
		req->append_output_body_nocopy(send_data.c_str(), length); /* nocopy */
		req->add_header_pair("Accept", "*/*");
		req->add_header_pair("User-Agent", "TestAgent");
		req->add_header_pair("Connection", "close");
		httpTask->start();
		while(counter<=i){
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		// std::unique_lock<std::mutex> lock(wait_mutex);
		// wait_condition.wait(lock, [] { return true; });
		// std::cout<<"loop:"<<i<<std::endl;
	}

	wait_group.wait();

	auto e = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch())
				.count();
	std::cout << "HTTP request time consume: " << e - s << "us" << std::endl;
	return 0;
}
