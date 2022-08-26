#include "OpenXRForceFeedback.h"

using namespace godot;

#define RETURN_IF_NULLPTR(var) \
	{                          \
		if (var == nullptr)    \
			return;            \
	}

OpenXRForceFeedback::OpenXRForceFeedback() {
	openxr_api = OpenXRApi::openxr_get_api();
	force_feedback_wrapper = XRMNDXForceFeedbackExtensionWrapper::get_singleton();
}

OpenXRForceFeedback::~OpenXRForceFeedback() {

}

void OpenXRForceFeedback::_register_methods() {
	register_method("_ready", &OpenXRForceFeedback::_ready);
	register_method("_init", &OpenXRForceFeedback::_init);
	register_method("set_force_feedback", &OpenXRForceFeedback::set_force_feedback);

	register_method("is_active", &OpenXRForceFeedback::is_active);
}

void OpenXRForceFeedback::_init() {}

void OpenXRForceFeedback::_ready() {}

bool OpenXRForceFeedback::is_active() {
	if (openxr_api == nullptr || force_feedback_wrapper == nullptr) {
		return false;
	}
	if (!openxr_api->is_initialised())
		return false;

	return true;
}

void OpenXRForceFeedback::set_force_feedback() {
	RETURN_IF_NULLPTR(force_feedback_wrapper);


}