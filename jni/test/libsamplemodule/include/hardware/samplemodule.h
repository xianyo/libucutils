

#ifndef ANDROID_INCLUDE_SAMPLEMODULE_HARDWARE_H
#define ANDROID_INCLUDE_SAMPLEMODULE_HARDWARE_H

#include <stdint.h>
#include <sys/cdefs.h>

#include <u/libu.h>

__BEGIN_DECLS

#define SAMPLE_MODULE_API_VERSION_1_0 HARDWARE_MODULE_API_VERSION(1, 0)
#define SAMPLE_MODULE_API_VERSION_2_0 HARDWARE_MODULE_API_VERSION(2, 0)
#define SAMPLE_HARDWARE_MODULE_ID "sample"


typedef struct sample_device {
    struct hw_device_t common;
    int (*set)(struct sample_device* dev,int flag);
}sample_device_t;

typedef struct sample_module {
    struct hw_module_t common;
} sample_module_t;

__END_DECLS

#endif  /* ANDROID_INCLUDE_HARDWARE_HARDWARE_H */