/*
 * GpioEventMonitor.h
 *
 *  Created on: 24 Jan 2020
 *      Author: masters
 */

#ifndef LNX4_DASHBOARD_GPIOEVENTMONITOR_H_
#define LNX4_DASHBOARD_GPIOEVENTMONITOR_H_

#include <egt/ui>
#include <linux/gpio.h>

class GpioEventMonitor {
public:
	using Callback = std::function<void(gpioevent_data& event)>;

	GpioEventMonitor() = delete;

	GpioEventMonitor(int fd, Callback callback)
		: input(egt::Application::instance().event().io()),
		  callback(callback),
		  buffer(sizeof(gpioevent_data))
	{
	    // assign fd to input stream
	    input.assign(fd);

	    // register first read
	    egt::asio::async_read(input, egt::asio::buffer(buffer),
	    				std::bind(&GpioEventMonitor::handle_read,
	    				this, std::placeholders::_1));
	}

	void handle_read(const egt::asio::error_code& error)
	{
		if (error)
		{
			std::cout << "handle_read: " << error.message() << std::endl;
			return;
		}

		const auto event = reinterpret_cast<gpioevent_data*>(buffer.data());
		callback(*event);

		// register any additional async read
		egt::asio::async_read(input, egt::asio::buffer(buffer),
						std::bind(&GpioEventMonitor::handle_read,
						this, std::placeholders::_1));
	}

private:
	// input wrapper around the fd
	egt::asio::posix::stream_descriptor input;
	// registered callback
	Callback callback;
	// buffer to hold the gpioevent_data
	std::vector<char> buffer;
};

#endif /* LNX4_DASHBOARD_GPIOEVENTMONITOR_H_ */
