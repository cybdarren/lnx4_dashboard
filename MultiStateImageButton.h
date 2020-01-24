/*
 * MultiStateImageButton.h
 *
 *  Created on: 20 Jan 2020
 *      Author: masters
 */

#ifndef MULTISTATEIMAGEBUTTON_H_
#define MULTISTATEIMAGEBUTTON_H_

#include <egt/button.h>

namespace egt {
namespace v1 {
namespace user {

class MultiStateImageButton: public ImageButton {
public:
	// create a default image button with only one image
	MultiStateImageButton(const Image& image,
							const std::string& text = {},
							alignmask text_align = DEFAULT_TEXT_ALIGN) noexcept;

	// create an image button with an up and down image
	MultiStateImageButton(const Image& image,
							const Image& altImage,
							const std::string& text = {},
							alignmask text_align = DEFAULT_TEXT_ALIGN) noexcept;

	virtual ~MultiStateImageButton();

	void set_altImage(Image& altImage);

	void handle(Event& event);

protected:
	// default image to display when not pressed
	Image m_defaultImage;
    // alternative image to display when pressed
    Image m_altImage;

};

} /* namespace user */
} /* namespace v1 */
} /* namespace egt */

#endif /* MULTISTATEIMAGEBUTTON_H_ */
