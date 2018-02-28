/*
 * JSON_RPC._cpp
 *
 *  Created on: 27.10.2017
 *      Author: marcel.seerig
 */

#include <string>
#include <esp_log.h>

#include "Task.h"
#include "FreeRTOS.h"
#include "sdkconfig.h"

#include "JSON_RPC.h"
#include "JSON.h"

extern "C" {
void app_main(void);
}

// Simple test class
class Parameter{
    public:
        Parameter();
        int test;
};

Parameter::Parameter(){
    test = 12345; // Simple test member
}

// This is my JSON-RPC callback function!
int helloWorld(JsonObject *io, void* data){
	//get my pointer to my Parameter class
    Parameter* m_para = (Parameter*) data; // not needed in this example

    // looking for the given parameters
    // returning these, if it is an string
    if(io->isStringItem("params")) io->setString("result", io->getString("params"));
    else return JSONRPC_INVALID_PARAMETER; // return an error, if not

    return 0; // means success
}

class Main_Task: public Task {
	void run(void *args) override {
		Parameter para; // test class

		//create JSON-RPC server
		JSON_RPC rpc;
		// add my callback function to the parser
		rpc.addMethod(helloWorld, "helloWorld", &para);

		// create example JSON-RPC call
		JsonObject jsonObject = JSON::createObject();
		jsonObject.setString("jsonrpc", JSONRPC_VERSION);
		jsonObject.setString("method", "helloWorld");
		jsonObject.setString("params", "Halo123");
		jsonObject.setString("id", "1");

		ESP_LOGD("App","JSON-RPC Request: %s",jsonObject.toString().c_str());

		std::string response;

		for(;;){
			// parse my given JSON-RPC request string in to a JSON-RPC Response string
			response = rpc.parse(jsonObject.toString());

			ESP_LOGD("App","JSON-RPC Response: %s", response.c_str());
			FreeRTOS::sleep(1000);
		}
	}
}
;

static Main_Task *main_Task;

void app_main(void) {

	main_Task = new Main_Task();
	main_Task->start();

}
