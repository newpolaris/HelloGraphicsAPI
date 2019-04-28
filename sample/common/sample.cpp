#include "sample.h"
#include <predefine.h>

const std::string el::getResourcePath()
{
#if EL_PLAT_IOS
    return[NSBundle.mainBundle.resourcePath stringByAppendingString : @"/data / "].UTF8String;
    #elif EL_PLAT_ANDROID
    return "";
#else
    return EL_DEFINE_RESOURCE_PATH;
#endif
}

const std::string el::getSamplePath()
{
    return EL_DEFINE_SAMPLE_PATH;
}

