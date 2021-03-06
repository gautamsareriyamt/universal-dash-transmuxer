/*
Copyright 2014 Google Inc. All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "library/ps/nalu.h"
#include "library/ps/pes.h"
#include "library/ps/program_stream_out.h"
#include "library/utilities_gmock.h"

namespace {
const size_t kNaluLength = 4;
const size_t kScrDefaultValue = 0;
const size_t kDefaultPts = 18000;
const size_t kDefaultDts = 18000;
const size_t kDefaultScr = 0;
const uint64_t kDefaultDuration = 9000;
const uint32_t kDefaultMuxRate = 94;
const uint8_t kExpectedOutput[] = {
  0x00, 0x00, 0x01, 0xba, 0x44, 0x00, 0x04, 0x00,
  0x04, 0x01, 0x00, 0x01, 0x7b, 0xf8, 0x00, 0x00,
  0x01, 0xbb, 0x00, 0x0f, 0x80, 0x00, 0x01, 0x04,
  0xe1, 0x7f, 0xbc, 0xe0, 0x00, 0xe0, 0xe0, 0x00,
  0xc0, 0xe0, 0x00, 0x00, 0x00, 0x01, 0xbc, 0x00,
  0x4e, 0xe0, 0xff, 0x00, 0x36, 0x1d, 0x34, 0x10,
  0x01, 0x02, 0x80, 0x80, 0x2e, 0x00, 0x4f, 0xff,
  0xff, 0xfe, 0xfe, 0xff, 0x03, 0x80, 0x80, 0x80,
  0x22, 0x00, 0xc0, 0x00, 0x04, 0x80, 0x80, 0x80,
  0x14, 0x40, 0x15, 0x00, 0x18, 0x00, 0x00, 0x01,
  0xf4, 0x00, 0x00, 0x01, 0xf4, 0x00, 0x05, 0x80,
  0x80, 0x80, 0x02, 0x12, 0x10, 0x06, 0x80, 0x80,
  0x80, 0x01, 0x02, 0x00, 0x0e, 0x1b, 0xe0, 0x00,
  0x03, 0x06, 0x01, 0x03, 0x0f, 0xc0, 0x00, 0x03,
  0x06, 0x01, 0x01, 0xad, 0xd0, 0x20, 0x1a, 0x00,
  0x00, 0x01, 0xe0, 0x01, 0x5c, 0x84, 0x80, 0x05,
  0x21, 0x00, 0x01, 0x8c, 0xa1, 0x00, 0x00, 0x00,
  0x01, 0x09, 0x10, 0x00, 0x00, 0x00, 0x01, 0x27,
  0x42, 0xe0, 0x0d, 0xa9, 0x18, 0x28, 0x3f, 0x60,
  0x0d, 0x41, 0x80, 0x41, 0xad, 0xb7, 0xa0, 0x2f,
  0x01, 0xe9, 0x7b, 0xdf, 0x01, 0x00, 0x00, 0x00,
  0x01, 0x28, 0xce, 0x09, 0x88, 0x00, 0x00, 0x00,
  0x01, 0x06, 0x00, 0x07, 0x81, 0xf6, 0x3b, 0x80,
  0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x00, 0x01,
  0x25, 0xb8, 0x20, 0x00, 0xf9, 0x31, 0x40, 0x00,
  0x52, 0xea, 0xfb, 0xef, 0xbe, 0xfb, 0xef, 0xbe,
  0xfb, 0xef, 0xbe, 0xfb, 0xef, 0xbe, 0xfb, 0xef,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xaf, 0xff, 0xf9, 0xf0, 0xa0, 0x43, 0x80, 0x91,
  0xeb, 0x7e, 0x5a, 0xbb, 0x9b, 0xf3, 0xf0, 0x00,
  0x00, 0x00, 0x01, 0x25, 0x01, 0x2e, 0xe0, 0x80,
  0x07, 0xff, 0xff, 0xe1, 0x38, 0xa0, 0x00, 0x20,
  0xb1, 0x80, 0x87, 0xde, 0xfc, 0xf7, 0xb5, 0xb3,
  0x73, 0xf3, 0xc3, 0xb8, 0x46, 0xfb, 0xef, 0xbe,
  0xfb, 0xef, 0xbe, 0x4c, 0x50, 0x00, 0x14, 0xba,
  0xbe, 0xfb, 0xef, 0xbe, 0xfb, 0xef, 0xbe, 0xfa,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xf0};
const uint8_t kInputSample[] = {
  0x00, 0x00, 0x00, 0x0b, 0x06, 0x00, 0x07, 0x81,
  0xf6, 0x3b, 0x80, 0x00, 0x00, 0x40, 0x80, 0x00,
  0x00, 0x00, 0x87, 0x25, 0xb8, 0x20, 0x00, 0xf9,
  0x31, 0x40, 0x00, 0x52, 0xea, 0xfb, 0xef, 0xbe,
  0xfb, 0xef, 0xbe, 0xfb, 0xef, 0xbe, 0xfb, 0xef,
  0xbe, 0xfb, 0xef, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xaf, 0xff, 0xf9, 0xf0, 0xa0,
  0x43, 0x80, 0x91, 0xeb, 0x7e, 0x5a, 0xbb, 0x9b,
  0xf3, 0xf0, 0x00, 0x00, 0x00, 0x8e, 0x25, 0x01,
  0x2e, 0xe0, 0x80, 0x07, 0xff, 0xff, 0xe1, 0x38,
  0xa0, 0x00, 0x20, 0xb1, 0x80, 0x87, 0xde, 0xfc,
  0xf7, 0xb5, 0xb3, 0x73, 0xf3, 0xc3, 0xb8, 0x46,
  0xfb, 0xef, 0xbe, 0xfb, 0xef, 0xbe, 0x4c, 0x50,
  0x00, 0x14, 0xba, 0xbe, 0xfb, 0xef, 0xbe, 0xfb,
  0xef, 0xbe, 0xfa, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xf0};

const uint8_t kInputSample2[] = {
  0x00, 0x00, 0x00, 0x22, 0x21, 0xe1, 0x04, 0x03,
  0xe0, 0x25, 0x4d, 0xc0, 0x7f, 0x00, 0x0e, 0xc2,
  0x10, 0x30, 0x40, 0x8f, 0x2d, 0x53, 0x47, 0xd3,
  0x6f, 0x1a, 0x41, 0x92, 0x5b, 0xb8, 0x97, 0xc2,
  0x9f, 0xfd, 0xb6, 0xf6, 0xdb, 0xe0, 0x00, 0x00,
  0x00, 0x2b, 0x21, 0x01, 0x2f, 0x84, 0x10, 0x1f,
  0x34, 0x71, 0x1f, 0xfe, 0x13, 0x81, 0xc1, 0x10,
  0x00, 0x15, 0xee, 0xe0, 0x68, 0x45, 0x44, 0xb4,
  0x9f, 0xe6, 0x11, 0xc7, 0x0f, 0xe1, 0x19, 0x40,
  0xc5, 0x00, 0xe0, 0x3a, 0x66, 0xd3, 0x18, 0x3f,
  0x35, 0x6f, 0x5f, 0x01, 0x2b};
const uint8_t kExpectedOutput2[] = {
  0x00, 0x00, 0x01, 0xba, 0x44, 0x00, 0x05, 0x19,
  0x44, 0x01, 0x00, 0x00, 0x5f, 0xf8, 0x00, 0x00,
  0x01, 0xe0, 0x00, 0x63, 0x80, 0x80, 0x05, 0x21,
  0x00, 0x01, 0xd2, 0xf1, 0x00, 0x00, 0x00, 0x01,
  0x09, 0x30, 0x00, 0x00, 0x00, 0x01, 0x21, 0xe1,
  0x04, 0x03, 0xe0, 0x25, 0x4d, 0xc0, 0x7f, 0x00,
  0x0e, 0xc2, 0x10, 0x30, 0x40, 0x8f, 0x2d, 0x53,
  0x47, 0xd3, 0x6f, 0x1a, 0x41, 0x92, 0x5b, 0xb8,
  0x97, 0xc2, 0x9f, 0xfd, 0xb6, 0xf6, 0xdb, 0xe0,
  0x00, 0x00, 0x00, 0x01, 0x21, 0x01, 0x2f, 0x84,
  0x10, 0x1f, 0x34, 0x71, 0x1f, 0xfe, 0x13, 0x81,
  0xc1, 0x10, 0x00, 0x15, 0xee, 0xe0, 0x68, 0x45,
  0x44, 0xb4, 0x9f, 0xe6, 0x11, 0xc7, 0x0f, 0xe1,
  0x19, 0x40, 0xc5, 0x00, 0xe0, 0x3a, 0x66, 0xd3,
  0x18, 0x3f, 0x35, 0x6f, 0x5f, 0x01, 0x2b};

const uint8_t kExpectedWithSpsPpsAud[] = {
  0x00, 0x00, 0x00, 0x02, 0x09, 0x10, 0x00, 0x00,
  0x00, 0x16, 0x27, 0x42, 0xe0, 0x0d, 0xa9, 0x18,
  0x28, 0x3f, 0x60, 0x0d, 0x41, 0x80, 0x41, 0xad,
  0xb7, 0xa0, 0x2f, 0x01, 0xe9, 0x7b, 0xdf, 0x01,
  0x00, 0x00, 0x00, 0x04, 0x28, 0xce, 0x09, 0x88,
  0x00, 0x00, 0x00, 0x0b, 0x06, 0x00, 0x07, 0x81,
  0xf6, 0x3b, 0x80, 0x00, 0x00, 0x40, 0x80, 0x00,
  0x00, 0x00, 0x87, 0x25, 0xb8, 0x20, 0x00, 0xf9,
  0x31, 0x40, 0x00, 0x52, 0xea, 0xfb, 0xef, 0xbe,
  0xfb, 0xef, 0xbe, 0xfb, 0xef, 0xbe, 0xfb, 0xef,
  0xbe, 0xfb, 0xef, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xaf, 0xff, 0xf9, 0xf0, 0xa0,
  0x43, 0x80, 0x91, 0xeb, 0x7e, 0x5a, 0xbb, 0x9b,
  0xf3, 0xf0, 0x00, 0x00, 0x00, 0x8e, 0x25, 0x01,
  0x2e, 0xe0, 0x80, 0x07, 0xff, 0xff, 0xe1, 0x38,
  0xa0, 0x00, 0x20, 0xb1, 0x80, 0x87, 0xde, 0xfc,
  0xf7, 0xb5, 0xb3, 0x73, 0xf3, 0xc3, 0xb8, 0x46,
  0xfb, 0xef, 0xbe, 0xfb, 0xef, 0xbe, 0x4c, 0x50,
  0x00, 0x14, 0xba, 0xbe, 0xfb, 0xef, 0xbe, 0xfb,
  0xef, 0xbe, 0xfa, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xf0};

const uint8_t kExpectedAfterFixingNalu[] = {
  0x00, 0x00, 0x00, 0x01, 0x09, 0x10, 0x00, 0x00,
  0x00, 0x01, 0x27, 0x42, 0xe0, 0x0d, 0xa9, 0x18,
  0x28, 0x3f, 0x60, 0x0d, 0x41, 0x80, 0x41, 0xad,
  0xb7, 0xa0, 0x2f, 0x01, 0xe9, 0x7b, 0xdf, 0x01,
  0x00, 0x00, 0x00, 0x01, 0x28, 0xce, 0x09, 0x88,
  0x00, 0x00, 0x00, 0x01, 0x06, 0x00, 0x07, 0x81,
  0xf6, 0x3b, 0x80, 0x00, 0x00, 0x40, 0x80, 0x00,
  0x00, 0x00, 0x01, 0x25, 0xb8, 0x20, 0x00, 0xf9,
  0x31, 0x40, 0x00, 0x52, 0xea, 0xfb, 0xef, 0xbe,
  0xfb, 0xef, 0xbe, 0xfb, 0xef, 0xbe, 0xfb, 0xef,
  0xbe, 0xfb, 0xef, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xaf, 0xff, 0xf9, 0xf0, 0xa0,
  0x43, 0x80, 0x91, 0xeb, 0x7e, 0x5a, 0xbb, 0x9b,
  0xf3, 0xf0, 0x00, 0x00, 0x00, 0x01, 0x25, 0x01,
  0x2e, 0xe0, 0x80, 0x07, 0xff, 0xff, 0xe1, 0x38,
  0xa0, 0x00, 0x20, 0xb1, 0x80, 0x87, 0xde, 0xfc,
  0xf7, 0xb5, 0xb3, 0x73, 0xf3, 0xc3, 0xb8, 0x46,
  0xfb, 0xef, 0xbe, 0xfb, 0xef, 0xbe, 0x4c, 0x50,
  0x00, 0x14, 0xba, 0xbe, 0xfb, 0xef, 0xbe, 0xfb,
  0xef, 0xbe, 0xfa, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae,
  0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb,
  0xae, 0xba, 0xeb, 0xae, 0xba, 0xeb, 0xae, 0xba,
  0xeb, 0xae, 0xba, 0xf0};
const uint8_t kExpectedHeader[] = {
  0x00, 0x00, 0x01, 0xba, 0x44, 0x00, 0x04, 0x00,
  0x04, 0x01, 0x00, 0x01, 0x7b, 0xf8};
const uint8_t kSpsPps[] = {
  0x00, 0x00, 0x00, 0x16, 0x27, 0x42, 0xe0, 0x0d,
  0xa9, 0x18, 0x28, 0x3f, 0x60, 0x0d, 0x41, 0x80,
  0x41, 0xad, 0xb7, 0xa0, 0x2f, 0x01, 0xe9, 0x7b,
  0xdf, 0x01, 0x00, 0x00, 0x00, 0x04, 0x28, 0xce,
  0x09, 0x88};
const uint8_t kAudioOid[] = {
  0x1d, 0x34, 0x10, 0x01, 0x02, 0x80, 0x80, 0x2e,
  0x00, 0x4f, 0xff, 0xff, 0xfe, 0xfe, 0xff, 0x03,
  0x80, 0x80, 0x80, 0x22, 0x00, 0xc0, 0x00, 0x04,
  0x80, 0x80, 0x80, 0x14, 0x40, 0x15, 0x00, 0x18,
  0x00, 0x00, 0x01, 0xf4, 0x00, 0x00, 0x01, 0xf4,
  0x00, 0x05, 0x80, 0x80, 0x80, 0x02, 0x12, 0x10,
  0x06, 0x80, 0x80, 0x80, 0x01, 0x02};
}  // namespace

using std::make_pair;
namespace dash2hls {

class ProgramStreamOutTest : public ProgramStreamOut {
 public:
  using ProgramStreamOut::PreprocessNalus;
  using ProgramStreamOut::AddNeededNalu;
  using ProgramStreamOut::ConvertLengthToStartCode;
  using ProgramStreamOut::GetHeaderSize;
  using ProgramStreamOut::WriteHeader;
  using ProgramStreamOut::AddHeaders;
};

TEST(ProgramStreamOut, InternalRoutines) {
  std::vector<uint8_t> buffer(kInputSample,
                              kInputSample + sizeof(kInputSample));
  ProgramStreamOutTest ps_out;
  std::vector<uint8_t> sps_pps(kSpsPps, kSpsPps + sizeof(kSpsPps));
  ps_out.set_nalu_length(kNaluLength);
  ps_out.set_sps_pps(sps_pps);

  bool has_aud;
  nalu::PicType pic_type;
  ps_out.PreprocessNalus(&buffer, &has_aud, &pic_type);
  EXPECT_THAT(make_pair(&buffer[0], buffer.size()),
              testing::MemEq(kInputSample, sizeof(kInputSample)));
  EXPECT_EQ(false, has_aud);
  EXPECT_EQ(nalu::kPicType_I, pic_type);

  ps_out.AddNeededNalu(&buffer, pic_type, true);
  EXPECT_THAT(make_pair(&buffer[0], buffer.size()),
              testing::MemEq(kExpectedWithSpsPpsAud,
                             sizeof(kExpectedWithSpsPpsAud)));

  ps_out.ConvertLengthToStartCode(&buffer);
  EXPECT_THAT(make_pair(&buffer[0], buffer.size()),
              testing::MemEq(kExpectedAfterFixingNalu,
                             sizeof(kExpectedAfterFixingNalu)));

  ps_out.set_sps_pps(std::vector<uint8_t>(kSpsPps,
                                          kSpsPps + sizeof(kSpsPps)));
  ps_out.set_audio_oid(std::vector<uint8_t>(kAudioOid,
                                            kAudioOid + sizeof(kAudioOid)));
  std::vector<uint8_t> header;
  ASSERT_EQ(sizeof(kExpectedHeader), ps_out.GetHeaderSize());
  header.resize(ps_out.GetHeaderSize());
  ps_out.WriteHeader(&header[0], kScrDefaultValue, kDefaultMuxRate);
  EXPECT_THAT(make_pair(&header[0], header.size()),
              testing::MemEq(kExpectedHeader, sizeof(kExpectedHeader)));

  std::vector<uint8_t> final_buffer;
  ps_out.set_has_audio(true);
  ps_out.set_has_video(true);
  PES pes;
  pes.set_stream_id(PES::kVideoStreamId);
  pes.SetDataAlignmentIndicator(true);
  pes.SetPts(kDefaultPts);
  pes.AddPayload(&buffer[0], buffer.size());
  ps_out.AddHeaders(pes, true, kDefaultDuration, kDefaultScr, &final_buffer);
  EXPECT_THAT(make_pair(&final_buffer[0], final_buffer.size()),
              testing::MemEq(kExpectedOutput, sizeof(kExpectedOutput)));
}

TEST(ProgramStreamOut, ProcessPacket) {
  std::vector<uint8_t> output;
  std::vector<uint8_t> sps_pps(kSpsPps, kSpsPps + sizeof(kSpsPps));

  ProgramStreamOutTest ps_out;
  ps_out.set_nalu_length(kNaluLength);
  ps_out.set_sps_pps(sps_pps);
  ps_out.set_audio_oid(std::vector<uint8_t>(kAudioOid,
                                            kAudioOid + sizeof(kAudioOid)));
  ps_out.set_has_audio(true);
  ps_out.set_has_video(true);
  ps_out.ProcessSample(kInputSample, sizeof(kInputSample), true, true,
                       kDefaultPts, kDefaultDts, kDefaultScr, kDefaultDuration,
                       &output);
  EXPECT_THAT(make_pair(&output[0], output.size()),
              testing::MemEq(kExpectedOutput, sizeof(kExpectedOutput)));

  ps_out.ProcessSample(kInputSample2, sizeof(kInputSample2), true, false,
                       kDefaultPts + kDefaultDuration,
                       kDefaultDts + kDefaultDuration,
                       kDefaultScr + kDefaultDuration,
                       kDefaultDuration, &output);
  EXPECT_THAT(make_pair(&output[0], output.size()),
              testing::MemEq(kExpectedOutput2, sizeof(kExpectedOutput2)));
}

}  // namespace dash2hls
