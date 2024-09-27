/* © Copyright CERN 2021.  All rights reserved. This software is released under a CERN proprietary
 * software licence. Any permission to use it shall be granted in writing. Requests shall be
 * addressed to CERN through mail-KT@cern.ch
 *
 * Author: Yulia Venichenko CERN BE/CEM/MRO 2021
 *         Alejandro Diaz Rosales CERN BE/CEM/MRO 2021
 *
 *  ===============================================================================================
 */

#include <iostream>
#include <stdio.h>

#include "Template/Hello.hpp"
#include "spdlog/spdlog.h"
#include "libusb.h"
#include "ConsoleHelper.h"

static void print_devs(libusb_device **devs)
{
	libusb_device *dev;
	int i = 0, j = 0;
	uint8_t path[8]; 

	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return;
		}

		printf("%04x:%04x (bus %d, device %d)",
			desc.idVendor, desc.idProduct,
			libusb_get_bus_number(dev), libusb_get_device_address(dev));

		r = libusb_get_port_numbers(dev, path, sizeof(path));
		if (r > 0) {
			printf(" path: %d", path[0]);
			for (j = 1; j < r; j++)
				printf(".%d", path[j]);
		}
		printf("\n");
	}
}

int main(int, char**) {
    std::string helloJim = generateHelloString("Yulia");
    std::cout << helloJim << std::endl;

    spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);
    spdlog::warn("Easy padding in numbers like {:08d}", 12);
    spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o};", 42);
    spdlog::info("Support for floats {:03.2f}", 1.23456);
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    spdlog::info("{:<30}", "left aligned");
    spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
    spdlog::debug("This message should be displayed..");

    //test libusb module
	libusb_device **devs;
	int r;
	ssize_t cnt;

	r = libusb_init_context(/*ctx=*/NULL, /*options=*/NULL, /*num_options=*/0);
	if (r < 0)
		return r;

	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0){
		libusb_exit(NULL);
		return (int) cnt;
	}

	print_devs(devs);
	libusb_free_device_list(devs, 1);

	libusb_exit(NULL);

	//test console helper class
	CConsoleHelper chdpp;
	
	spdlog::info("Running DPP LibUsb tests from console...");
	spdlog::info("Connecting to default LibUsb device...");
	if (chdpp.LibUsb_Connect_Default_DPP()) {
		spdlog::info("LibUsb DPP device connected.");
		spdlog::info("LibUsb DPP devices present: {}", chdpp.LibUsb_NumDevices);
		chdpp.LibUsb_Close_Connection();
	} else {
		spdlog::error("LibUsb DPP device not connected." );
		spdlog::error("No LibUsb DPP device present.");
	}

    return 0;
}
