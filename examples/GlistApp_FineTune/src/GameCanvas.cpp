/*
 * GameCanvas.cpp
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */

#include "GameCanvas.h"

GameCanvas::GameCanvas(gApp *root) :
		gBaseCanvas(root) {
	this->root = root;
}

GameCanvas::~GameCanvas() {

}

void GameCanvas::setup() {
	root->setFramerate(1);
	status = false;
	try {
		std::string apikey = "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";
		ai.initialize(apikey);

		// Upload file
		uploadedfileid = ai.uploadFineTuneDataJsonFile("data.jsonl");
		fineTuneId = ai.fineTuneModel(uploadedfileid);
		std::cout << "Fine-tune ID: " << fineTuneId << std::endl;
	} catch (const std::exception &ex) {
		gLoge("GameCanvas") << ex.what();
	}

}

void GameCanvas::update() {
	if (status)
		return;
	try {
		gipOpenAI::Json retrieve = ai.getRetrieve(fineTuneId);
		std::cout << "Retrieved Fine-tuned Model Content: " << retrieve.dump(2) << std::endl;
		if (retrieve["status"] == "succeeded") {
			std::string chat_prompt = "what is Glistegine?";
			gLogi("GameCanvas2") << "Chat prompt:" << chat_prompt;
			std::string chat_response = ai.getFineTunedCompletion(chat_prompt, 20);
			gLogi("GameCanvas") << "Chat response:" << chat_response;
			// Delete file
			auto deletion = openai::file().del(uploadedfileid);
			std::cout << deletion.dump(2) << "\n\n";
		}
	} catch (const std::exception &ex) {

		gLoge("GameCanvas") << ex.what();
	}
}

void GameCanvas::draw() {
}

void GameCanvas::keyPressed(int key) {
	//	gLogi("GameCanvas") << "keyPressed:" << key;
}

void GameCanvas::keyReleased(int key) {
	//	gLogi("GameCanvas") << "keyReleased:" << key;
}

void GameCanvas::charPressed(unsigned int codepoint) {
	//	gLogi("GameCanvas") << "charPressed:" << gCodepointToStr(codepoint);
}

void GameCanvas::mouseMoved(int x, int y) {
	//	gLogi("GameCanvas") << "mouseMoved" << ", x:" << x << ", y:" << y;
}

void GameCanvas::mouseDragged(int x, int y, int button) {
	//	gLogi("GameCanvas") << "mouseDragged" << ", x:" << x << ", y:" << y << ", b:" << button;
}

void GameCanvas::mousePressed(int x, int y, int button) {
}

void GameCanvas::mouseReleased(int x, int y, int button) {
	//	gLogi("GameCanvas") << "mouseReleased" << ", button:" << button;
}

void GameCanvas::mouseScrolled(int x, int y) {
	//	gLogi("GameCanvas") << "mouseScrolled" << ", x:" << x << ", y:" << y;
}

void GameCanvas::mouseEntered() {
}

void GameCanvas::mouseExited() {
}

void GameCanvas::windowResized(int w, int h) {
}

void GameCanvas::showNotify() {

}

void GameCanvas::hideNotify() {

}

