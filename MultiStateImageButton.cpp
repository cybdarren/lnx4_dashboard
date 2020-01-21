/*
 * MultiStateImageButton.cpp
 *
 *  Created on: 20 Jan 2020
 *      Author: masters
 */

#include <egt/ui>
#include "MultiStateImageButton.h"

using namespace egt;

namespace egt {
namespace v1 {
namespace user {

MultiStateImageButton::MultiStateImageButton(const Image& image,
											const std::string& text,
											alignmask text_align) noexcept
	: ImageButton(image, text, {}, text_align)
{


}

MultiStateImageButton::~MultiStateImageButton() {
	// TODO Auto-generated destructor stub
}

void MultiStateImageButton::draw(Painter& painter, const Rect& rect)
{
    //Drawer<MultiStateImageButton>::draw(*this, painter, rect);

	Rect target = egt::detail::align_algorithm(image().size(),
											content_area(),
											image_align());
	painter.draw(target.point());
	painter.draw(image());
}

} /* namespace user */
} /* namespace v1 */
} /* namespace egt */
