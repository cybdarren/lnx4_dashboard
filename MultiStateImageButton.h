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
	MultiStateImageButton(const Image& image,
							const std::string& text = {},
							alignmask text_align = DEFAULT_TEXT_ALIGN) noexcept;
	virtual ~MultiStateImageButton();

    virtual void draw(Painter& painter, const Rect& rect) override;

};

} /* namespace user */
} /* namespace v1 */
} /* namespace egt */

#endif /* MULTISTATEIMAGEBUTTON_H_ */
