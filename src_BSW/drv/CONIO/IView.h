/*
 * IView.h
 *
 *  Created on: 26-Apr-2017
 *      Author: Rahul
 */

#ifndef SRC_ASW_UI_IVIEW_H_
#define SRC_ASW_UI_IVIEW_H_

#include "ui.h"

namespace ui {

class IView {
public:
	virtual RC_t render() = 0;

	virtual RC_t model() = 0;
};

} /* namespace ui */

#endif /* SRC_ASW_UI_IVIEW_H_ */
