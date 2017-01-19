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

#ifndef ANDROID_INCLUDE_MYUTILS_H
#define ANDROID_INCLUDE_MYUTILS_H

#include <stdint.h>
#include <sys/cdefs.h>


__BEGIN_DECLS

time_t getLibUBuildUTC();
int hexdump(char type, char *in, int len, char *out, int limit);
__END_DECLS

#endif  /* ANDROID_INCLUDE_MYUTILS_H */