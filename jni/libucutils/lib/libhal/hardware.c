/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "hardware.h"

#include <u/libu.h>

#include <dlfcn.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <time.h> 

#if 0
int hw_get_module_by_class(const char *class_id, const char *inst,
                           const struct hw_module_t **module)
{
    return -1;
}

int hw_get_module(const char *id, const struct hw_module_t **module)
{
    return -1;
}
#endif

int lib_module_load(const char *path,
        const struct hw_module_t **pHmi)
{
    int status = -EINVAL;
    void *handle = NULL;
    struct hw_module_t *hmi = NULL;

    /*
     * load the symbols resolving undefined symbols before
     * dlopen returns. Since RTLD_GLOBAL is not or'd in with
     * RTLD_NOW the external symbols will not be global
     */
    handle = dlopen(path, RTLD_NOW);
    if (handle == NULL) {
        char const *err_str = dlerror();
        ALOGE("load: module=%s\n%s", path, err_str?err_str:"unknown");
        status = -EINVAL;
        goto done;
    }

    /* Get the address of the struct hal_module_info. */
    const char *sym = HAL_MODULE_INFO_SYM_AS_STR;
    hmi = (struct hw_module_t *)dlsym(handle, sym);
    if (hmi == NULL) {
        ALOGE("load: couldn't find symbol %s", sym);
        status = -EINVAL;
        goto done;
    }

    hmi->dso = handle;

    /* success */
    status = 0;

done:
    if (status != 0) {
        hmi = NULL;
        if (handle != NULL) {
            dlclose(handle);
            handle = NULL;
        }
    } else {
        ALOGI("loaded HAL id=%s path=%s hmi=%p handle=%p api:%d",
                hmi->id, path, *pHmi, handle, hmi->module_api_version);
    }

    *pHmi = hmi;

    return status;
}

int lib_module_close(struct hw_module_t **pHmi){
    int status = -EINVAL;
    struct hw_module_t *hmi = NULL;
    hmi = *pHmi;
    
    if(hmi->dso){
        //status = dlclose(hmi->dso);
        status = 0;
        //hmi->dso = NULL;
        ALOGI("unloaded HAL id=%s hmi=%p handle=%p api:%d", 
                hmi->id, hmi, hmi->dso, hmi->module_api_version);
    }
    
    return status;
}


int openHal(const char *path, hw_device_t **hw_device) {
    ALOGV("openHal path %s",path);
    int err;
    hw_module_t *hw_module = NULL;
    if (0 != (err = lib_module_load(path, &hw_module))) {
        ALOGE("Can't open HW Module, error: %d", err);
        return -1;
    }
    if (NULL == hw_module) {
        ALOGE("No valid module");
        return -1;
    }

    if (hw_module->methods->open == NULL) {
        ALOGE("No valid open method");
        return -1;
    }

    hw_device_t *device = *hw_device;

    if (0 != (err = hw_module->methods->open(hw_module, NULL, &device))) {
        ALOGE("Can't open methods, error: %d", err);
        return -1;
    }

    ALOGV("openHal device version got, %p, %d, %s", device,device->version,ctime(&device->version));

    *hw_device = device;

    return 0;
}

int closeHal(hw_device_t *hw_device) {
    ALOGV("closeHal");
    if (hw_device == NULL) {
        ALOGE("No valid hw_device");
        return -ENOSYS;
    }

    hw_module_t *hw_module = hw_device->module;
    int err;
    if (0 != (err = hw_device->close(hw_device))) {
        ALOGE("Can't close module, error: %d", err);
        return err;
    }

    if (0 != (err = lib_module_close(&hw_module))) {
        ALOGE("Can't dlclose module, error: %d", err);
        return err;
    }
    return 0;
}
