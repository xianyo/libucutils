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

#include <u/libu.h>

#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <time.h> 


time_t getLibUBuildUTC() {
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

/*
type 0
00000000  00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |........ ........|
00000010  10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  |........ ........|
00000020  20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f  | !"#$%&' ()*+,-./|
00000030  30 31 32 33 34 35 36 37  38 39 3a 3b 3c 3d 3e 3f  |01234567 89:;<=>?|
00000040  40 41 42 43 44 45 46 47  48 49 4a 4b 4c 4d 4e 4f  |@ABCDEFG HIJKLMNO|
00000050  50 51 52 53 54 55 56 57  58 59 5a 5b 5c 5d 5e 5f  |PQRSTUVW XYZ[\]^_|
00000060  60 61 62 63                                       |`abc             |

type 1
00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f
10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f
20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f
30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f
40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f
50 51 52 53 54 55 56 57 58 59 5a 5b 5c 5d 5e 5f
60 61 62 63

type 2
000102030405060708090a0b0c0d0e0f
101112131415161718191a1b1c1d1e1f
202122232425262728292a2b2c2d2e2f
303132333435363738393a3b3c3d3e3f
404142434445464748494a4b4c4d4e4f
505152535455565758595a5b5c5d5e5f
60616263


type 3
0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,

0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,

0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,

0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,

0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,

0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,

0x60,0x61,0x62,0x63,

 */

int hexdump(char type, char *in, int len, char *out, int limit) {
#define SEGMENTS_PER_LINE (2)
#define BYTES_PER_SEGMENT (8)

#define BYTES_PER_LINE    (SEGMENTS_PER_LINE * BYTES_PER_SEGMENT)

  int pos = 0, cnt = 0, cnt2 = 0, posx = 0, cur_byte = 0;
  char tmpbuf[9], outbuf[10 + BYTES_PER_LINE * 3 + SEGMENTS_PER_LINE + BYTES_PER_LINE + 4];
  if(type==0){
      while (cur_byte < len) {
        pos = cur_byte % BYTES_PER_LINE;
        if (pos == 0) {
          if (cur_byte > 0){
            cnt = snprintf(out+posx,limit-posx,"%s\n", outbuf);
            posx = posx + cnt;
            //printf("%s\n",outbuf);
          }
          memset(outbuf, ' ', sizeof(outbuf));
          outbuf[sizeof(outbuf)-1] = '\0';
          sprintf(tmpbuf, "%08x", cur_byte);
          memcpy(outbuf, tmpbuf, 8);
          outbuf[BYTES_PER_LINE * 3 + 12] = outbuf[BYTES_PER_LINE * 4 + 14] = '|';
        }
        sprintf(tmpbuf, "%02x", in[cur_byte]);
        memcpy(outbuf + 10 + (pos * 3) + ((int)(pos/BYTES_PER_SEGMENT)), tmpbuf, 2);
        outbuf[13 + BYTES_PER_LINE * 3 + ((int)(pos / BYTES_PER_SEGMENT)) + pos] = isprint(in[cur_byte]) ? in[cur_byte] : '.';
        cur_byte += 1;
      }
      snprintf(out+posx,limit-posx,"%s\n", outbuf);
  }else if(type==1){
    while (cur_byte < len) {
        pos = cur_byte % BYTES_PER_LINE;
        if (pos == 0) {
          if (cur_byte > 0){
            cnt = snprintf(out+posx,limit-posx,"%s\n", outbuf);
            posx = posx + cnt;
            //printf("%s\n",outbuf);
          }
          cnt2 = 0;
          memset(outbuf, 0 , sizeof(outbuf));
          outbuf[sizeof(outbuf)-1] = '\0';
        }
        cnt = sprintf(outbuf+cnt2, "%02x ", in[cur_byte]);
        cnt2= cnt2+cnt;
        cur_byte += 1;
      }
      snprintf(out+posx,limit-posx,"%s\n", outbuf);
  }else if(type==2){
    while (cur_byte < len) {
        pos = cur_byte % BYTES_PER_LINE;
        if (pos == 0) {
          if (cur_byte > 0){
            cnt = snprintf(out+posx,limit-posx,"%s\n", outbuf);
            posx = posx + cnt;
            //printf("%s\n",outbuf);
          }
          cnt2 = 0;
          memset(outbuf, 0 , sizeof(outbuf));
          outbuf[sizeof(outbuf)-1] = '\0';
        }
        cnt = sprintf(outbuf+cnt2, "%02x", in[cur_byte]);
        cnt2= cnt2+cnt;
        cur_byte += 1;
      }
      snprintf(out+posx,limit-posx,"%s\n", outbuf);
  }else {
    while (cur_byte < len) {
        pos = cur_byte % BYTES_PER_LINE;
        if (pos == 0) {
          if (cur_byte > 0){
            cnt = snprintf(out+posx,limit-posx,"%s\n", outbuf);
            posx = posx + cnt;
            //printf("%s\n",outbuf);
          }
          cnt2 = 0;
          memset(outbuf, 0 , sizeof(outbuf));
          outbuf[sizeof(outbuf)-1] = '\0';
        }
        cnt = sprintf(outbuf+cnt2, "0x%02x, ", in[cur_byte]);
        cnt2= cnt2+cnt;
        cur_byte += 1;
      }
      snprintf(out+posx,limit-posx,"%s\n", outbuf);
  }
  //printf("%s\n",outbuf);
  return 0;
}