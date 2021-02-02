//
//  hrtf3D.cpp
//  oFHRTF
//
//  Created by keita miyano on 2021/02/01.
//

#include "soundEffects/hrtf3D.hpp"

hrtf3D::hrtf3D(int size, int sampleRate) {
    //! サンプルレート
    this->sampleRate = sampleRate;
    //! 保持されている音のサンプルサイズ(各HRTFデータに何サンプル分のデータがあるか)
    this->size = size;
    //! バッファ
    buffer.resize(size);
    for (int i = 0; i < buffer.size(); i++) {
        buffer[i] = 0.;
    }
    //! 元のsampleが保持用bufferの今どの位置か
    originalSamplePoint = 0;
    //! elevは -40, -20, 0, 20, 40, 60, 80 の 7個
    //! azimuthは 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330 の 11個
    hrtfValuesL.resize(77);
    hrtfValuesR.resize(77);
    elevs.resize(77);
    azimuths.resize(77);
    // HRTFのデータセット読み込み
    int i = 0;
    for (int elev=-40; elev<=80; elev+=20) {
        for (int azimuth=30; azimuth<=330; azimuth+=30) {
            std::ostringstream sout;
            sout << std::setfill('0') << std::setw(3) << azimuth;
            // hrtfデータベースのファイルパスを取得
            std::string filePathL = "hrtfs/elev" + to_string(elev) + "/L" + to_string(elev) + "e" + sout.str() + "a.dat";
            std::string filePathR = "hrtfs/elev" + to_string(elev) + "/R" + to_string(elev) + "e" + sout.str() + "a.dat";
            ofBuffer fileBufferL = ofBufferFromFile(filePathL);
            ofBuffer fileBufferR = ofBufferFromFile(filePathR);
            string _textL = fileBufferL.getText();
            hrtfValuesL[i] = splitString(_textL, '\n');
            string _textR = fileBufferR.getText();
            hrtfValuesR[i] = splitString(_textR, '\n');
            hrtfValuesL[i].resize(size);
            hrtfValuesR[i].resize(size);
            elevs[i] = elev;
            azimuths[i] = azimuth;
            // cout << "index: " << i << ", elev: " << elev << ", azimuth: " << azimuth << endl;
            i++;
        }
    }
    // メインの音として重ね合わせた音を使う
    mainHrtfValueL.resize(size);
    mainHrtfValueR.resize(size);
    std::string filePath30L = "hrtfs/elev" + to_string(0) + "/L0e" + "030" + "a.dat";
    std::string filePath30R = "hrtfs/elev" + to_string(0) + "/R0e" + "030" + "a.dat";
    std::string filePath330L = "hrtfs/elev" + to_string(0) + "/L0e" + "330" + "a.dat";
    std::string filePath330R = "hrtfs/elev" + to_string(0) + "/R0e" + "330" + "a.dat";
    ofBuffer fileBuffer30L = ofBufferFromFile(filePath30L);
    ofBuffer fileBuffer30R = ofBufferFromFile(filePath30R);
    ofBuffer fileBuffer330L = ofBufferFromFile(filePath330L);
    ofBuffer fileBuffer330R = ofBufferFromFile(filePath330R);
    string _text30L = fileBuffer30L.getText();
    string _text30R = fileBuffer30R.getText();
    mainHrtfValueL = splitString(_text30L, '\n');
    mainHrtfValueR = splitString(_text30R, '\n');
    string _text330L = fileBuffer330L.getText();
    string _text330R = fileBuffer330R.getText();
    vector<float> _v330L = splitString(_text330L, '\n');
    vector<float> _v330R = splitString(_text330R, '\n');
    for (int j=0; j<size; j++) {
        mainHrtfValueL[j] += _v330L[j];
        mainHrtfValueR[j] += _v330R[j];
    }
    
}

void hrtf3D::getSample(float& sampleL, float& sampleR, int index) {
    // indexは0~76の整数
    if (index < 0){ index = abs(index); }
    if (index > 76) { index %= 77; }
    float delayTime; // 単位はms
    int point;
    sampleL = 0.;
    sampleR = 0.;
    for (int i = 0; i < size; i++) {
        delayTime = i * 1000. / 44100.;
        point = originalSamplePoint + size - (delayTime*0.001*sampleRate) - 1;
        if(point >= size){point -= size;}
        sampleL += buffer[point] * hrtfValuesL[index][i] * 5.; // 音が原音に比べて小さすぎるので5倍する
        sampleR += buffer[point] * hrtfValuesR[index][i] * 5.; // 音が原音に比べて小さすぎるので5倍する
    }
}

void hrtf3D::getMainSample(float& sampleL, float& sampleR) {
    float delayTime; // 単位はms
    int point;
    sampleL = 0.;
    sampleR = 0.;
    for (int i = 0; i < size; i++) {
        delayTime = i * 1000. / 44100.;
        point = originalSamplePoint + size - (delayTime*0.001*sampleRate) - 1;
        if(point >= size){point -= size;}
        sampleL += buffer[point] * mainHrtfValueL[i] * 5.; // 音が原音に比べて小さすぎるので5倍する
        sampleR += buffer[point] * mainHrtfValueR[i] * 5.;
    }
}

// sampleを保持用バッファにいれていく
void hrtf3D::feed(float sample) {
    buffer[originalSamplePoint] = sample;
    originalSamplePoint++;
    // indexがバッファサイズを超えたら0に戻す
    if(originalSamplePoint >= size){ originalSamplePoint = 0;}
}

int hrtf3D::getElev(int index) {
    return elevs[index];
}

int hrtf3D::getAzimuth(int index) {
    return azimuths[index];
}

//文字列delimiterでspitしてvector配列を返すメソッド(文字列を全てfloatに変換する)
vector<float> hrtf3D::splitString(string& input, char delimiter) {
    istringstream stream(input);
    string field;
    vector<float> result;
    while (getline(stream, field, delimiter)) {
        result.push_back(stof(field));
    }
    return result;
}
