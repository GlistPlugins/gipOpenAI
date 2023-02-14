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
		MODEL_CODE_DAVINCI, MODEL_CODE_CUSHMAN
	};

	typedef nlohmann::json Json;

	gipOpenAI();
	virtual ~gipOpenAI();

	void initialize(std::string apiKey);
	std::string getCompletion(std::string prompt, int maxTokens, int modelType = 0, int temperature = 0);
	std::vector<std::string> getImage(std::string prompt, int num = 1, std::string size = "256x256");
	std::vector<std::string> getImageVariation(std::string fullPath, int num = 1, std::string size = "256x256");

private:
	std::string apikey;
	std::string modeltype[6];
};

#endif /* SRC_GIPOPENAI_H_ */
