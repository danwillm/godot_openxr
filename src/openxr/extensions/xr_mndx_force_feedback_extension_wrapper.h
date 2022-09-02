#ifndef XR_MNDX_FORCE_FEEDBACK_EXTENSION_WRAPPER_H
#define XR_MNDX_FORCE_FEEDBACK_EXTENSION_WRAPPER_H

#include "openxr/OpenXRApi.h"
#include "openxr/include/openxr_inc.h"
#include "xr_extension_wrapper.h"

#define MAX_FORCE_FEEDBACK_DEVICES 2

class ForceFeedback {
public:
	bool is_initialised = false;

	XrHandTrackerEXT handTracker = XR_NULL_HANDLE;
};

class XRMNDXForceFeedbackExtensionWrapper : public XRExtensionWrapper {
public:
	static XRMNDXForceFeedbackExtensionWrapper *get_singleton();

	void on_instance_initialized(const XrInstance instance) override;

	void on_state_ready() override;

	void set_force_feedback(XrApplyForceFeedbackCurlLocationsMNDX* locations);

protected:
	XRMNDXForceFeedbackExtensionWrapper();
	~XRMNDXForceFeedbackExtensionWrapper() override;

private:
	static XRAPI_ATTR XrResult XRAPI_CALL xrApplyForceFeedbackCurlMNDX(
			XrHandTrackerEXT handTracker,
			const XrApplyForceFeedbackCurlLocationsMNDX *locations);

	static XrResult initialise_mndx_force_feedback_extension(XrInstance instance);

	bool initialise_force_feedback();

	void cleanup();

	static XRMNDXForceFeedbackExtensionWrapper *singleton;
	OpenXRApi *openxr_api = nullptr;

	bool force_feedback_ext = false;

	ForceFeedback force_feedback[MAX_FORCE_FEEDBACK_DEVICES];
};

#endif