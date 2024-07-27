/*  eq.h

This file is part of a program that implements a Software-Defined Radio.

Copyright (C) 2013, 2016 Warren Pratt, NR0V
Copyright (C) 2024 Edouard Griffiths, F4EXB Adapted to SDRangel

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

The author can be reached by email at

warren@wpratt.com

*/

/********************************************************************************************************
*                                                                                                       *
*                                   Partitioned Overlap-Save Equalizer                                  *
*                                                                                                       *
********************************************************************************************************/

#ifndef wdsp_eqp_h
#define wdsp_eqp_h

#include "export.h"

namespace WDSP {

class FIRCORE;
class RXA;
class TXA;

class WDSP_API EQP
{
public:
    int run;
    int size;
    int nc;
    int mp;
    float* in;
    float* out;
    int nfreqs;
    float* F;
    float* G;
    int ctfmode;
    int wintype;
    double samplerate;
    FIRCORE *fircore;

    EQP(
        int run,
        int size,
        int nc,
        int mp,
        float *in,
        float *out,
        int nfreqs,
        float* F,
        float* G,
        int ctfmode,
        int wintype,
        int samplerate
    );
    ~EQP();

    void flush();
    void execute();
    void setBuffers(float* in, float* out);
    void setSamplerate(int rate);
    void setSize(int size);
    // Public properties
    void setRun(int run);
    void setNC(int nc);
    void setMP(int mp);
    void setProfile(int nfreqs, const float* F, const float* G);
    void setCtfmode(int mode);
    void setWintype(int wintype);
    void setGrphEQ(int *rxeq);
    void setGrphEQ10(int *rxeq);

    static float* eq_impulse (int N, int nfreqs, float* F, float* G, double samplerate, double scale, int ctfmode, int wintype);

private:
    static int fEQcompare (const void * a, const void * b);
};

} // namespace WDSP

#endif
