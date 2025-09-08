#ifndef _UAPI_LINUX_MEDIA_H
#define _UAPI_LINUX_MEDIA_H

#include <linux/types.h>

#define MEDIA_IOC_DEVICE_INFO _IOR('M', 2, struct media_device_info)
#define MEDIA_IOC_ENUM_ENTITIES _IOWR('M', 3, struct media_entity_desc)
#define MEDIA_IOC_ENUM_LINKS _IOWR('M', 4, struct media_links_enum)

struct media_device_info {
    char driver[32];
    char model[32];
    char serial[32];
    char bus_info[32];
    __u32 media_version;
    __u32 hw_revision;
    __u32 driver_version;
    __u32 reserved[31];
};

struct media_entity_desc {
    __u32 id;
    char name[32];
    __u32 type;
    __u32 revision;
    __u32 flags;
    __u32 group_id;
    __u32 pads;
    __u32 links;
    __u32 reserved[4];
};

struct media_links_enum {
    __u32 entity;
    __u32 pads;
    __u32 links;
    struct media_pad_desc *pads;
    struct media_link_desc *links;
    __u32 reserved[4];
};

struct media_pad_desc {
    __u32 entity;
    __u32 index;
    __u32 flags;
    __u32 reserved[3];
};

struct media_link_desc {
    __u32 source_entity;
    __u32 source_pad;
    __u32 flags;
    __u32 target_entity;
    __u32 target_pad;
    __u32 reserved[2];
};

#define MEDIA_ENT_F_OLD_BASE (0x00000000)
#define MEDIA_ENT_F_IO_V4L (MEDIA_ENT_F_OLD_BASE + 0x00000001)
#define MEDIA_ENT_F_V4L2_SUBDEV_IO (MEDIA_ENT_F_OLD_BASE + 0x00010000)

#define MEDIA_PAD_FL_SINK (1 << 0)
#define MEDIA_PAD_FL_SOURCE (1 << 1)

#define MEDIA_LNK_FL_ENABLED (1 << 0)
#define MEDIA_LNK_FL_IMMUTABLE (1 << 1)
#define MEDIA_LNK_FL_DYNAMIC (1 << 2)

#endif /* _UAPI_LINUX_MEDIA_H */