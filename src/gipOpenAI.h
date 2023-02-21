/*
 * gipOpenAI.h
 *
 *  Created on: 14 Feb 2023
 *      Author: Noyan
 */

#ifndef SRC_GIPOPENAI_H_
#define SRC_GIPOPENAI_H_

#include "gBaseComponent.h"
#include "openai.hpp"


class gipOpenAI : public gBaseComponent {
public:

	enum {
		MODEL_TEXT_DAVINCI, MODEL_TEXT_CURIE, MODEL_TEXT_BABBAGE, MODEL_TEXT_ADA,
		MODEL_TEXTEDIT_DAVINCI,
		MODEL_TEXTEMBED_ADA,
		MODEL_CODE_DAVINCI, MODEL_CODE_CUSHMAN
	};

	typedef nlohmann::json Json;

	gipOpenAI();
	virtual ~gipOpenAI();

	void initialize(std::string apiKey);

	std::string getTextCompletion(std::string prompt, int maxTokens, int modelType = MODEL_TEXT_DAVINCI, int temperature = 0);
	std::string getTextEdit(std::string input, std::string instruction, int modelType = MODEL_TEXTEDIT_DAVINCI);
	std::string getTextEmbed(std::string input, int modelType = MODEL_TEXTEMBED_ADA);

	std::vector<std::string> getImage(std::string prompt, int num = 1, std::string size = "256x256");
	std::vector<std::string> getImageVariation(std::string imageFullPath, int num = 1, std::string size = "256x256");

	std::string getCode(std::string prompt, int maxTokens, int modelType = MODEL_CODE_DAVINCI);


private:
	std::string apikey;
	std::string modeltype[8];
};

#endif /* SRC_GIPOPENAI_H_ */
