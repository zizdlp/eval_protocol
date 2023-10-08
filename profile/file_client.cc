
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "workflow/HttpMessage.h"
#include "workflow/HttpUtil.h"
#include "workflow/WFServer.h"
#include "workflow/WFHttpServer.h"
#include "workflow/WFFacilities.h"
#include <iostream>

void http_callback(WFHttpTask *http_task)
{
    // free(http_task->user_data);
    // ...
    std::cout<<"http_callback called"<<std::endl;
    protocol::HttpResponse *resp = http_task->get_resp();
    std::cout<<"status:"<< resp->get_status_code();
    std::cout<<"---"<<std::endl;
    // // response body
    // const void *body;
    // size_t body_len;
    // resp->get_parsed_body(&body, &body_len);
    // free(http_task->user_data);
    // spdlog::info("resp info : {}", static_cast<const char *>(body));
}
// static WFFacilities::WaitGroup wait_group(1);
int main(int argc, char *argv[])
{
    int length = 25000;
    std::string send_data(length, 'a');
	WFHttpTask *http_task; /* for requesting remote webserver. */
    http_task = WFTaskFactory::create_http_task("localhost:5001", 0, 0,http_callback);
    http_task->get_req()->set_method("POST");
    http_task->get_req()->append_output_body_nocopy(send_data.c_str(), length); /* nocopy */
    // http_task->user_data = send_data.c_str(); /* To free. */
    http_task->start();
    // wait_group.wait();
}

