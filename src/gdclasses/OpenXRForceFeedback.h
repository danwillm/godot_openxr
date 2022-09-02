#ifndef OPENXR_FORCE_FEEDBACK_H
#define OPENXR_FORCE_FEEDBACK_H

#include "openxr/OpenXRApi.h"
#include "openxr/extensions/xr_mndx_force_feedback_extension_wrapper.h"
#include <Node.hpp>

namespace godot {
class OpenXRForceFeedback : public Node {
	GODOT_CLASS(OpenXRForceFeedback, Node)

public:
	OpenXRForceFeedback();
	~OpenXRForceFeedback();

	static void _register_methods();

	void _init();
	void _ready();

	bool is_active();

	void set_force_feedback(float thumb, float index, float middle, float ring, float pinky);
	void relax_force_feedback();

private:
	OpenXRApi* openxr_api;
	XRMNDXForceFeedbackExtensionWrapper *force_feedback_wrapper = nullptr;

	int hand;
};
} // namespace godot

#endif