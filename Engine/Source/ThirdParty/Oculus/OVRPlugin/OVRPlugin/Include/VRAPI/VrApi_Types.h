/************************************************************************************

Filename    :   VrApi_Types.h
Content     :   Types for minimum necessary API for mobile VR
Created     :   April 30, 2015
Authors     :   J.M.P. van Waveren
Language    :   C99

Copyright   :   Copyright 2015 Oculus VR, LLC. All Rights reserved.

*************************************************************************************/
#ifndef OVR_VrApi_Types_h
#define OVR_VrApi_Types_h

#include <stdbool.h>
#include <stdint.h>
#include "VrApi_Config.h"   // needed for VRAPI_EXPORT

//-----------------------------------------------------------------
// Java
//-----------------------------------------------------------------

#if defined( ANDROID )
#include <jni.h>
#elif defined( __cplusplus )
typedef struct _JNIEnv JNIEnv;
typedef struct _JavaVM JavaVM;
typedef class _jobject * jobject;
#else
typedef const struct JNINativeInterface * JNIEnv;
typedef const struct JNIInvokeInterface * JavaVM;
typedef void * jobject;
#endif

/// Java details about an activity
typedef struct
{
	JavaVM *	Vm;					//< Java Virtual Machine
	JNIEnv *	Env;				//< Thread specific environment
	jobject		ActivityObject;		//< Java activity object
} ovrJava;

OVR_VRAPI_ASSERT_TYPE_SIZE_32_BIT( ovrJava, 12 );
OVR_VRAPI_ASSERT_TYPE_SIZE_64_BIT( ovrJava, 24 );

//-----------------------------------------------------------------
// Basic Types
//-----------------------------------------------------------------

typedef signed int ovrResult;

/// ovrResult isn't actually an enum type and the the success / failure types are not
/// defined anywhere for GearVR VrApi. This needs to be remedied. For now, I'm defining
/// these here and will try to address this larger issue in a follow-on changeset.
/// errors are < 0, successes are >= 0
/// Except where noted, these match error codes from PC CAPI.
typedef enum ovrSuccessResult_
{
	ovrSuccess						= 0,
} ovrSuccessResult;

typedef enum ovrErrorResult_
{
	ovrError_MemoryAllocationFailure	= -1000,
	ovrError_NotInitialized				= -1004,
	ovrError_InvalidParameter			= -1005,
	ovrError_DeviceUnavailable			= -1010,	//< device is not connected, or not connected as input device
	ovrError_InvalidOperation			= -1015,

	// enums not in CAPI
	ovrError_UnsupportedDeviceType		= -1050,	//< specified device type isn't supported on GearVR
	ovrError_NoDevice					= -1051,	//< specified device ID does not map to any current device
	ovrError_NotImplemented				= -1052,	//< executed an incomplete code path - this should not be possible in public releases.

	ovrResult_EnumSize 					= 0x7fffffff
} ovrErrorResult;

/// A 2D vector.
typedef struct ovrVector2f_
{
	float x, y;
} ovrVector2f;

OVR_VRAPI_ASSERT_TYPE_SIZE( ovrVector2f, 8 );

/// A 3D vector.
typedef struct ovrVector3f_
{
	float x, y, z;
} ovrVector3f;

OVR_VRAPI_ASSERT_TYPE_SIZE( ovrVector3f, 12 );

/// A 4D vector.
typedef struct ovrVector4f_
{
	float x, y, z, w;
} ovrVector4f;

OVR_VRAPI_ASSERT_TYPE_SIZE( ovrVector4f, 16 );

/// Quaternion.
typedef struct ovrQuatf_
{
	float x, y, z, w;
} ovrQuatf;

OVR_VRAPI_ASSERT_TYPE_SIZE( ovrQuatf, 16 );

/// Row-major 4x4 matrix.
typedef struct ovrMatrix4f_
{
	float M[4][4];
} ovrMatrix4f;

OVR_VRAPI_ASSERT_TYPE_SIZE( ovrMatrix4f, 64 );

/// Position and orientation together.
typedef struct ovrPosef_
{
	ovrQuatf	Orientation;
	ovrVector3f	Position;
} ovrPosef;

OVR_VRAPI_ASSERT_TYPE_SIZE( ovrPosef, 28 );

/// A rectangle with 2D size and position.
typedef struct ovrRectf_
{
	float x;
	float y;
	float width;
	float height;
} ovrRectf;

OVR_VRAPI_ASSERT_TYPE_SIZE( ovrRectf, 16 );

/// True or false.
typedef enum
{
	VRAPI_FALSE = 0,
	VRAPI_TRUE	= 1
} ovrBooleanResult;

/// One of the user's eyes.
typedef enum
{
	VRAPI_EYE_LEFT	= 0,
	VRAPI_EYE_RIGHT	= 1,
	VRAPI_EYE_COUNT	= 2
} ovrEye;

//-----------------------------------------------------------------
// Structure Types
//-----------------------------------------------------------------

/// Defines a layout for ovrInitParms, ovrModeParms, or ovrFrameParms.
typedef enum
{
	VRAPI_STRUCTURE_TYPE_INIT_PARMS			= 1,
	VRAPI_STRUCTURE_TYPE_MODE_PARMS			= 2,
	VRAPI_STRUCTURE_TYPE_FRAME_PARMS		= 3,
///--BEGIN_SDK_REMOVE
	VRAPI_STRUCTURE_TYPE_INIT_PARMS_EXT		= 4,
	VRAPI_STRUCTURE_TYPE_MODE_PARMS_VULKAN	= 5,
///--END_SDK_REMOVE
} ovrStructureType;

//-----------------------------------------------------------------
// System Properties and Status
//-----------------------------------------------------------------

/// A VR-capable device.
typedef enum
{
	VRAPI_DEVICE_TYPE_GEARVR_START			= 0,

	VRAPI_DEVICE_TYPE_NOTE4					= VRAPI_DEVICE_TYPE_GEARVR_START,
	VRAPI_DEVICE_TYPE_NOTE5					= 1,
	VRAPI_DEVICE_TYPE_S6					= 2,
	VRAPI_DEVICE_TYPE_S7					= 3,
	VRAPI_DEVICE_TYPE_NOTE7					= 4,			//< No longer supported.
	VRAPI_DEVICE_TYPE_S8					= 5,
	VRAPI_DEVICE_TYPE_NOTE8					= 6,
	VRAPI_DEVICE_TYPE_NOTE7_FE				= 7,			//< Fan Edition
	VRAPI_DEVICE_TYPE_A5_2018				= 8,			//< A8
	VRAPI_DEVICE_TYPE_A7_2018				= 9,			//< A8 Plus

///--BEGIN_SDK_REMOVE
	VRAPI_DEVICE_TYPE_S9					= 10,
	VRAPI_DEVICE_TYPE_S9_PLUS				= 11,
	VRAPI_DEVICE_TYPE_GEARVR_PLUS			= 32,
///--END_SDK_REMOVE
	VRAPI_DEVICE_TYPE_GEARVR_END			= 63,

	// Standalone Devices
	VRAPI_DEVICE_TYPE_OCULUSGO_START		= 64,
	VRAPI_DEVICE_TYPE_OCULUSGO				= VRAPI_DEVICE_TYPE_OCULUSGO_START,
	VRAPI_DEVICE_TYPE_MIVR_STANDALONE		= VRAPI_DEVICE_TYPE_OCULUSGO_START + 1,	//< China-only SKU
	VRAPI_DEVICE_TYPE_OCULUSGO_END			= 127,

///--BEGIN_SDK_REMOVE
	VRAPI_DEVICE_TYPE_MONTEREY_START		= 256,
	VRAPI_DEVICE_TYPE_DAWN_PROTO0			= VRAPI_DEVICE_TYPE_MONTEREY_START,
	VRAPI_DEVICE_TYPE_MONTEREY_PROTO1_SDC,
	VRAPI_DEVICE_TYPE_MONTEREY_PROTO1_AUO,
	VRAPI_DEVICE_TYPE_MONTEREY,
	VRAPI_DEVICE_TYPE_MONTEREY_END			= 319,
///--END_SDK_REMOVE

	VRAPI_DEVICE_TYPE_UNKNOWN				= -1,
} ovrDeviceType;

/// A headset, which typically includes optics and tracking hardware, but not necessarily the device itself.
typedef enum
{
	VRAPI_HEADSET_TYPE_R320					= 0,			//< Note4 Innovator
	VRAPI_HEADSET_TYPE_R321					= 1,			//< S6 Innovator
	VRAPI_HEADSET_TYPE_R322					= 2,			//< Commercial 1
	VRAPI_HEADSET_TYPE_R323					= 3,			//< Commercial 2 (USB Type C)
	VRAPI_HEADSET_TYPE_R324					= 4,			//< Commercial 3 (USB Type C)
	VRAPI_HEADSET_TYPE_R325					= 5,			//< Commercial 4 2017 (USB Type C)

	// Standalone Headsets
	VRAPI_HEADSET_TYPE_OCULUSGO				= 64,			//< Oculus Go
	VRAPI_HEADSET_TYPE_MIVR_STANDALONE		= 65,			//< China-only SKU

///--BEGIN_SDK_REMOVE
	// Unannounced devices.

	// Prototype headsets.
	VRAPI_HEADSET_TYPE_MONTEREY_PROTO1		= 256,
///--END_SDK_REMOVE

	VRAPI_HEADSET_TYPE_UNKNOWN				= -1,
} ovrHeadsetType;

/// A geographic region authorized for certain hardware and content.
typedef enum
{
	VRAPI_DEVICE_REGION_UNSPECIFIED	= 0,
	VRAPI_DEVICE_REGION_JAPAN		= 1,
	VRAPI_DEVICE_REGION_CHINA		= 2,
} ovrDeviceRegion;

/// The maximum resolution and framerate supported by a video decoder.
typedef enum
{
	VRAPI_VIDEO_DECODER_LIMIT_4K_30FPS	= 0,
	VRAPI_VIDEO_DECODER_LIMIT_4K_60FPS	= 1,
} ovrVideoDecoderLimit;

/// System configuration properties.
typedef enum
{
	VRAPI_SYS_PROP_DEVICE_TYPE								= 0,
	VRAPI_SYS_PROP_MAX_FULLSPEED_FRAMEBUFFER_SAMPLES		= 1,
	/// Physical width and height of the display in pixels.
	VRAPI_SYS_PROP_DISPLAY_PIXELS_WIDE						= 2,
	VRAPI_SYS_PROP_DISPLAY_PIXELS_HIGH						= 3,
	/// Returns the refresh rate of the display in cycles per second.
	VRAPI_SYS_PROP_DISPLAY_REFRESH_RATE						= 4,
	/// With a display resolution of 2560x1440, the pixels at the center
	/// of each eye cover about 0.06 degrees of visual arc. To wrap a
	/// full 360 degrees, about 6000 pixels would be needed and about one
	/// quarter of that would be needed for ~90 degrees FOV. As such, Eye
	/// images with a resolution of 1536x1536 result in a good 1:1 mapping
	/// in the center, but they need mip-maps for off center pixels. To
	/// avoid the need for mip-maps and for significantly improved rendering
	/// performance this currently returns a conservative 1024x1024.
	VRAPI_SYS_PROP_SUGGESTED_EYE_TEXTURE_WIDTH				= 5,
	VRAPI_SYS_PROP_SUGGESTED_EYE_TEXTURE_HEIGHT				= 6,
	/// This is a product of the lens distortion and the screen size,
	/// but there is no truly correct answer.
	/// There is a tradeoff in resolution and coverage.
	/// Too small of an FOV will leave unrendered pixels visible, but too
	/// large wastes resolution or fill rate.  It is unreasonable to
	/// increase it until the corners are completely covered, but we do
	/// want most of the outside edges completely covered.
	/// Applications might choose to render a larger FOV when angular
	/// acceleration is high to reduce black pull in at the edges by
	/// the time warp.
	/// Currently symmetric 90.0 degrees.
	VRAPI_SYS_PROP_SUGGESTED_EYE_FOV_DEGREES_X				= 7,
	VRAPI_SYS_PROP_SUGGESTED_EYE_FOV_DEGREES_Y				= 8,
	/// Path to the external SD card. On Android-M, this path is dynamic and can
	/// only be determined once the SD card is mounted. Returns an empty string if
	/// device does not support an ext sdcard or if running Android-M and the SD card
	/// is not mounted.
	VRAPI_SYS_PROP_EXT_SDCARD_PATH							= 9,
	VRAPI_SYS_PROP_DEVICE_REGION							= 10,
	/// Video decoder limit for the device.
	VRAPI_SYS_PROP_VIDEO_DECODER_LIMIT						= 11,
	VRAPI_SYS_PROP_HEADSET_TYPE								= 12,

	/// A single press and release of the back button in less than this time is considered
	/// a 'short press'.
	/// Specified in seconds.
	VRAPI_SYS_PROP_BACK_BUTTON_SHORTPRESS_TIME				= 13,
	
	// enum 14 used to be VRAPI_SYS_PROP_BACK_BUTTON_DOUBLETAP_TIME

	/// Returns an ovrHandedness enum indicating left or right hand.
	VRAPI_SYS_PROP_DOMINANT_HAND							= 15,

	/// Returns the number of display refresh rates supported by the system.
	VRAPI_SYS_PROP_NUM_SUPPORTED_DISPLAY_REFRESH_RATES		= 64,
	/// Returns an array of the supported display refresh rates.
	VRAPI_SYS_PROP_SUPPORTED_DISPLAY_REFRESH_RATES			= 65,

	/// Returns VRAPI_TRUE if Multiview rendering support is available for this system,
	/// otherwise VRAPI_FALSE.
	VRAPI_SYS_PROP_MULTIVIEW_AVAILABLE						= 128,
	/// Returns VRAPI_TRUE if submission of SRGB Layers is supported for this system,
	/// otherwise VRAPI_FALSE.
	VRAPI_SYS_PROP_SRGB_LAYER_SOURCE_AVAILABLE				= 129,

	/// Returns VRAPI_TRUE if on-chip foveated rendering of swapchains is supported
	/// for this system, otherwise VRAPI_FALSE.
	VRAPI_SYS_PROP_FOVEATION_AVAILABLE						= 130,
///--BEGIN_SDK_REMOVE
	/// Returns VRAPI_TRUE if gpu utilization functionality is supported for this system,
	/// otherwise VRAPI_FALSE.
	VRAPI_SYS_PROP_GPU_UTIL_AVAILABLE						= 131,
	/// Returns VRAPI_TRUE if cpu utilization functionality is supported for this system,
	/// otherwise VRAPI_FALSE.
	VRAPI_SYS_PROP_CPU_UTIL_AVAILABLE						= 132,
///--END_SDK_REMOVE
} ovrSystemProperty;

/// Configurable VrApi properties.
typedef enum
{
///--BEGIN_SDK_REMOVE
	// enum 0-3 have been deprecated.

	VRAPI_TUTORIAL_COMPLETED			= 4,		//< Used by Oculus Home application for marking Tutorial Completed for China flow.
	VRAPI_INHIBIT_HANDLE_LONGPRESS		= 5,		//< Used by First Party Native Social apps which need to disable Long Press to UM handling.
	VRAPI_DO_NOT_DISTURB_MODE			= 6,		//< Used by System Activities application for setting the Do-Not-Disturb Mode.
	VRAPI_SYSTEM_BRIGHTNESS				= 7,		//< Used by System Activities application for setting the System Brightness.
	VRAPI_REMOTE_HANDEDNESS				= 8,		//< Used by System Activities application for setting the Remote Handedness.
	VRAPI_INHIBIT_HANDLE_HOMEKEY		= 9,		//< Used by First Party Apps which need to disable Home Button handling.
	VRAPI_INHIBIT_CONFIRM_QUIT_MENU		= 10,		//< Used by specific First Party Apps which should go straight to Home instead of Confirm-Quit.
	VRAPI_INHIBIT_SCREEN_CAPTURE		= 11,		//< Used by System Activities to decide whether to allow screen capture or not

	// enum 12 used to be VRAPI_RECENTER_REMOTE_POSE.

	VRAPI_SHELL_SYSTEM_UTILS			= 13,		//< Used by System Activities to query the cached value of Shell System Utils GK ( This enum can be removed once sysUtils is 100% )
	VRAPI_TRANSCRIBE_AVAILABLE			= 14,		//< Used by System Activities to query the cached value of Transcribe enabled GK ( This enum can be removed once transcribe is 100% )
///--END_SDK_REMOVE
	VRAPI_FOVEATION_LEVEL				= 15,		//< Used by apps that want to control swapchain foveation levels
///--BEGIN_SDK_REMOVE
	VRAPI_ANYTIMEUI						= 16,		//< Used by System Acitivites to query if the anytime UI is valid
///--END_SDK_REMOVE
} ovrProperty;

///--BEGIN_SDK_REMOVE
/// Whether or not screen capture is enabled.
typedef enum
{
	VRAPI_SCREEN_CAPTURE_UNKNOWN		= 0,
	VRAPI_SCREEN_CAPTURE_DISABLED		= 1,
	VRAPI_SCREEN_CAPTURE_ENABLED		= 2
} ovrScreenCapture;
///--END_SDK_REMOVE

/// Specifies left or right handedness.
typedef enum
{
	VRAPI_HAND_UNKNOWN	= 0,
	VRAPI_HAND_LEFT		= 1,
	VRAPI_HAND_RIGHT	= 2
} ovrHandedness;

/// System status bits.
typedef enum
{
	VRAPI_SYS_STATUS_DOCKED							= 0,	//< Device is docked.
	VRAPI_SYS_STATUS_MOUNTED						= 1,	//< Device is mounted.
	VRAPI_SYS_STATUS_THROTTLED						= 2,	//< Device is in powersave mode.

	// enum  3 used to be VRAPI_SYS_STATUS_THROTTLED2.

	// enum  4 used to be VRAPI_SYS_STATUS_THROTTLED_WARNING_LEVEL.

	VRAPI_SYS_STATUS_RENDER_LATENCY_MILLISECONDS	= 5,	//< Average time between render tracking sample and scanout.
	VRAPI_SYS_STATUS_TIMEWARP_LATENCY_MILLISECONDS	= 6,	//< Average time between timewarp tracking sample and scanout.
	VRAPI_SYS_STATUS_SCANOUT_LATENCY_MILLISECONDS	= 7,	//< Average time between Vsync and scanout.
	VRAPI_SYS_STATUS_APP_FRAMES_PER_SECOND			= 8,	//< Number of frames per second delivered through vrapi_SubmitFrame.
	VRAPI_SYS_STATUS_SCREEN_TEARS_PER_SECOND		= 9,	//< Number of screen tears per second (per eye).
	VRAPI_SYS_STATUS_EARLY_FRAMES_PER_SECOND		= 10,	//< Number of frames per second delivered a whole display refresh early.
	VRAPI_SYS_STATUS_STALE_FRAMES_PER_SECOND		= 11,	//< Number of frames per second delivered late.

	// enum 12 used to be VRAPI_SYS_STATUS_HEADPHONES_PLUGGED_IN

	VRAPI_SYS_STATUS_RECENTER_COUNT					= 13,	//< Returns the current HMD recenter count. Defaults to 0.
	VRAPI_SYS_STATUS_SYSTEM_UX_ACTIVE				= 14,	//< Returns VRAPI_TRUE if a system UX layer is active

	VRAPI_SYS_STATUS_FRONT_BUFFER_PROTECTED			= 128,	//< VRAPI_TRUE if the front buffer is allocated in TrustZone memory.
	VRAPI_SYS_STATUS_FRONT_BUFFER_565				= 129,	//< VRAPI_TRUE if the front buffer is 16-bit 5:6:5
	VRAPI_SYS_STATUS_FRONT_BUFFER_SRGB				= 130,	//< VRAPI_TRUE if the front buffer uses the sRGB color space.

///--BEGIN_SDK_REMOVE
	VRAPI_SYS_STATUS_SHOULD_QUIT					= 256,	//< VRAPI_TRUE if the app should terminate. (VrApi->CAPI Shim only)
	VRAPI_SYS_STATUS_HAS_FOCUS						= 257,	//< VRAPI_TRUE if the app has focus and is therefore visible on the HMD. (VrApi->CAPI Shim only)
	VRAPI_SYS_STATUS_HDCP_FAILURE					= 258,	//< VRAPI_TRUE if the HDCP link is broken. (VrApi->CAPI Shim only)

	VRAPI_SYS_STATUS_DO_NOT_DISTURB_MODE			= 259,	//< Returns VRAPI_TRUE if DND Mode is active. (SA only)
	VRAPI_SYS_STATUS_SYSTEM_BRIGHTNESS				= 260,	//< Returns the current System Brightness. (SA only)

	VRAPI_SYS_STATUS_TOTAL_STALE_FRAMES				= 261,	//< Returns the total stale frames starting from when VR mode is entered
	VRAPI_SYS_STATUS_GPU_UTIL						= 262,	//< Returns the current GPU Utilization, from 0.0 to 1.0
	VRAPI_SYS_STATUS_CPU_UTIL						= 263,	//< Returns the current CPU Utilization, from 0.0 to 1.0
	VRAPI_SYS_STATUS_CPU_UTIL_AVG					= 264,	//< Returns the current CPU Utilization averaged across all cores, from 0.0 to 1.0
///--END_SDK_REMOVE
} ovrSystemStatus;

//-----------------------------------------------------------------
// Initialization
//-----------------------------------------------------------------

/// Possible results of initialization.
typedef enum
{
	VRAPI_INITIALIZE_SUCCESS			=  0,
	VRAPI_INITIALIZE_UNKNOWN_ERROR		= -1,
	VRAPI_INITIALIZE_PERMISSIONS_ERROR	= -2,
} ovrInitializeStatus;

/// Supported graphics APIs.
typedef enum
{
	VRAPI_GRAPHICS_API_TYPE_ES		 = 0x10000,
	VRAPI_GRAPHICS_API_OPENGL_ES_2   = ( VRAPI_GRAPHICS_API_TYPE_ES | 0x0200 ),		//< OpenGL ES 2.x context
	VRAPI_GRAPHICS_API_OPENGL_ES_3   = ( VRAPI_GRAPHICS_API_TYPE_ES | 0x0300 ),		//< OpenGL ES 3.x context

	VRAPI_GRAPHICS_API_TYPE_OPENGL	 = 0x20000,
	VRAPI_GRAPHICS_API_OPENGL_COMPAT = ( VRAPI_GRAPHICS_API_TYPE_OPENGL | 0x0100 ), //< OpenGL Compatibility Profile
	VRAPI_GRAPHICS_API_OPENGL_CORE_3 = ( VRAPI_GRAPHICS_API_TYPE_OPENGL | 0x0300 ), //< OpenGL Core Profile 3.x
	VRAPI_GRAPHICS_API_OPENGL_CORE_4 = ( VRAPI_GRAPHICS_API_TYPE_OPENGL | 0x0400 ), //< OpenGL Core Profile 4.x

///--BEGIN_SDK_REMOVE
	VRAPI_GRAPHICS_API_TYPE_VULKAN	 = 0x40000,
	VRAPI_GRAPHICS_API_VULKAN_1		 = ( VRAPI_GRAPHICS_API_TYPE_VULKAN | 0x0100 ), //< Vulkan 1.x
///--END_SDK_REMOVE
} ovrGraphicsAPI;

/// Configuration details specified at startup.
typedef struct
{
	ovrStructureType	Type;
	int					ProductVersion;
	int					MajorVersion;
	int					MinorVersion;
	int					PatchVersion;
	ovrGraphicsAPI		GraphicsAPI;
	ovrJava				Java;
} ovrInitParms;

OVR_VRAPI_ASSERT_TYPE_SIZE_32_BIT( ovrInitParms, 36 );
OVR_VRAPI_ASSERT_TYPE_SIZE_64_BIT( ovrInitParms, 48 );

//-----------------------------------------------------------------
// VR Mode
//-----------------------------------------------------------------

/// \note the first two flags use the first two bytes for backwards compatibility on little endian systems.
typedef enum
{
	/// If set, warn and allow the app to continue at 30 FPS when throttling occurs.
	/// If not set, display the level 2 error message which requires the user to undock.
	VRAPI_MODE_FLAG_ALLOW_POWER_SAVE			= 0x000000FF,

	/// When an application moves backwards on the activity stack,
	/// the activity window it returns to is no longer flagged as fullscreen.
	/// As a result, Android will also render the decor view, which wastes a
	/// significant amount of bandwidth.
	/// By setting this flag, the fullscreen flag is reset on the window.
	/// Unfortunately, this causes Android life cycle events that mess up
	/// several NativeActivity codebases like Stratum and UE4, so this
	/// flag should only be set for specific applications.
	/// Use "adb shell dumpsys SurfaceFlinger" to verify
	/// that there is only one HWC next to the FB_TARGET.
	VRAPI_MODE_FLAG_RESET_WINDOW_FULLSCREEN		= 0x0000FF00,

	/// The WindowSurface passed in is an ANativeWindow.
	VRAPI_MODE_FLAG_NATIVE_WINDOW				= 0x00010000,

	/// Create the front buffer in TrustZone memory to allow protected DRM
	/// content to be rendered to the front buffer. This functionality
	/// requires the WindowSurface to be allocated from TimeWarp, via
	/// specifying the nativeWindow via VRAPI_MODE_FLAG_NATIVE_WINDOW.
	VRAPI_MODE_FLAG_FRONT_BUFFER_PROTECTED		= 0x00020000,

	/// Create a 16-bit 5:6:5 front buffer.
	VRAPI_MODE_FLAG_FRONT_BUFFER_565			= 0x00040000,

	/// Create a front buffer using the sRGB color space.
	VRAPI_MODE_FLAG_FRONT_BUFFER_SRGB			= 0x00080000,

	/// If set, indicates the OpenGL ES Context was created with EGL_CONTEXT_OPENGL_NO_ERROR_KHR attribute.
	/// The same attribute would be applied when TimeWrap creates the shared context.
	/// More information could be found at:
	/// https://www.khronos.org/registry/EGL/extensions/KHR/EGL_KHR_create_context_no_error.txt
	VRAPI_MODE_FLAG_CREATE_CONTEXT_NO_ERROR		= 0x00100000
} ovrModeFlags;

/// Configuration details that stay constant between a vrapi_EnterVrMode()/vrapi_LeaveVrMode() pair.
typedef struct
{
	ovrStructureType	Type;

	/// Combination of ovrModeFlags flags.
	unsigned int		Flags;

	/// The Java VM is needed for the time warp thread to create a Java environment.
	/// A Java environment is needed to access various system services. The thread
	/// that enters VR mode is responsible for attaching and detaching the Java
	/// environment. The Java Activity object is needed to get the windowManager,
	/// packageName, systemService, etc.
	ovrJava				Java;

	OVR_VRAPI_PADDING_32_BIT( 4 );

	/// Display to use for asynchronous time warp rendering.
	/// Using EGL this is an EGLDisplay.
	unsigned long long	Display;

	/// The window surface to use for asynchronous time warp rendering.
	/// Using EGL this can be the EGLSurface created by the application for the ANativeWindow.
	/// This should be the ANativeWIndow itself (requires VRAPI_MODE_FLAG_NATIVE_WINDOW).
	unsigned long long	WindowSurface;

	/// The resources from this context will be shared with the asynchronous time warp.
	/// Using EGL this is an EGLContext.
	unsigned long long	ShareContext;
} ovrModeParms;

OVR_VRAPI_ASSERT_TYPE_SIZE_32_BIT( ovrModeParms, 48 );
OVR_VRAPI_ASSERT_TYPE_SIZE_64_BIT( ovrModeParms, 56 );

///--BEGIN_SDK_REMOVE
/// Vulkan-specific mode paramaters.
typedef struct
{
	ovrModeParms		ModeParms;

	/// For Vulkan, this should be the VkQueue created on the same Device as specified
	/// by vrapi_CreateSystemVulkan. An internally created VkFence object will be signaled
	/// by the completion of commands on the queue.
	unsigned long long	SynchronizationQueue;
} ovrModeParmsVulkan;

OVR_VRAPI_ASSERT_TYPE_SIZE_32_BIT( ovrModeParmsVulkan, 56 );
OVR_VRAPI_ASSERT_TYPE_SIZE_64_BIT( ovrModeParmsVulkan, 64 );
///--END_SDK_REMOVE

/// VR context
/// To allow multiple Android activities that live in the same address space
/// to cooperatively use the VrApi, each activity needs to maintain its own
/// separate contexts for a lot of the video related systems.
typedef struct ovrMobile ovrMobile;

//-----------------------------------------------------------------
// Tracking
//-----------------------------------------------------------------

/// Full rigid body pose with first and second derivatives.
typedef struct ovrRigidBodyPosef_
{
	ovrPosef		Pose;
	ovrVector3f		AngularVelocity;
	ovrVector3f		LinearVelocity;
	ovrVector3f		AngularAcceleration;
	ovrVector3f		LinearAcceleration;
	OVR_VRAPI_PADDING( 4 );
	double			TimeInSeconds;			//< Absolute time of this pose.
	double			PredictionInSeconds;	//< Seconds this pose was predicted ahead.
} ovrRigidBodyPosef;

OVR_VRAPI_ASSERT_TYPE_SIZE( ovrRigidBodyPosef, 96 );

/// Bit flags describing the current status of sensor tracking.
typedef enum
{
	VRAPI_TRACKING_STATUS_ORIENTATION_TRACKED	= 0x0001,	//< Orientation is currently tracked.
	VRAPI_TRACKING_STATUS_POSITION_TRACKED		= 0x0002,	//< Position is currently tracked.
	VRAPI_TRACKING_STATUS_HMD_CONNECTED			= 0x0080	//< HMD is available & connected.
} ovrTrackingStatus;

/// Tracking state at a given absolute time.
typedef struct ovrTracking2_
{
	/// Sensor status described by ovrTrackingStatus flags.
	unsigned int		Status;

	OVR_VRAPI_PADDING( 4 );

	/// Predicted head configuration at the requested absolute time.
	/// The pose describes the head orientation and center eye position.
	ovrRigidBodyPosef	HeadPose;
	struct
	{
		ovrMatrix4f			ProjectionMatrix;
		ovrMatrix4f			ViewMatrix;
	} Eye[ VRAPI_EYE_COUNT ];
} ovrTracking2;

OVR_VRAPI_ASSERT_TYPE_SIZE( ovrTracking2, 360 );

/// Reports the status and pose of a motion tracker.
typedef struct ovrTracking_
{
	/// Sensor status described by ovrTrackingStatus flags.
	unsigned int		Status;

	OVR_VRAPI_PADDING( 4 );

	/// Predicted head configuration at the requested absolute time.
	/// The pose describes the head orientation and center eye position.
	ovrRigidBodyPosef	HeadPose;
} ovrTracking;

OVR_VRAPI_ASSERT_TYPE_SIZE( ovrTracking, 104 );

/// Specifies a reference frame for motion tracking data.
typedef enum
{
	VRAPI_TRACKING_TRANSFORM_IDENTITY					= 0,
	VRAPI_TRACKING_TRANSFORM_CURRENT					= 1,
	VRAPI_TRACKING_TRANSFORM_SYSTEM_CENTER_EYE_LEVEL	= 2,
	VRAPI_TRACKING_TRANSFORM_SYSTEM_CENTER_FLOOR_LEVEL	= 3,
} ovrTrackingTransform;

//-----------------------------------------------------------------
// Texture Swap Chain
//-----------------------------------------------------------------

/// A texture type, such as 2D, array, or cubemap.
typedef enum
{
	VRAPI_TEXTURE_TYPE_2D			= 0,	//< 2D textures.
	VRAPI_TEXTURE_TYPE_2D_EXTERNAL	= 1,	//< External 2D texture.
	VRAPI_TEXTURE_TYPE_2D_ARRAY		= 2,	//< Texture array.
	VRAPI_TEXTURE_TYPE_CUBE			= 3,	//< Cube maps.
	VRAPI_TEXTURE_TYPE_MAX			= 4,
///--BEGIN_SDK_REMOVE
	/// Needed for UE4 multi-res push until vrapi_refactor ships.
	VRAPI_TEXTURE_TYPE_2D_ARRAY_FOVEATED = 128,		//< 4-deep texture array.
///--END_SDK_REMOVE
} ovrTextureType;

/// A texture format.
typedef enum
{
	VRAPI_TEXTURE_FORMAT_NONE				= 0,
	VRAPI_TEXTURE_FORMAT_565				= 1,
	VRAPI_TEXTURE_FORMAT_5551				= 2,
	VRAPI_TEXTURE_FORMAT_4444				= 3,
	VRAPI_TEXTURE_FORMAT_8888				= 4,
	VRAPI_TEXTURE_FORMAT_8888_sRGB			= 5,
	VRAPI_TEXTURE_FORMAT_RGBA16F			= 6,
	VRAPI_TEXTURE_FORMAT_DEPTH_16			= 7,
	VRAPI_TEXTURE_FORMAT_DEPTH_24			= 8,
	VRAPI_TEXTURE_FORMAT_DEPTH_24_STENCIL_8	= 9,

///--BEGIN_SDK_REMOVE
	VRAPI_TEXTURE_FORMAT_8888_HDCP			= 128,	//< VrApi->CAPI Shim only
	VRAPI_TEXTURE_FORMAT_8888_sRGB_HDCP		= 129,	//< VrApi->CAPI Shim only
///--END_SDK_REMOVE
} ovrTextureFormat;

/// Built-in convenience swapchains.
typedef enum
{
	VRAPI_DEFAULT_TEXTURE_SWAPCHAIN					= 0x1,
	VRAPI_DEFAULT_TEXTURE_SWAPCHAIN_LOADING_ICON	= 0x2
} ovrDefaultTextureSwapChain;

typedef struct ovrTextureSwapChain ovrTextureSwapChain;

//-----------------------------------------------------------------
// Frame Submission
//-----------------------------------------------------------------

/// Per-frame configuration options.
typedef enum
{
	/// To get gamma correct sRGB filtering of the eye textures, the textures must be
	/// allocated with GL_SRGB8_ALPHA8 format and the window surface must be allocated
	/// with these attributes:
	/// EGL_GL_COLORSPACE_KHR,  EGL_GL_COLORSPACE_SRGB_KHR
	///
	/// While we can reallocate textures easily enough, we can't change the window
	/// colorspace without relaunching the entire application, so if you want to
	/// be able to toggle between gamma correct and incorrect, you must allocate
	/// the framebuffer as sRGB, then inhibit that processing when using normal
	/// textures.
	VRAPI_FRAME_FLAG_INHIBIT_SRGB_FRAMEBUFFER					= 1,
	/// Flush the warp swap pipeline so the images show up immediately.
	/// This is expensive and should only be used when an immediate transition
	/// is needed like displaying black when resetting the HMD orientation.
	VRAPI_FRAME_FLAG_FLUSH										= 2,
	/// This is the final frame. Do not accept any more frames after this.
	VRAPI_FRAME_FLAG_FINAL										= 4,

	/// enum  8 used to be VRAPI_FRAME_FLAG_TIMEWARP_DEBUG_GRAPH_SHOW.

	/// enum 16 used to be VRAPI_FRAME_FLAG_TIMEWARP_DEBUG_GRAPH_FREEZE.

	/// enum 32 used to be VRAPI_FRAME_FLAG_TIMEWARP_DEBUG_GRAPH_LATENCY_MODE.

	/// Don't show the volume layer whent set.
	VRAPI_FRAME_FLAG_INHIBIT_VOLUME_LAYER						= 64,

	/// enum 128 used to be VRAPI_FRAME_FLAG_SHOW_LAYER_COMPLEXITY.

	/// enum 256 used to be VRAPI_FRAME_FLAG_SHOW_TEXTURE_DENSITY.

///--BEGIN_SDK_REMOVE
	VRAPI_FRAME_FLAG_IGNORE_FENCES								= 512,
///--END_SDK_REMOVE

} ovrFrameFlags;

/// Per-frame configuration options that apply to a particular layer.
typedef enum
{
	/// \deprecated Enable writing to the alpha channel
	VRAPI_FRAME_LAYER_FLAG_WRITE_ALPHA								= 1,
	/// Correct for chromatic aberration. Quality/perf trade-off.
	VRAPI_FRAME_LAYER_FLAG_CHROMATIC_ABERRATION_CORRECTION			= 2,
	/// Used for some HUDs, but generally considered bad practice.
	VRAPI_FRAME_LAYER_FLAG_FIXED_TO_VIEW							= 4,
	/// Spin the layer - for loading icons
	VRAPI_FRAME_LAYER_FLAG_SPIN										= 8,
	/// Clip fragments outside the layer's TextureRect
	VRAPI_FRAME_LAYER_FLAG_CLIP_TO_TEXTURE_RECT						= 16,
///--BEGIN_SDK_REMOVE
	/// Needed for UE4 multi-res push until vrapi_refactor ships.
	VRAPI_FRAME_LAYER_FLAG_ARRAY_OFFSET_TWO							= 128,

	/// If the texture being read isn't an sRGB texture, the conversion
	/// on write must be inhibited or the colors are washed out.
	/// This is necessary for using external images on an sRGB framebuffer.
	VRAPI_FRAME_LAYER_FLAG_INHIBIT_SRGB_FRAMEBUFFER					= 256,

	/// The external image layers normally check for updates before each
	/// eye, but it may be useful to only check on one eye to provide a
	/// 24 fps update rate across "five eyes".
	VRAPI_FRAME_LAYER_FLAG_INHIBIT_LEFT_UPDATE						= 512,
	VRAPI_FRAME_LAYER_FLAG_INHIBIT_RIGHT_UPDATE						= 1024,

	/// If you set this flag, vrapi will use this layer to draw
	/// the pass-through camera feed.
	/// It is basically a dummy layer that allows fine control of when
	/// the pass-through camera will draw in relation to other layers.
	VRAPI_FRAME_LAYER_FLAG_PASS_THROUGH_CAMERA						= 2048,
///--END_SDK_REMOVE

} ovrFrameLayerFlags;

/// The user's eye (left or right) that can see a layer.
typedef enum
{
	VRAPI_FRAME_LAYER_EYE_LEFT		= 0,
	VRAPI_FRAME_LAYER_EYE_RIGHT		= 1,
	VRAPI_FRAME_LAYER_EYE_MAX		= 2
} ovrFrameLayerEye;

/// Selects an operation for alpha blending two images.
typedef enum
{
	VRAPI_FRAME_LAYER_BLEND_ZERO					= 0,
	VRAPI_FRAME_LAYER_BLEND_ONE						= 1,
	VRAPI_FRAME_LAYER_BLEND_SRC_ALPHA				= 2,
	/// \deprecated
	VRAPI_FRAME_LAYER_BLEND_DST_ALPHA				= 3,
	/// \deprecated
	VRAPI_FRAME_LAYER_BLEND_ONE_MINUS_DST_ALPHA		= 4,
	VRAPI_FRAME_LAYER_BLEND_ONE_MINUS_SRC_ALPHA		= 5
} ovrFrameLayerBlend;

/// \deprecated Explicit indices for frame layers should be used instead.
typedef enum
{
	VRAPI_FRAME_LAYER_TYPE_MAX	= 4
} ovrFrameLayerType;

/// Extra latency mode pipelines app CPU work a frame ahead of VR composition.
typedef enum
{
	VRAPI_EXTRA_LATENCY_MODE_OFF		= 0,
	VRAPI_EXTRA_LATENCY_MODE_ON			= 1,
	VRAPI_EXTRA_LATENCY_MODE_DYNAMIC	= 2
} ovrExtraLatencyMode;

//-------------------------------------
// Legacy monolithic FrameParm submission structures.
//-------------------------------------

/// A compositor layer.
/// \note Any layer textures that are dynamic must be triple buffered.
typedef struct
{
	/// Because OpenGL ES does not support clampToBorder, it is the
	/// application's responsibility to make sure that all mip levels
	/// of the primary eye texture have a black border that will show
	/// up when time warp pushes the texture partially off screen.
	ovrTextureSwapChain *	ColorTextureSwapChain;

	/// \deprecated The depth texture is optional for positional time warp.
	ovrTextureSwapChain *	DepthTextureSwapChain;

	/// Index to the texture from the set that should be displayed.
	int						TextureSwapChainIndex;

	/// Points on the screen are mapped by a distortion correction
	/// function into ( TanX, TanY, -1, 1 ) vectors that are transformed
	/// by this matrix to get ( S, T, Q, _ ) vectors that are looked
	/// up with texture2dproj() to get texels.
	ovrMatrix4f				TexCoordsFromTanAngles;

	/// Only texels within this range should be drawn.
	/// This is a sub-rectangle of the [(0,0)-(1,1)] texture coordinate range.
	ovrRectf				TextureRect;

	OVR_VRAPI_PADDING( 4 );

	/// The tracking state for which ModelViewMatrix is correct.
	/// It is ok to update the orientation for each eye, which
	/// can help minimize black edge pull-in, but the position
	/// must remain the same for both eyes, or the position would
	/// seem to judder "backwards in time" if a frame is dropped.
	ovrRigidBodyPosef		HeadPose;

	/// If not zero, this fence will be used to determine whether or not
	/// rendering to the color texture swap chains has completed.
	unsigned long long		CompletionFence;
} ovrFrameLayerTexture;

OVR_VRAPI_ASSERT_TYPE_SIZE_32_BIT( ovrFrameLayerTexture, 200 );
OVR_VRAPI_ASSERT_TYPE_SIZE_64_BIT( ovrFrameLayerTexture, 208 );

/// Per-frame state of a compositor layer.
typedef struct
{
	/// Image used for each eye.
	ovrFrameLayerTexture	Textures[VRAPI_FRAME_LAYER_EYE_MAX];

	/// Speed and scale of rotation when VRAPI_FRAME_LAYER_FLAG_SPIN is set in ovrFrameLayer::Flags
	float					SpinSpeed;	//< Radians/Second
	float					SpinScale;

	/// Color scale for this layer (including alpha)
	float					ColorScale;

	/// padding for deprecated variable.
	OVR_VRAPI_PADDING( 4 );

	/// Layer blend function.
	ovrFrameLayerBlend		SrcBlend;
	ovrFrameLayerBlend		DstBlend;

	/// Combination of ovrFrameLayerFlags flags.
	int						Flags;
} ovrFrameLayer;

OVR_VRAPI_ASSERT_TYPE_SIZE_32_BIT( ovrFrameLayer, 432 );
OVR_VRAPI_ASSERT_TYPE_SIZE_64_BIT( ovrFrameLayer, 448 );

/// Configuration parameters that affect system performance and scheduling behavior.
typedef struct
{
	/// These are fixed clock levels in the range [0, 3].
	int						CpuLevel;
	int						GpuLevel;

	/// These threads will get SCHED_FIFO.
	int						MainThreadTid;
	int						RenderThreadTid;
} ovrPerformanceParms;

OVR_VRAPI_ASSERT_TYPE_SIZE( ovrPerformanceParms, 16 );

/// Per-frame details.
typedef struct
{
	ovrStructureType		Type;

	OVR_VRAPI_PADDING( 4 );

	/// Layers composited in the time warp.
	ovrFrameLayer	 		Layers[VRAPI_FRAME_LAYER_TYPE_MAX];
	int						LayerCount;

	/// Combination of ovrFrameFlags flags.
	int 					Flags;

	/// Application controlled frame index that uniquely identifies this particular frame.
	/// This must be the same frame index that was passed to vrapi_GetPredictedDisplayTime()
	/// when synthesis of this frame started.
	long long				FrameIndex;

	/// WarpSwap will not return until at least this many V-syncs have
	/// passed since the previous WarpSwap returned.
	/// Setting to 2 will reduce power consumption and may make animation
	/// more regular for applications that can't hold full frame rate.
	int						SwapInterval;

	/// Latency Mode.
	ovrExtraLatencyMode		ExtraLatencyMode;

	/// \deprecated
	/// Rotation from a joypad can be added on generated frames to reduce
	/// judder in FPS style experiences when the application framerate is
	/// lower than the V-sync rate.
	/// This will be applied to the view space distorted
	/// eye vectors before applying the rest of the time warp.
	/// This will only be added when the same ovrFrameParms is used for
	/// more than one V-sync.
	ovrMatrix4f				ExternalVelocity;

	/// \deprecated
	/// jobject that will be updated before each eye for minimal
	/// latency.
	/// IMPORTANT: This should be a JNI weak reference to the object.
	/// The system will try to convert it into a global reference before
	/// calling SurfaceTexture->Update, which allows it to be safely
	/// freed by the application.
	jobject					SurfaceTextureObject;

	/// CPU/GPU performance parameters.
	ovrPerformanceParms		PerformanceParms;

	/// For handling HMD events and power level state changes.
	ovrJava					Java;
} ovrFrameParms;

OVR_VRAPI_ASSERT_TYPE_SIZE_32_BIT( ovrFrameParms, 1856 );
OVR_VRAPI_ASSERT_TYPE_SIZE_64_BIT( ovrFrameParms, 1936 );

//-------------------------------------
// New Style Flexible Layer List structures.
//-------------------------------------
enum
{
	ovrMaxLayerCount = 16
};

/// A layer type.
typedef enum ovrLayerType2_
{
	VRAPI_LAYER_TYPE_PROJECTION2			= 1,
///--BEGIN_SDK_REMOVE
//	enum 2 is reserved for VRAPI_LAYER_TYPE_QUAD2
///--END_SDK_REMOVE
	VRAPI_LAYER_TYPE_CYLINDER2				= 3,
	VRAPI_LAYER_TYPE_CUBE2					= 4,
	VRAPI_LAYER_TYPE_EQUIRECT2				= 5,
	VRAPI_LAYER_TYPE_LOADING_ICON2			= 6,
///--BEGIN_SDK_REMOVE
	VRAPI_LAYER_TYPE_FISHEYE2				= 7,
///--END_SDK_REMOVE
} ovrLayerType2;

/// Properties shared by any type of layer.
typedef struct ovrLayerHeader2_
{
	ovrLayerType2		Type;
	uint32_t			Flags;

	ovrVector4f			ColorScale;
	ovrFrameLayerBlend	SrcBlend;
	ovrFrameLayerBlend	DstBlend;

	/// jobject that will be updated before each eye for minimal
	/// latency.
	/// IMPORTANT: This should be a JNI weak reference to the object.
	/// The system will try to convert it into a global reference before
	/// calling SurfaceTexture->Update, which allows it to be safely
	/// freed by the application.
	jobject				SurfaceTextureObject;
} ovrLayerHeader2;

OVR_VRAPI_ASSERT_TYPE_SIZE_32_BIT( ovrLayerHeader2, 36 );
OVR_VRAPI_ASSERT_TYPE_SIZE_64_BIT( ovrLayerHeader2, 40 );

/// ovrLayerProjection2 provides support for a typical world view layer.
typedef struct
{
	/// Header.Type must be VRAPI_LAYER_TYPE_PROJECTION2.
	ovrLayerHeader2			Header;

	ovrRigidBodyPosef		HeadPose;

	struct
	{
		ovrTextureSwapChain * ColorSwapChain;
		int					SwapChainIndex;
		ovrMatrix4f			TexCoordsFromTanAngles;
		ovrRectf			TextureRect;
	} Textures[VRAPI_FRAME_LAYER_EYE_MAX];
} ovrLayerProjection2;

OVR_VRAPI_ASSERT_TYPE_SIZE_32_BIT( ovrLayerProjection2, 312 );
OVR_VRAPI_ASSERT_TYPE_SIZE_64_BIT( ovrLayerProjection2, 328 );

/// ovrLayerCylinder2 provides support for a single 2D texture projected onto a cylinder shape.
///
/// For Cylinder, the vertex coordinates will be transformed as if the texture type was CUBE.
/// Additionally, the interpolated vec3 will be remapped to vec2 by a direction-to-hemicyl mapping.
/// This mapping is currently hard-coded to 180 degrees around and 60 degrees vertical FOV.
///
/// After the mapping to 2D, an optional textureMatrix is applied. In the monoscopic case, the matrix
/// will typically be the identity matrix (ie no scale, bias). In the stereo case, when the image source
/// comes from a single image, the transform is necessary to map the [0.0,1.0] output to a different
/// (sub)rect.
///
/// Regardless of how the textureMatrix transforms the vec2 output of the equirect transform, each
/// TextureRect clamps the resulting texture coordinates so that no coordinates are beyond the specified
/// extents. No guarantees are made about whether fragments will be shaded outside the rect, so it is
/// important that the subrect have a transparent border.
///
typedef struct
{
	/// Header.Type must be VRAPI_LAYER_TYPE_CYLINDER2.
	ovrLayerHeader2			Header;

	ovrRigidBodyPosef		HeadPose;

	struct
	{
		/// Texture type used to create the swapchain must be a 2D target (VRAPI_TEXTURE_TYPE_2D_*).
		ovrTextureSwapChain * ColorSwapChain;
		int					SwapChainIndex;
		ovrMatrix4f			TexCoordsFromTanAngles;
		ovrRectf			TextureRect;
		/// \note textureMatrix is set up like the following:
		///	sx,  0, tx, 0
		///	0,  sy, ty, 0
		///	0,   0,  1, 0
		///	0,   0,  0, 1
		/// since we do not need z coord for mapping to 2d texture.
		ovrMatrix4f			TextureMatrix;
	} Textures[VRAPI_FRAME_LAYER_EYE_MAX];
} ovrLayerCylinder2;

OVR_VRAPI_ASSERT_TYPE_SIZE_32_BIT( ovrLayerCylinder2, 440 );
OVR_VRAPI_ASSERT_TYPE_SIZE_64_BIT( ovrLayerCylinder2, 456 );

/// ovrLayerCube2 provides support for a single timewarped cubemap at infinity
/// with optional Offset vector (provided in normalized [-1.0,1.0] space).
///
/// Cube maps are an omni-directional layer source that are directly supported
/// by the graphics hardware. The nature of the cube map definition results in
/// higher resolution (in pixels per solid angle) at the corners and edges of
/// the cube and lower resolution at the center of each face. While the cube map
/// does have variability in sample density, the variability is spread symmetrically
/// around the sphere.
///
/// Sometimes it is valuable to have an omni-directional format that has a
/// directional bias where quality and sample density is better in a particular
/// direction or over a particular region. If we changed the cube map sampling
/// 
/// from:
///   color = texture( cubeLayerSampler, direction );
/// to:
///   color = texture( cubeLayerSampler, normalize( direction ) + offset );
///
/// we can provide a remapping of the cube map sample distribution such that
/// samples in the "offset" direction map to a smaller region of the cube map
/// (and are thus higher resolution).
///
/// A normal high resolution cube map can be resampled using the inverse of this
/// mapping to retain high resolution for one direction while signficantly reducing
/// the required size of the cube map.
///
typedef struct
{
	/// Header.Type must be VRAPI_LAYER_TYPE_CUBE2.
	ovrLayerHeader2			Header;

	ovrRigidBodyPosef		HeadPose;
	ovrMatrix4f				TexCoordsFromTanAngles;

	ovrVector3f				Offset;

	struct
	{
		/// Texture type used to create the swapchain must be a cube target (VRAPI_TEXTURE_TYPE_CUBE).
		ovrTextureSwapChain * ColorSwapChain;
		int					SwapChainIndex;
	} Textures[VRAPI_FRAME_LAYER_EYE_MAX];
} ovrLayerCube2;

OVR_VRAPI_ASSERT_TYPE_SIZE_32_BIT( ovrLayerCube2, 232 );
OVR_VRAPI_ASSERT_TYPE_SIZE_64_BIT( ovrLayerCube2, 248 );

/// ovrLayerEquirect2 provides support for a single Equirectangular texture at infinity.
///
/// For Equirectangular, the vertex coordinates will be transformed as if the texture type was CUBE,
/// and in the fragment shader, the interpolated vec3 will be remapped to vec2 by a direction-to-equirect
/// mapping.
///
/// After the mapping to 2D, an optional textureMatrix is applied. In the monoscopic case, the matrix
/// will typically be the identity matrix (ie no scale, bias). In the stereo case, when the image source
/// come from a single image, the transform is necessary to map the [0.0,1.0] output to a different
/// (sub)rect.
///
/// Regardless of how the textureMatrix transforms the vec2 output of the equirect transform, each
/// TextureRect clamps the resulting texture coordinates so that no coordinates are beyond the specified
/// extents. No guarantees are made about whether fragments will be shaded outside the rect, so it is
/// important that the subrect have a transparent border.
///
typedef struct
{
	/// Header.Type must be VRAPI_LAYER_TYPE_EQUIRECT2.
	ovrLayerHeader2			Header;

	ovrRigidBodyPosef		HeadPose;
	ovrMatrix4f				TexCoordsFromTanAngles;

	struct
	{
		/// Texture type used to create the swapchain must be a 2D target (VRAPI_TEXTURE_TYPE_2D_*).
		ovrTextureSwapChain * ColorSwapChain;
		int					SwapChainIndex;
		ovrRectf			TextureRect;
		/// \note textureMatrix is set up like the following:
		///	sx,  0, tx, 0
		///	0,  sy, ty, 0
		///	0,   0,  1, 0
		///	0,   0,  0, 1
		/// since we do not need z coord for mapping to 2d texture.
		ovrMatrix4f			TextureMatrix;
	} Textures[VRAPI_FRAME_LAYER_EYE_MAX];
} ovrLayerEquirect2;

OVR_VRAPI_ASSERT_TYPE_SIZE_32_BIT( ovrLayerEquirect2, 376 );
OVR_VRAPI_ASSERT_TYPE_SIZE_64_BIT( ovrLayerEquirect2, 392 );

/// ovrLayerLoadingIcon2 provides support for a monoscopic spinning layer.
///
typedef struct
{
	/// Header.Type must be VRAPI_LAYER_TYPE_LOADING_ICON2.
	ovrLayerHeader2			Header;

	float					SpinSpeed;				//< radians per second
	float					SpinScale;

	/// Only monoscopic texture supported for spinning layer.
	ovrTextureSwapChain *	ColorSwapChain;
	int						SwapChainIndex;
} ovrLayerLoadingIcon2;

OVR_VRAPI_ASSERT_TYPE_SIZE_32_BIT( ovrLayerLoadingIcon2, 52 );
OVR_VRAPI_ASSERT_TYPE_SIZE_64_BIT( ovrLayerLoadingIcon2, 64 );

///--BEGIN_SDK_REMOVE
/// An "equiangular fisheye" or "f-theta" lens can be used to capture photos or video
/// of around 180 degrees without stitching.
///
/// The cameras probably aren't exactly vertical, so a transformation may need to be applied
/// before performing the fisheye calculation.
/// A stereo fisheye camera rig will usually have slight misalignments between the two
/// cameras, so they need independent transformations.
///
/// Once in lens space, the ray is transformed into an ideal fisheye projection, where the
/// 180 degree hemisphere is mapped to a -1 to 1 2D space.
///
/// From there it can be mapped into actual texture coordinates, possibly two to an image for stereo.
///
typedef struct
{
	/// Header.Type must be VRAPI_LAYER_TYPE_FISHEYE2.
	ovrLayerHeader2	Header;

	ovrRigidBodyPosef	HeadPose;

	struct
	{
		ovrTextureSwapChain * ColorSwapChain;
		int					SwapChainIndex;
		ovrMatrix4f			LensFromTanAngles;			//< transforms a tanAngle ray into lens space
		ovrRectf			TextureRect;				//< packed stereo images will need to clamp at the mid border
		ovrMatrix4f			TextureMatrix;				//< transform from a -1 to 1 ideal fisheye to the texture
		ovrVector4f			Distortion;					//< Not currently used.
	} Textures[VRAPI_FRAME_LAYER_EYE_MAX];
} ovrLayerFishEye2;

OVR_VRAPI_ASSERT_TYPE_SIZE_32_BIT( ovrLayerFishEye2, 472 );
OVR_VRAPI_ASSERT_TYPE_SIZE_64_BIT( ovrLayerFishEye2, 488 );
///--END_SDK_REMOVE

/// Union that combines ovrLayer types in a way that allows them
/// to be used in a polymorphic way.
typedef union
{
	ovrLayerHeader2			Header;
	ovrLayerProjection2		Projection;
	ovrLayerCylinder2		Cylinder;
	ovrLayerCube2			Cube;
	ovrLayerEquirect2		Equirect;
	ovrLayerLoadingIcon2	LoadingIcon;
///--BEGIN_SDK_REMOVE
	ovrLayerFishEye2		FishEye;
///--END_SDK_REMOVE
} ovrLayer_Union2;

/// Parameters for frame submission.
typedef struct ovrSubmitFrameDescription2_
{
	uint32_t			Flags;
	uint32_t			SwapInterval;
	uint64_t			FrameIndex;
	double 				DisplayTime;
	unsigned long long	CompletionFence;
	uint32_t			LayerCount;
	const ovrLayerHeader2 *	const * Layers;
} ovrSubmitFrameDescription2;

OVR_VRAPI_ASSERT_TYPE_SIZE_32_BIT( ovrSubmitFrameDescription2, 40 );
OVR_VRAPI_ASSERT_TYPE_SIZE_64_BIT( ovrSubmitFrameDescription2, 48 );

//-----------------------------------------------------------------
// Performance
//-----------------------------------------------------------------

/// Identifies a VR-related application thread.
typedef enum
{
	VRAPI_PERF_THREAD_TYPE_MAIN			= 0,
	VRAPI_PERF_THREAD_TYPE_RENDERER		= 1,
} ovrPerfThreadType;

#endif	// OVR_VrApi_Types_h
