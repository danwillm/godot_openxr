#include "xr_mndx_force_feedback_extension_wrapper.h"
#include "xr_ext_hand_tracking_extension_wrapper.h"

#define XR_SUCCESS_OR_RETURN(func) \
	XrResult result = func;        \
	if (result != XR_SUCCESS) {    \
		return result;             \
	}

XRMNDXForceFeedbackExtensionWrapper::XRMNDXForceFeedbackExtensionWrapper() {
	openxr_api = OpenXRApi::openxr_get_api();

	request_extensions[XR_MNDX_FORCE_FEEDBACK_CURL_EXTENSION_NAME] = &force_feedback_ext;
}

XRMNDXForceFeedbackExtensionWrapper *XRMNDXForceFeedbackExtensionWrapper::singleton = nullptr;

XRMNDXForceFeedbackExtensionWrapper *XRMNDXForceFeedbackExtensionWrapper::get_singleton() {
	if (!singleton) {
		singleton = new XRMNDXForceFeedbackExtensionWrapper();
	}

	return singleton;
}

XRMNDXForceFeedbackExtensionWrapper::~XRMNDXForceFeedbackExtensionWrapper() {
	cleanup();
	OpenXRApi::openxr_release_api();
}

void XRMNDXForceFeedbackExtensionWrapper::on_instance_initialized(const XrInstance instance) {
	if (force_feedback_ext) {
		XrResult result = initialise_mndx_force_feedback_extension(instance);
		if (!openxr_api->xr_result(result, "Failed to initialise force feedback extension")) {
			force_feedback_ext = false;
		}
	}
}

void XRMNDXForceFeedbackExtensionWrapper::on_state_ready() {
	initialise_force_feedback();
}

PFN_xrApplyForceFeedbackCurlMNDX xrApplyForceFeedbackCurlMNDX_ptr = nullptr;

XRAPI_ATTR XrResult XRAPI_CALL XRMNDXForceFeedbackExtensionWrapper::xrApplyForceFeedbackCurlMNDX(XrHandTrackerEXT handTracker, const XrApplyForceFeedbackCurlLocationsMNDX *locations) {
	if (xrApplyForceFeedbackCurlMNDX_ptr == nullptr) {
		return XR_ERROR_HANDLE_INVALID;
	}

	return (*xrApplyForceFeedbackCurlMNDX_ptr)(handTracker, locations);
}

XrResult XRMNDXForceFeedbackExtensionWrapper::initialise_mndx_force_feedback_extension(XrInstance instance) {
	XrResult result;

	result = xrGetInstanceProcAddr(instance, "xrApplyForceFeedbackCurlMNDX", (PFN_xrVoidFunction *)&xrApplyForceFeedbackCurlMNDX_ptr);
	if (result != XR_SUCCESS) {
		return result;
	}

	return XR_SUCCESS;
}

bool XRMNDXForceFeedbackExtensionWrapper::initialise_force_feedback() {
	XrSystemForceFeedbackCurlPropertiesMNDX forceFeedbackSystemProperties = {
		.type = XR_TYPE_SYSTEM_FORCE_FEEDBACK_CURL_PROPERTIES_MNDX
	};

	XrSystemProperties systemProperties = {
		.type = XR_TYPE_SYSTEM_PROPERTIES,
		.next = &forceFeedbackSystemProperties
	};

	XrResult result = xrGetSystemProperties(openxr_api->get_instance(), openxr_api->get_system_id(), &systemProperties);
	if (!forceFeedbackSystemProperties.supportsForceFeedbackCurl) {
		Godot::print("Force feedback is not supported\n");
		return false;
	}

	XRExtHandTrackingExtensionWrapper *handTrackingWrapper = XRExtHandTrackingExtensionWrapper::get_singleton();

	for (int i = 0; i < 2; i++) {
		force_feedback[i].is_initialised = true;
		force_feedback[i].hand_tracker = handTrackingWrapper->get_hand_tracker(i)->hand_tracker;
	}

	return result == XR_SUCCESS;
}

void XRMNDXForceFeedbackExtensionWrapper::set_force_feedback(uint32_t hand, const ForceFeedbackData &data) {
	XrApplyForceFeedbackCurlLocationsMNDX result;

	const uint32_t locationCount = 5;
	result.locationCount = locationCount;

	XrApplyForceFeedbackCurlLocationMNDX locations[locationCount];

	locations[0].value = data.thumb;
	locations[0].location = XR_FORCE_FEEDBACK_CURL_LOCATION_FINGER_THUMB_CURL_MNDX;

	locations[1].value = data.index;
	locations[1].location = XR_FORCE_FEEDBACK_CURL_LOCATION_FINGER_INDEX_CURL_MNDX;

	locations[2].value = data.middle;
	locations[2].location = XR_FORCE_FEEDBACK_CURL_LOCATION_FINGER_MIDDLE_CURL_MNDX;

	locations[3].value = data.ring;
	locations[3].location = XR_FORCE_FEEDBACK_CURL_LOCATION_FINGER_RING_CURL_MNDX;

	locations[4].value = data.pinky;
	locations[4].location = XR_FORCE_FEEDBACK_CURL_LOCATION_FINGER_LITTLE_CURL_MNDX;

	result.locations = locations;

	xrApplyForceFeedbackCurlMNDX(force_feedback[hand].hand_tracker, &result);
}

void XRMNDXForceFeedbackExtensionWrapper::cleanup() {
}