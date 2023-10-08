#include <stdio.h>
#include "workflow/WFHttpServer.h"
#include <iostream>
using namespace std;
int main()
{
    WFHttpServer server([](WFHttpTask *task) {
        task->get_resp()->append_output_body("<html>Hello World!</html>");
    });

    if (server.start(50052) == 0) { // start server on port 8888
        getchar(); // press "Enter" to end.
        server.stop();
    }
    return 0;
}

