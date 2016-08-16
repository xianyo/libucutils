
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <u/libu.h>
#include <hardware/samplemodule.h>
#include <time.h> 



time_t getBuildUTC() {
    char s_month[5];
    int month, day, year, hour, minute, second;
    struct tm t = {0};
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    const char* time = __TIME__ " "__DATE__ ;

    sscanf(__DATE__, "%s %d %d", s_month, &day, &year); 
    month = (strstr(month_names, s_month)-month_names)/3;
    sscanf(__TIME__, "%d:%d:%d", &hour, &minute, &second);
    
    t.tm_mon = month;
    t.tm_mday = day;
    t.tm_year = year - 1900;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;
    t.tm_isdst = -1;

    return mktime(&t);
}

static int sample_close(hw_device_t *dev)
{
    if (dev) {
        free(dev);
        return 0;
    } else {
        return -1;
    }
}


static int sample_set(struct sample_device __unused *dev, int __unused flag) {
    ALOGI("sample_set flag %d",flag);
    return -1;
}

static int sample_open(const hw_module_t* module, const char __unused *id,
                            hw_device_t** device)
{
    if (device == NULL) {
        ALOGE("NULL device on open");
        return -EINVAL;
    }

    sample_device_t *dev = malloc(sizeof(sample_device_t));
    memset(dev, 0, sizeof(sample_device_t));

    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = getBuildUTC();
    dev->common.module = (struct hw_module_t*) module;
    dev->common.close = sample_close;

    dev->set = sample_set;

    *device = (hw_device_t*) dev;
    ALOGI("sample_open %s",ctime(&dev->common.version));
    return 0;
}

static struct hw_module_methods_t sample_module_methods = {
    .open = sample_open,
};

sample_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag                = HARDWARE_MODULE_TAG,
        .module_api_version = SAMPLE_MODULE_API_VERSION_2_0,
        .hal_api_version    = HARDWARE_HAL_API_VERSION,
        .id                 = SAMPLE_HARDWARE_MODULE_ID,
        .name               = "Demo Sample Module HAL",
        .author             = "The Android Open Source Project",
        .methods            = &sample_module_methods,
    },
};