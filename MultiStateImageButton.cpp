/*
 * MultiStateImageButton.cpp
 *
 *  Created on: 20 Jan 2020
 *      Author: masters
 */

#include <egt/ui>
#include "MultiStateImageButton.h"

using namespace egt;
using namespace std;

namespace egt {
namespace v1 {
namespace user {

MultiStateImageButton::MultiStateImageButton(const Image& image,
											const std::string& text,
											alignmask text_align) noexcept
	: ImageButton(image, text, {}, text_align),
	  m_defaultImage(image)
{

}

MultiStateImageButton::MultiStateImageButton(const Image& image,
											const Image& altImage,
											const std::string& text,
											alignmask text_align) noexcept
	: ImageButton(image, text, {}, text_align),
	  m_defaultImage(image), m_altImage(altImage)
{

}

MultiStateImageButton::~MultiStateImageButton() {
	// TODO Auto-generated destructor stub
}

void MultiStateImageButton::set_altImage(Image& altImage)
{
	m_altImage = altImage;
}

void MultiStateImageButton::handle(Event& event)
{
	ImageButton::handle(event);

	switch (event.id())
	{
	case eventid::raw_pointer_down:
		set_active(true);
		if (!m_altImage.empty())
		{
			do_set_image(m_altImage);
		}
		break;
	case eventid::raw_pointer_up:
		set_active(false);
		do_set_image(m_defaultImage);
		break;
	default:
		break;
	}
}



} /* namespace user */
} /* namespace v1 */
} /* namespace egt */
