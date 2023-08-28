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

class gipOpenAI: public gBaseComponent {
public:

	enum {
		MODEL_CHAT_GPT40,
		MODEL_CHAT_GPT35TURBO,
		MODEL_TEXT_DAVINCI,
		MODEL_TEXT_CURIE,
		MODEL_TEXT_BABBAGE,
		MODEL_TEXT_ADA,
		MODEL_TEXTEDIT_DAVINCI,
		MODEL_TEXTEMBED_ADA,
		MODEL_CODE_DAVINCI,
		MODEL_CODE_CUSHMAN
	};

	typedef nlohmann::json Json;

	gipOpenAI();
	virtual ~gipOpenAI();

	void initialize(std::string apiKey);

	std::string getChatCompletion(std::string prompt, int maxTokens, int modelType = MODEL_CHAT_GPT35TURBO, int temperature = 0);

	std::string getTextCompletion(std::string prompt, int maxTokens, int modelType = MODEL_TEXT_DAVINCI, int temperature = 0);
	std::string getTextEdit(std::string input, std::string instruction, int modelType = MODEL_TEXTEDIT_DAVINCI);
	std::string getTextEmbed(std::string input, int modelType = MODEL_TEXTEMBED_ADA);

	std::vector<std::string> getImage(std::string prompt, int num = 1, std::string size = "256x256");
	std::vector<std::string> getImageVariation(std::string imageFullPath, int num = 1, std::string size = "256x256");

	std::string getCode(std::string prompt, int maxTokens, int modelType = MODEL_CODE_DAVINCI);

	/*
	 * Upload Json file data for FineTune Model.
	 *
	 * @param jsonFile
	 * @return uploadedfileid
	 */
	std::string uploadFineTuneDataJsonFile(std::string jsonFile);

	/*
	 * Delete Json file
	 *
	 * @param uploadedfileid
	 */
	void deleteFile(std::string uploadedfileid);

	/*
	 * Fine tunes a model using the specified file.
	 *
	 * @param file The file to be used for fine tuning.
	 * @return The result of the fine tuning process.
	 */
	std::string fineTuneModel(std::string file);
	std::string reFineTuneModel(std::string file, std::string modelName);

	/*
	 * Retrieves the content of a fine-tuned model with a given ID.
	 *
	 * @param fineTuneId The ID of the fine-tuned model.
	 * @return A Json object containing the content of the fine-tuned model.
	 */
	Json retrieveFineTunedModelContent(std::string fineTuneId);

	Json getRetrieve(std::string fineTuneId);

	/*
	 * Get a completion of a given prompt using a specific model type.
	 *
	 * @param prompt The initial string to which the model should respond.
	 * @param maxTokens The maximum length of the model's response.
	 * @param temperature (Optional) The randomness of the model's output. Default is 0.
	 * @return The generated completion as a string.
	 */
	std::string getFineTunedCompletion(std::string prompt, int maxTokens, int temperature = 0);


private:
	std::string apikey;
	std::string modeltype[10];
	std::string uploadedfileid;
	Json retrieve;
};

#endif /* SRC_GIPOPENAI_H_ */
