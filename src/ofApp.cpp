#include "ofApp.h"

#define BUFFERSIZE 256; // バッファサイズ(256推奨．大きくすると処理に余裕はでるが遅延が長くなる)
#define SAMPLERATE 44100; // サンプルレート(Hz)

//--------------------------------------------------------------
void ofApp::setup(){
    
    // audio setting
    bufferSize = BUFFERSIZE;
    sampleRate = SAMPLERATE;
    azimuth = 0;
    
    inputBuffer.resize(bufferSize);
    ofSoundStreamSettings soundSettings;
    soundSettings.setOutListener(this);
    soundSettings.setInListener(this);
    soundSettings.sampleRate = sampleRate;
    soundSettings.numOutputChannels=2;
    soundSettings.numInputChannels=1;
    soundSettings.bufferSize = bufferSize;
    sound_stream.setup(soundSettings);
    
    
    myHrtf = new hrtf(512, sampleRate);
    hrtfOn = true;
    
    // 立方体
    soundSource.set(50);
    soundSource.setPosition(0, 0, 0);
    // 球
    microphone.set(50, 16);
    microphone.setPosition(200, 0, 0);
    // カメラ
    const float cameraPositionX = - 200;
    const float cameraPositionY = 200;
    const float cameraPositionZ = 200;
    cam.setPosition(cameraPositionX, cameraPositionY, cameraPositionZ);
    cam.lookAt(ofVec3f(-cameraPositionX, -cameraPositionY, -cameraPositionZ), ofVec3f(0, 0, 1));
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    // 座標系を移動
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    cam.begin();
    ofSetColor(255);
    // 立方体
    soundSource.draw();
    // 球
    microphone.drawWireframe();
    // 円(球の軌道)
    ofNoFill();
    ofDrawCircle(0, 0, 0, 200);
    cam.end();
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer &buffer){
    const int frames = buffer.getNumFrames();
    for(int i = 0; i < frames; i++){
        inputBuffer[i] = buffer[i];
    }
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &buffer){
    const int frames = buffer.getNumFrames();
    azimuth += 0.5;
    if(azimuth>360){azimuth=0;}
    for(int i = 0; i < frames; i++){
        const int channels = buffer.getNumChannels();
        float currentSample = inputBuffer[i];
        
        float currentSampleL = currentSample;
        float currentSampleR = currentSample;
        if (hrtfOn) {
            // inputの音をそのまま保持用バッファにいれる
            myHrtf->feed(currentSample);
            myHrtf->getSample(currentSampleL, currentSampleR, (int)azimuth);
        }
        buffer[i*channels+0] = currentSampleL;
        buffer[i*channels+1] = currentSampleR;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
    case 'h':
        if (hrtfOn) {
            hrtfOn = false;
        } else {
            hrtfOn = true;
        }
        break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit(){
    ofSoundStreamClose();
}
