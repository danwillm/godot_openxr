#include "xr_mndx_force_feedback_extension_wrapper.h"

#define XR_SUCCESS_OR_RETURN(func) \
	XrResult result = func;        \
	if (result != XR_SUCCESS) {    \
		return result;             \
	}

XRMNDXForceFeedbackExtensionWrapper::XRMNDXForceFeedbackExtensionWrapper() {
	openxr_api = OpenXRApi::openxr_get_api();

	request_extensions[XR_MNDX_FORCE_FEEDBACK_CURL_EXTENSION_NAME] = &force_feedback_ext;
}

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

XrResult XRMNDXForceFeedbackExtensionWrapper::initialise_mndx_force_feedback_extension(XrInstance instance) {
	XR_SUCCESS_OR_RETURN(xrGetInstanceProcAddr(instance, "xrApplyForceFeedbackCurlMNDX", (PFN_xrVoidFunction *)&xrApplyForceFeedbackCurlMNDX));

	return XR_SUCCESS;
}

bool XRMNDXForceFeedbackExtensionWrapper::initialise_force_feedback() {
	XrSystemForceFeedbackPropertiesMNDX forceFeedbackSystemProperties = {
		.type = XR_TYPE_SYSTEM_FORCE_FEEDBACK_PROPERTIES_MNDX
	};

	XrSystemProperties systemProperties = {
		.type = XR_TYPE_SYSTEM_PROPERTIES,
		.next = &forceFeedbackSystemProperties
	};

	XrResult result = xrGetSystemProperties(openxr_api->get_instance(), openxr_api->get_system_id(), &systemProperties);

	return result == XR_SUCCESS;
}

void XRMNDXForceFeedbackExtensionWrapper::set_force_feedback(XrApplyForceFeedbackCurlLocationMNDX *locations, uint64_t location_count) {
}

PFN_xrApplyForceFeedbackCurlMNDX xrApplyForceFeedbackCurlMNDX_ptr = nullptr;

XRAPI_ATTR XrResult XRAPI_CALL XRMNDXForceFeedbackExtensionWrapper::xrApplyForceFeedbackCurlMNDX(XrHandTrackerEXT handTracker, const XrApplyForceFeedbackCurlLocationMNDX *locations, uint64_t locationCount) {
	if (xrApplyForceFeedbackCurlMNDX_ptr == nullptr) {
		return XR_ERROR_HANDLE_INVALID;
	}

	return (*xrApplyForceFeedbackCurlMNDX_ptr)(handTracker, locations, locationCount);
}
