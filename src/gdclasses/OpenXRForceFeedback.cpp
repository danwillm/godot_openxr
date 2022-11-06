#include "OpenXRForceFeedback.h"

#include <string>

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
	register_method("relax_force_feedback", &OpenXRForceFeedback::relax_force_feedback);
	register_method("set_force_feedback", &OpenXRForceFeedback::set_force_feedback);
	register_method("is_active", &OpenXRForceFeedback::is_active);

	register_property<OpenXRForceFeedback, float>("thumb", &OpenXRForceFeedback::thumb_value, 0.0f);
	register_property<OpenXRForceFeedback, float>("index", &OpenXRForceFeedback::index_value, 0.0f);
	register_property<OpenXRForceFeedback, float>("middle", &OpenXRForceFeedback::middle_value, 0.0f);
	register_property<OpenXRForceFeedback, float>("ring", &OpenXRForceFeedback::ring_value, 0.0f);
	register_property<OpenXRForceFeedback, float>("pinky", &OpenXRForceFeedback::pinky_value, 0.0f);
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

void OpenXRForceFeedback::relax_force_feedback(uint32_t hand) {
	ForceFeedbackData result{};

	result.thumb = 0.0f;
	result.index = 0.0f;
	result.middle = 0.0f;
	result.ring = 0.0f;
	result.pinky = 0.0f;

	force_feedback_wrapper->set_force_feedback(hand, result);
}

void OpenXRForceFeedback::set_force_feedback(uint32_t hand) {
	Godot::print(("force feedback activated " + std::to_string(thumb_value) + "\n").c_str());

	ForceFeedbackData result{};

	result.thumb = thumb_value;
	result.index = index_value;
	result.middle = middle_value;
	result.ring = ring_value;
	result.pinky = pinky_value;

	force_feedback_wrapper->set_force_feedback(hand, result);
}
