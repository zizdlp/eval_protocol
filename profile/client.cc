
#include <signal.h>
#include <workflow/HttpMessage.h>
#include <workflow/HttpUtil.h>
#include <workflow/WFTaskFactory.h>
#include <workflow/WFFacilities.h>
#include <iostream>
static WFFacilities::WaitGroup wait_group(1);
void sig_handler(int signo){
	wait_group.done();
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
    auto *resp = httpTask->get_resp();
    std::cout<<resp->get_status_code()<<std::endl;
    const void *body;
    size_t body_len;
    resp->get_parsed_body(&body, &body_len);

    std::cout<< static_cast<const char *>(body);


	fprintf(stderr, "success\n");
	wait_group.done();
}

int main(int argc, char *argv[]){
	std::string url = "http://";
	url.append(argv[1]);
	signal(SIGINT, sig_handler);
	auto httpTask = WFTaskFactory::create_http_task(url, 0, 0, callback);
	protocol::HttpRequest *req = httpTask->get_req();
	req->add_header_pair("Accept", "*/*");
	req->add_header_pair("User-Agent", "TestAgent");
	req->add_header_pair("Connection", "close");
	httpTask->start();
	wait_group.wait();
	return 0;
}
