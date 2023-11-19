///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2023 Edouard Griffiths, F4EXB <f4exb06@gmail.com>               //
//                                                                               //
// This is the code from ft8mon: https://github.com/rtmrtmrtmrtm/ft8mon          //
// reformatted and adapted to Qt and SDRangel context                            //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
// (at your option) any later version.                                           //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

namespace FT8 {

//
// this is the LDPC(174,91) parity check matrix.
// each row describes one parity check.
// 83 rows.
// each number is an index into the codeword (1-origin).
// the codeword bits mentioned in each row must xor to zero.
// From WSJT-X's ldpc_174_91_c_reordered_parity.f90
//
int Nm[][7] = {
 {  4,  31,  59,  91,  92,  96, 153 },
 {  5,  32,  60,  93, 115, 146,   0 },
 {  6,  24,  61,  94, 122, 151,   0 },
 {  7,  33,  62,  95,  96, 143,   0 },
 {  8,  25,  63,  83,  93,  96, 148 },
 {  6,  32,  64,  97, 126, 138,   0 },
 {  5,  34,  65,  78,  98, 107, 154 },
 {  9,  35,  66,  99, 139, 146,   0 },
 { 10,  36,  67, 100, 107, 126,   0 },
 { 11,  37,  67,  87, 101, 139, 158 },
 { 12,  38,  68, 102, 105, 155,   0 },
 { 13,  39,  69, 103, 149, 162,   0 },
 {  8,  40,  70,  82, 104, 114, 145 },
 { 14,  41,  71,  88, 102, 123, 156 },
 { 15,  42,  59, 106, 123, 159,   0 },
 {  1,  33,  72, 106, 107, 157,   0 },
 { 16,  43,  73, 108, 141, 160,   0 },
 { 17,  37,  74,  81, 109, 131, 154 },
 { 11,  44,  75, 110, 121, 166,   0 },
 { 45,  55,  64, 111, 130, 161, 173 },
 {  8,  46,  71, 112, 119, 166,   0 },
 { 18,  36,  76,  89, 113, 114, 143 },
 { 19,  38,  77, 104, 116, 163,   0 },
 { 20,  47,  70,  92, 138, 165,   0 },
 {  2,  48,  74, 113, 128, 160,   0 },
 { 21,  45,  78,  83, 117, 121, 151 },
 { 22,  47,  58, 118, 127, 164,   0 },
 { 16,  39,  62, 112, 134, 158,   0 },
 { 23,  43,  79, 120, 131, 145,   0 },
 { 19,  35,  59,  73, 110, 125, 161 },
 { 20,  36,  63,  94, 136, 161,   0 },
 { 14,  31,  79,  98, 132, 164,   0 },
 {  3,  44,  80, 124, 127, 169,   0 },
 { 19,  46,  81, 117, 135, 167,   0 },
 {  7,  49,  58,  90, 100, 105, 168 },
 { 12,  50,  61, 118, 119, 144,   0 },
 { 13,  51,  64, 114, 118, 157,   0 },
 { 24,  52,  76, 129, 148, 149,   0 },
 { 25,  53,  69,  90, 101, 130, 156 },
 { 20,  46,  65,  80, 120, 140, 170 },
 { 21,  54,  77, 100, 140, 171,   0 },
 { 35,  82, 133, 142, 171, 174,   0 },
 { 14,  30,  83, 113, 125, 170,   0 },
 {  4,  29,  68, 120, 134, 173,   0 },
 {  1,   4,  52,  57,  86, 136, 152 },
 { 26,  51,  56,  91, 122, 137, 168 },
 { 52,  84, 110, 115, 145, 168,   0 },
 {  7,  50,  81,  99, 132, 173,   0 },
 { 23,  55,  67,  95, 172, 174,   0 },
 { 26,  41,  77, 109, 141, 148,   0 },
 {  2,  27,  41,  61,  62, 115, 133 },
 { 27,  40,  56, 124, 125, 126,   0 },
 { 18,  49,  55, 124, 141, 167,   0 },
 {  6,  33,  85, 108, 116, 156,   0 },
 { 28,  48,  70,  85, 105, 129, 158 },
 {  9,  54,  63, 131, 147, 155,   0 },
 { 22,  53,  68, 109, 121, 174,   0 },
 {  3,  13,  48,  78,  95, 123,   0 },
 { 31,  69, 133, 150, 155, 169,   0 },
 { 12,  43,  66,  89,  97, 135, 159 },
 {  5,  39,  75, 102, 136, 167,   0 },
 {  2,  54,  86, 101, 135, 164,   0 },
 { 15,  56,  87, 108, 119, 171,   0 },
 { 10,  44,  82,  91, 111, 144, 149 },
 { 23,  34,  71,  94, 127, 153,   0 },
 { 11,  49,  88,  92, 142, 157,   0 },
 { 29,  34,  87,  97, 147, 162,   0 },
 { 30,  50,  60,  86, 137, 142, 162 },
 { 10,  53,  66,  84, 112, 128, 165 },
 { 22,  57,  85,  93, 140, 159,   0 },
 { 28,  32,  72, 103, 132, 166,   0 },
 { 28,  29,  84,  88, 117, 143, 150 },
 {  1,  26,  45,  80, 128, 147,   0 },
 { 17,  27,  89, 103, 116, 153,   0 },
 { 51,  57,  98, 163, 165, 172,   0 },
 { 21,  37,  73, 138, 152, 169,   0 },
 { 16,  47,  76, 130, 137, 154,   0 },
 {  3,  24,  30,  72, 104, 139,   0 },
 {  9,  40,  90, 106, 134, 151,   0 },
 { 15,  58,  60,  74, 111, 150, 163 },
 { 18,  42,  79, 144, 146, 152,   0 },
 { 25,  38,  65,  99, 122, 160,   0 },
 { 17,  42,  75, 129, 170, 172,   0 },
};

// Mn from WSJT-X's ldpc_174_91_c_reordered_parity.f90
// each of the 174 rows corresponds to a codeword bit.
// the numbers indicate which three parity
// checks (rows in Nm) refer to the codeword bit.
// 1-origin.
int Mn[][3] = {
  {  16,  45,  73 },
  {  25,  51,  62 },
  {  33,  58,  78 },
  {   1,  44,  45 },
  {   2,   7,  61 },
  {   3,   6,  54 },
  {   4,  35,  48 },
  {   5,  13,  21 },
  {   8,  56,  79 },
  {   9,  64,  69 },
  {  10,  19,  66 },
  {  11,  36,  60 },
  {  12,  37,  58 },
  {  14,  32,  43 },
  {  15,  63,  80 },
  {  17,  28,  77 },
  {  18,  74,  83 },
  {  22,  53,  81 },
  {  23,  30,  34 },
  {  24,  31,  40 },
  {  26,  41,  76 },
  {  27,  57,  70 },
  {  29,  49,  65 },
  {   3,  38,  78 },
  {   5,  39,  82 },
  {  46,  50,  73 },
  {  51,  52,  74 },
  {  55,  71,  72 },
  {  44,  67,  72 },
  {  43,  68,  78 },
  {   1,  32,  59 },
  {   2,   6,  71 },
  {   4,  16,  54 },
  {   7,  65,  67 },
  {   8,  30,  42 },
  {   9,  22,  31 },
  {  10,  18,  76 },
  {  11,  23,  82 },
  {  12,  28,  61 },
  {  13,  52,  79 },
  {  14,  50,  51 },
  {  15,  81,  83 },
  {  17,  29,  60 },
  {  19,  33,  64 },
  {  20,  26,  73 },
  {  21,  34,  40 },
  {  24,  27,  77 },
  {  25,  55,  58 },
  {  35,  53,  66 },
  {  36,  48,  68 },
  {  37,  46,  75 },
  {  38,  45,  47 },
  {  39,  57,  69 },
  {  41,  56,  62 },
  {  20,  49,  53 },
  {  46,  52,  63 },
  {  45,  70,  75 },
  {  27,  35,  80 },
  {   1,  15,  30 },
  {   2,  68,  80 },
  {   3,  36,  51 },
  {   4,  28,  51 },
  {   5,  31,  56 },
  {   6,  20,  37 },
  {   7,  40,  82 },
  {   8,  60,  69 },
  {   9,  10,  49 },
  {  11,  44,  57 },
  {  12,  39,  59 },
  {  13,  24,  55 },
  {  14,  21,  65 },
  {  16,  71,  78 },
  {  17,  30,  76 },
  {  18,  25,  80 },
  {  19,  61,  83 },
  {  22,  38,  77 },
  {  23,  41,  50 },
  {   7,  26,  58 },
  {  29,  32,  81 },
  {  33,  40,  73 },
  {  18,  34,  48 },
  {  13,  42,  64 },
  {   5,  26,  43 },
  {  47,  69,  72 },
  {  54,  55,  70 },
  {  45,  62,  68 },
  {  10,  63,  67 },
  {  14,  66,  72 },
  {  22,  60,  74 },
  {  35,  39,  79 },
  {   1,  46,  64 },
  {   1,  24,  66 },
  {   2,   5,  70 },
  {   3,  31,  65 },
  {   4,  49,  58 },
  {   1,   4,   5 },
  {   6,  60,  67 },
  {   7,  32,  75 },
  {   8,  48,  82 },
  {   9,  35,  41 },
  {  10,  39,  62 },
  {  11,  14,  61 },
  {  12,  71,  74 },
  {  13,  23,  78 },
  {  11,  35,  55 },
  {  15,  16,  79 },
  {   7,   9,  16 },
  {  17,  54,  63 },
  {  18,  50,  57 },
  {  19,  30,  47 },
  {  20,  64,  80 },
  {  21,  28,  69 },
  {  22,  25,  43 },
  {  13,  22,  37 },
  {   2,  47,  51 },
  {  23,  54,  74 },
  {  26,  34,  72 },
  {  27,  36,  37 },
  {  21,  36,  63 },
  {  29,  40,  44 },
  {  19,  26,  57 },
  {   3,  46,  82 },
  {  14,  15,  58 },
  {  33,  52,  53 },
  {  30,  43,  52 },
  {   6,   9,  52 },
  {  27,  33,  65 },
  {  25,  69,  73 },
  {  38,  55,  83 },
  {  20,  39,  77 },
  {  18,  29,  56 },
  {  32,  48,  71 },
  {  42,  51,  59 },
  {  28,  44,  79 },
  {  34,  60,  62 },
  {  31,  45,  61 },
  {  46,  68,  77 },
  {   6,  24,  76 },
  {   8,  10,  78 },
  {  40,  41,  70 },
  {  17,  50,  53 },
  {  42,  66,  68 },
  {   4,  22,  72 },
  {  36,  64,  81 },
  {  13,  29,  47 },
  {   2,   8,  81 },
  {  56,  67,  73 },
  {   5,  38,  50 },
  {  12,  38,  64 },
  {  59,  72,  80 },
  {   3,  26,  79 },
  {  45,  76,  81 },
  {   1,  65,  74 },
  {   7,  18,  77 },
  {  11,  56,  59 },
  {  14,  39,  54 },
  {  16,  37,  66 },
  {  10,  28,  55 },
  {  15,  60,  70 },
  {  17,  25,  82 },
  {  20,  30,  31 },
  {  12,  67,  68 },
  {  23,  75,  80 },
  {  27,  32,  62 },
  {  24,  69,  75 },
  {  19,  21,  71 },
  {  34,  53,  61 },
  {  35,  46,  47 },
  {  33,  59,  76 },
  {  40,  43,  83 },
  {  41,  42,  63 },
  {  49,  75,  83 },
  {  20,  44,  48 },
  {  42,  49,  57 },
};

} // namespace FT8
