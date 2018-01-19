#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofEnableDepthTest();
	ofBackground(255);
	ofSetWindowTitle("Insta");

	ofSetRectMode(ofRectMode::OF_RECTMODE_CENTER);

	this->cap.open(1);

	this->blend_image.allocate(this->cap.get(CV_CAP_PROP_FRAME_WIDTH), this->cap.get(CV_CAP_PROP_FRAME_HEIGHT), OF_IMAGE_COLOR);
	this->blend_frame = cv::Mat(this->blend_image.getHeight(), this->blend_image.getWidth(), CV_MAKETYPE(CV_8UC3, this->blend_image.getPixels().getNumChannels()), this->blend_image.getPixels().getData(), 0);

	this->wipe_image.allocate(this->cap.get(CV_CAP_PROP_FRAME_WIDTH) * 0.3, this->cap.get(CV_CAP_PROP_FRAME_HEIGHT) * 0.3, OF_IMAGE_COLOR);
	this->wipe_frame = cv::Mat(this->wipe_image.getHeight(), this->wipe_image.getWidth(), CV_MAKETYPE(CV_8UC3, this->wipe_image.getPixels().getNumChannels()), this->wipe_image.getPixels().getData(), 0);
}

//--------------------------------------------------------------
void ofApp::update() {
	cv::Mat src;
	this->cap >> src;
	cv::cvtColor(src, src, cv::COLOR_BGR2RGB);
	cv::flip(src, src, 1);

	if (ofGetFrameNum() < this->frame_number) {
		for (int i = ofGetFrameNum() - 1; i > 0; i--) {
			this->save_frame[i - 1].copyTo(this->save_frame[i]);
		}
		src.copyTo(this->save_frame[0]);
	}
	else {
		for (int i = this->frame_number - 1; i > 0; i--) {
			this->save_frame[i - 1].copyTo(this->save_frame[i]);
		}
		src.copyTo(this->save_frame[0]);
	}

	if(ofGetFrameNum() > this->frame_number){

		for (int y = 0; y < this->blend_frame.rows; y += 1) {
			for (int x = 0; x < this->blend_frame.cols; x += 1) {
				int index = ofNoise(x * 0.002, y * 0.002, ofGetFrameNum() * 0.05) * 120;
				this->blend_frame.at<cv::Vec3b>(y, x) = this->save_frame[index].at<cv::Vec3b>(y, x);
			}
		}

		this->blend_image.update();

	}

	cv::resize(src, this->wipe_frame, cv::Size(), 0.3, 0.3);
	this->wipe_image.update();

	ofSetWindowTitle(std::to_string(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw() {
	this->wipe_image.draw(ofGetWidth() / 2, ofGetHeight() / 2 + 250);

	if (ofGetFrameNum() > 120) {
		this->blend_image.draw(ofGetWidth() / 2, ofGetHeight() / 2 - 100);
	}
}

//========================================================================
int main() {
	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}