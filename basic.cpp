/*
 * basic.cpp
 *
 *  Created on: 8 Jan 2020
 *      Author: masters
 *      To halt execution on the target kill -SIGUSR1 pid
 *      Then resume in the debugger window and it will print out the EGT object stack
 */

#include <egt/ui>
#include <iostream>
#include <fstream>
#include <linux/input.h>
#include "MultiStateImageButton.h"
#include "GpioEventMonitor.h"

using namespace egt;
using namespace std;
using namespace egt::experimental;
using namespace egt::v1::user;

template<class T>
static void demo_up_down_animator(std::shared_ptr<T> widget, int min, int max, std::chrono::seconds duration = std::chrono::seconds(10))
{
	auto animationup = std::make_shared<PropertyAnimator>(min, max, duration, easing_linear);
	animationup->on_change(std::bind(&T::set_value, std::ref(*widget), std::placeholders::_1));

	auto animationdown = std::make_shared<PropertyAnimator>(max, min, duration, easing_linear);
	animationdown->on_change(std::bind(&T::set_value, std::ref(*widget), std::placeholders::_1));

	auto sequence = new AnimationSequence(true);
	sequence->add(animationup);
	sequence->add(animationdown);
	sequence->start();
}

// create an image button to go on the dashboard with an up/down state
static std::shared_ptr<MultiStateImageButton> create_button_from_svg(TopWindow &parent, SvgImage &svgSrc,
		const string res_name, const string res_name2 = "")
{
	// get the bounding box for the button in the SVG image
	RectF boundingBox = svgSrc.id_box(res_name);
	// get just the image clipped to the bounding box
	auto btnImageDefault = make_shared<Image>(svgSrc.id(res_name, boundingBox));
	// create an ImageButton
	auto btn = make_shared<MultiStateImageButton>(*btnImageDefault);

	if (res_name2 != "")
	{
		RectF altBoundingBox = svgSrc.id_box(res_name2);
		auto altButtonImage = make_shared<Image>(svgSrc.id(res_name2, altBoundingBox));
		btn->set_altImage(*altButtonImage);
	}

	// prevent the button auto-resizing and drawing the background (only draw the image)
	btn->flags().set(Widget::flag::no_autoresize);
	btn->set_boxtype(Theme::boxtype::none);
	// move the button to the same location as in the Svg file
	btn->move(Point(boundingBox.x(), boundingBox.y()));
	btn->set_name(res_name);

	parent.add(btn);
	return btn;
}

int main(int argc, const char** argv)
{
	Application app(argc, argv);
	TopWindow win;

	win.set_color(Palette::ColorId::bg, Palette::black);

auto start = chrono::steady_clock::now();
	// load the background image directly from the PNG file for speed
	win.set_background(Image("dashboard_bg.png"));
auto end = chrono::steady_clock::now();
cout << "Time to load background png: " << chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ms" << endl;

	// load the svg image to allow selection of discrete objects
	SvgImage dashboard_svg("dashboard.svg", SizeF(win.content_area().width(), 0));

	// Create the list box for diagnostic output on the dashboard
	RectF txtRect = dashboard_svg.id_box("#rect_list");
	Rect iTxtRect = Rect(txtRect.x(), txtRect.y(), txtRect.width(), txtRect.height());

	auto txtDiag = make_shared<TextBox>(u8"EGT Dashboard demo for LNX4 "
			"This is a multiline text box", iTxtRect);
	txtDiag->text_flags().set((TextBox::flag::multiline));
	txtDiag->set_border(0);
	txtDiag->set_color(Palette::ColorId::bg, Palette::black);
	txtDiag->set_color(Palette::ColorId::text, Palette::white);
	win.add(txtDiag);

	// create a ScrolledView to hold the text output window
//	auto scrollView = make_shared<ScrolledView>(iTxtRect);

//	win.add(scrollView);


	// Create the MPH gauge
	// As an exercise in logical programming the gauge is created with 3 layers
	//     Background, which is also the largest part of the gauge
	//     Needle, which is animated
	//     Hub which is located at the center and on top of the needle
	// These objects are created relative to the gauge and then the gauge is positioned to the
	// correct place on the screen
	Gauge mph_gauge;

	RectF mph_bg_box = dashboard_svg.id_box("#mph_bg_bmp");
	Image mph_bg_img = dashboard_svg.id("#mph_bg_bmp", mph_bg_box);
	GaugeLayer mph_bg_layer(mph_bg_img);
	mph_gauge.add_layer(mph_bg_layer);

	RectF mph_needle_box = dashboard_svg.id_box("#mphneedle");
	auto mph_gauge_needle = make_shared<NeedleLayer>(dashboard_svg.id("#mphneedle", mph_needle_box),
								0, 180, -135, 135);
	// set the rotation center within the needle object
	mph_gauge_needle->set_needle_center(PointF(mph_needle_box.width() / 2.0, 112.0));
	// set the rotation center relative to the gauge
	mph_gauge_needle->set_needle_point(PointF(mph_bg_box.center()) - PointF(mph_bg_box.top_left()));
	mph_gauge.add_layer(mph_gauge_needle);

	// assign a demo animator to the mph gauge
	demo_up_down_animator(mph_gauge_needle, 0, 180, std::chrono::seconds(5));

	RectF center_point = dashboard_svg.id_box("#mph_hub_bmp");
	Image hub_img = dashboard_svg.id("#mph_hub_bmp", center_point);
	GaugeLayer mph_gauge_center(hub_img);
	mph_gauge_center.move_to_center(mph_bg_layer.center());
	mph_gauge.add_layer(mph_gauge_center);

	mph_gauge.move(Point(mph_bg_box.x(), mph_bg_box.y()));
	win.add(mph_gauge);

	// create the buttons on the dashboard and add handlers for the presses
	shared_ptr<MultiStateImageButton> left_btn = create_button_from_svg(win, dashboard_svg, "#btn_left", "#btn_left2");
	left_btn->on_event([] (Event& event)
	{
		cout << "Left button click" << endl;
	}, {eventid::pointer_click});

	shared_ptr<MultiStateImageButton> right_btn = create_button_from_svg(win, dashboard_svg, "#btn_right", "#btn_right2");
	right_btn->on_event([] (Event& event)
	{
		cout << "Right button click" << endl;
	}, {eventid::pointer_click});

	shared_ptr<MultiStateImageButton> front_lights_btn = create_button_from_svg(win, dashboard_svg, "#frontlights");
	front_lights_btn->on_event([] (Event& event)
	{
		cout << "Front lights click" << endl;
	}, {eventid::pointer_click});

	shared_ptr<MultiStateImageButton> rear_lights_btn = create_button_from_svg(win, dashboard_svg, "#rearlights");
	rear_lights_btn->on_event([] (Event& event)
	{
		cout << "Rear lights click" << endl;
	}, {eventid::pointer_click});

	shared_ptr<MultiStateImageButton> start_btn = create_button_from_svg(win, dashboard_svg, "#btn_start", "#btn_start2");
	start_btn->on_event([] (Event& event)
	{
		cout << "Start button click" << endl;
	}, {eventid::pointer_click});


	// create the progress bar to display RPM. Rather than using an object from the svg like the buttons we
	// instead  use the size of the rpm rectangle to calculate the position on screen and place a standard
	// ProgressBar widget in the same location
	RectF rpm_rect = dashboard_svg.id_box("#rect_rpm");
	Rect intRpmRect = Rect(rpm_rect.x(), rpm_rect.y(), rpm_rect.width(), rpm_rect.height());
	ProgressBar rpm_bar(intRpmRect, 0, 100, 0);
	rpm_bar.set_show_label(false);
	win.add(rpm_bar);

	// animator for acceleration
	PropertyAnimator anim_accelerate(0, 100, std::chrono::milliseconds(6000), easing_easy);
	anim_accelerate.on_change(std::bind(&ProgressBar::set_value, std::ref(rpm_bar),
            std::placeholders::_1));

	// animator for deceleration
	PropertyAnimator anim_decelerate(100, 0, std::chrono::milliseconds(4000), easing_easy);
	anim_decelerate.on_change(std::bind(&ProgressBar::set_value, std::ref(rpm_bar),
			std::placeholders::_1));

	shared_ptr<MultiStateImageButton> accel_btn = create_button_from_svg(win, dashboard_svg, "#btn_accel");
	accel_btn->on_event([&anim_accelerate, &anim_decelerate, &rpm_bar, txtDiag] (Event& event)
	{
		switch (event.id())
		{
		case eventid::raw_pointer_down:
		case eventid::keyboard_down:
			// stop any deceleration
			anim_decelerate.stop();
			// start accelerating again from the current rpm
			anim_accelerate.set_starting(rpm_bar.value());
			anim_accelerate.start();
			txtDiag->append("Accelerating\n");
			break;
		case eventid::raw_pointer_up:
		case eventid::keyboard_up:
			// stop acceleration
			anim_accelerate.stop();
			// start deceleration
			anim_decelerate.set_starting(rpm_bar.value());
			anim_decelerate.start();
			break;
		default:
			break;
		}
	});


	int fd = open("/dev/input/event2", O_RDONLY);
	if (fd < 0)
	{
		cerr << "Failed to open /dev/input/event2" << std::endl;
		return 1;
	}

	// start a monitor
	GpioEventMonitor eventMonitor(fd, [accel_btn](gpioevent_data& event)
	{
		// we are not expecting events from anything other than the USER button so
		// we will play fast and loose with the cast
		const auto inputEvent = reinterpret_cast<input_event*>(&event);
		if (inputEvent->code == 0x104)
		{
			Event ev;

			// USER gpio activity
			if (inputEvent->value == 1)
			{
				// GPIO button pressed
				ev.set_id(eventid::keyboard_down);
			}
			else
			{
				// GPIO button released
				ev.set_id(eventid::keyboard_up);
			}

			// USER events are sent to the accelerator button
			accel_btn->handle(ev);
		}
	});

	win.show();

	return app.run();
}



