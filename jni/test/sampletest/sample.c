
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

    sscanf("build_date:"__DATE__, "build_date:%s %d %d", s_month, &day, &year); 
    month = (strstr(month_names, s_month)-month_names)/3;
    sscanf("build_time:"__TIME__, "build_time:%d:%d:%d", &hour, &minute, &second);
    
    t.tm_mon = month;
    t.tm_mday = day;
    t.tm_year = year - 1900;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;
    t.tm_isdst = -1;

    return mktime(&t);
}


int main(int argc, char const *argv[])
{
    ALOGI("main %s %s ",__DATE__,__TIME__);
    int build_time = getBuildUTC();
    u_info("time_t %d",build_time);
    ALOGI("build time %s",ctime(&build_time));

    sample_device_t *device;

    if(0!=openHal("/system/lib/hw/libsamplemodule.so",&device)){
        return -1;
    }

    ALOGI("open ok %p",device);

    device->set(device,build_time);

    closeHal(device);
    /* code */
    return 0;
}