
// The following includes may fail if the kernel headers are not available in the build environment.
// To fix this, ensure that the include paths are set correctly and that the required headers are present.
// If building outside of the target device, you may need to provide stubs or use third_party headers.
// Use third_party/linux/include for kernel headers to avoid dependency issues

// The following headers are required by the third_party media headers.
// If you encounter errors about missing "linux/videodev2.h" or "linux/types.h",
// ensure that stubs for these headers exist in third_party/linux/include/linux/.
// You may need to create empty or minimal versions of these headers if building outside the target device.

#include "../../../../third_party/linux/include/linux/types.h"
#include "../../../../third_party/linux/include/linux/videodev2.h"

#include "../../../../third_party/linux/include/media/cam_defs.h"
#include "../../../../third_party/linux/include/media/cam_isp.h"
#include "../../../../third_party/linux/include/media/cam_icp.h"
#include "../../../../third_party/linux/include/media/cam_isp_ife.h"
#include "../../../../third_party/linux/include/media/cam_sensor_cmn_header.h"

