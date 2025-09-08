#ifndef _UAPI__LINUX_VIDEODEV2_H
#define _UAPI__LINUX_VIDEODEV2_H

#include <linux/types.h>
#include <linux/time.h>

#define V4L2_CID_BASE 0x00980900
#define V4L2_BUF_TYPE_VIDEO_CAPTURE 1
#define V4L2_BUF_TYPE_VIDEO_OUTPUT 2
#define V4L2_BUF_TYPE_VIDEO_OVERLAY 3
#define V4L2_BUF_TYPE_VBI_CAPTURE 4
#define V4L2_BUF_TYPE_VBI_OUTPUT 5
#define V4L2_BUF_TYPE_SLICED_VBI_CAPTURE 6
#define V4L2_BUF_TYPE_SLICED_VBI_OUTPUT 7
#define V4L2_BUF_TYPE_VIDEO_OUTPUT_OVERLAY 8
#define V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE 9
#define V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE 10
#define V4L2_BUF_TYPE_SDR_CAPTURE 11
#define V4L2_BUF_TYPE_SDR_OUTPUT 12
#define V4L2_BUF_TYPE_META_CAPTURE 13
#define V4L2_BUF_TYPE_META_OUTPUT 14

#define V4L2_PIX_FMT_YUV420 0x30323449  /* YUV 4:2:0 */
#define V4L2_PIX_FMT_NV12 'p'

#define V4L2_FIELD_ANY           0
#define V4L2_FIELD_NONE          1
#define V4L2_FIELD_INTERLACED    2
#define V4L2_FIELD_SEQ_TB        3
#define V4L2_FIELD_SEQ_BT        4
#define V4L2_FIELD_INTERLACED_TB 5
#define V4L2_FIELD_INTERLACED_BT 6
#define V4L2_FIELD_PROGRESSIVE   7

#define V4L2_BUF_FLAG_MAPPED	0x00000001
#define V4L2_BUF_FLAG_QUEUED	0x00000002
#define V4L2_BUF_FLAG_DONE	0x00000004
#define V4L2_BUF_FLAG_KEYFRAME	0x00000008
#define V4L2_BUF_FLAG_PFRAME	0x00000010
#define V4L2_BUF_FLAG_BFRAME	0x00000020
#define V4L2_BUF_FLAG_ERROR	0x00000040
#define V4L2_BUF_FLAG_TIMECODE	0x00000100
#define V4L2_BUF_FLAG_INPUT     0x00000200
#define V4L2_BUF_FLAG_PREPARED	0x00000400
#define V4L2_BUF_FLAG_NO_CACHE  0x00000800
#define V4L2_BUF_FLAG_RECOVERY_MSECS 0x00001000
#define V4L2_BUF_FLAG_TIMESTAMP_MASK		(0x03 << 14)
#define V4L2_BUF_FLAG_TIMESTAMP_UNKNOWN		(0x00 << 14)
#define V4L2_BUF_FLAG_TIMESTAMP_MONOTONIC	(0x01 << 14)
#define V4L2_BUF_FLAG_TIMESTAMP_COPY		(0x02 << 14)
#define V4L2_BUF_FLAG_TSTAMP_SRC_MASK	(0x03 << 16)
#define V4L2_BUF_FLAG_TSTAMP_CONTIN		(0x01 << 16)
#define V4L2_BUF_FLAG_TSTAMP_DELTA		(0x02 << 16)
#define V4L2_BUF_FLAG_LAST		0x80000000

#define V4L2_MEMORY_MMAP		1
#define V4L2_MEMORY_USERPTR		2
#define V4L2_MEMORY_OVERLAY		3
#define V4L2_MEMORY_DMABUF		4

struct v4l2_rect {
	__s32   left;
	__s32   top;
	__s32   width;
	__s32   height;
};

struct v4l2_timecode {
	__u32	type;
	__u32	flags;
	__u8	frames;
	__u8	seconds;
	__u8	minutes;
	__u8	hours;
	__u8	userbits[4];
};

struct v4l2_plane_pix_format {
	__u32		sizeimage;
	__u32		bytesperline;
	__u32		reserved[6];
};

struct v4l2_pix_format_mplane {
	__u32		width;
	__u32		height;
	__u32		pixelformat;
	__u32		field;
	__u32		colorspace;
	__u32		plane_fmt[8];
	__u8		num_planes;
	__u8		flags;
	__u8		ycbcr_enc;
	__u8		quantization;
	__u8		xfer_func;
	__u8		reserved[7];
};

struct v4l2_plane {
	__u32			bytesused;
	__u32			length;
	union {
		__u32		mem_offset;
		unsigned long	userptr;
		__s32		fd;
	} m;
	__u32			data_offset;
	__u32			reserved[11];
};

struct v4l2_buffer {
	__u32			index;
	__u32			type;
	__u32			bytesused;
	__u32			flags;
	__u32			field;
	struct timeval		timestamp;
	struct v4l2_timecode	timecode;
	__u32			sequence;
	/* memory location */
	__u32			memory;
	union {
		__u32           offset;
		unsigned long   userptr;
		struct v4l2_plane *planes;
		__s32		fd;
	} m;
	__u32			length;
	__u32			reserved2;
	union {
		__u32		fence_fd;
		__u32		reserved;
	};
};

struct v4l2_pix_format {
	__u32			width;
	__u32			height;
	__u32			pixelformat;
	__u32			field;
	__s32			bytesperline;
	__s32			sizeimage;
	__u32			colorspace;
	__u32			priv;
	__u32			flags;
	union {
		__u32		yzuv_planes;
		__u32		planar_output;
	};
	__u32			quantization;
	__u32			xfer_func;
};

struct v4l2_format {
	__u32	 type;
	union {
		struct v4l2_pix_format	pix;
		struct v4l2_pix_format_mplane	pix_mp;
		__u8	raw_data[200];
	} fmt;
};

struct v4l2_capability {
	__u8	driver[16];
	__u8	card[32];
	__u8	bus_info[32];
	__u32	version;
	__u32	capabilities;
	__u32	device_caps;
	__u32	reserved[3];
};

#define VIDIOC_QUERYCAP		_IOR('V',  0, struct v4l2_capability)

#endif /* _UAPI__LINUX_VIDEODEV2_H */