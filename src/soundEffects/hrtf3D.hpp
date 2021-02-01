//
//  hrtf3D.hpp
//  oFHRTF
//
//  Created by keita miyano on 2021/02/01.
//

#pragma once

#include "ofMain.h"


class hrtf3D {

    public:
        vector<float> buffer;
        int size;
        int sampleRate;
        int originalSamplePoint;
        vector<vector<float>> hrtfValuesL;
        vector<vector<float>> hrtfValuesR;
        vector<int> elevs;
        vector<int> azimuths;
        hrtf3D(int size, int sampleRate);
        ~hrtf3D();
        void feed(float sample);
        void getSample(float& sampleL, float& sampleR, int index);
        int getElev(int index);
        int getAzimuth(int index);
    
    private:
        vector<float> splitString(string& input, char delimiter);

};
