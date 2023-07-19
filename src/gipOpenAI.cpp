/*
 * gipOpenAI.cpp
 *
 *  Created on: 14 Feb 2023
 *      Author: Noyan
 */

#include "gipOpenAI.h"
#include "gFile.h"
#include "gImage.h"


gipOpenAI::gipOpenAI() {
	apikey = "";
	modeltype[MODEL_CHAT_GPT40] = "gpt-4";
	modeltype[MODEL_CHAT_GPT35TURBO] = "gpt-3.5-turbo";
	modeltype[MODEL_TEXT_DAVINCI] = "text-davinci-003";
	modeltype[MODEL_TEXT_CURIE] = "text-curie-001";
	modeltype[MODEL_TEXT_BABBAGE] = "text-babbage-001";
	modeltype[MODEL_TEXT_ADA] = "text-ada-001";
	modeltype[MODEL_TEXTEDIT_DAVINCI] = "text-davinci-edit-001";
	modeltype[MODEL_TEXTEMBED_ADA] = "text-embedding-ada-002";
	modeltype[MODEL_CODE_DAVINCI] = "code-davinci-002";
	modeltype[MODEL_CODE_CUSHMAN] = "code-cushman-001";
}

gipOpenAI::~gipOpenAI() {
}

void gipOpenAI::initialize(std::string apiKey) {
	apikey = apiKey;
	openai::start(apikey);
}

std::string gipOpenAI::getChatCompletion(std::string prompt, int maxTokens, int modelType, int temperature) {
	Json messages = Json::array( {Json::object({{"role", "user"}, {"content", prompt}})} );
	Json j = {
			{"model", modeltype[modelType]},
			{"messages", messages},
			{"max_tokens", maxTokens},
			{"temperature", temperature}
	};
	auto completion = openai::chat().create(j);
//	std::cout << "Response is:\n" << completion.dump(2) << '\n';
	return completion["choices"][0]["message"]["content"];
}

std::string gipOpenAI::getTextCompletion(std::string prompt, int maxTokens, int modelType, int temperature) {
	Json j = {
			{"model", modeltype[modelType]},
			{"prompt", prompt},
			{"max_tokens", maxTokens},
			{"temperature", temperature}
	};
	auto completion = openai::completion().create(j);
//	std::cout << "Response is:\n" << completion.dump(2) << '\n';
	return completion["choices"][0]["text"];
}

std::string gipOpenAI::getTextEdit(std::string input, std::string instruction, int modelType) {
	Json j = {
			{"model", modeltype[modelType]},
			{"input", input},
			{"instruction", instruction}
	};
	auto edit = openai::edit().create(j);
//	std::cout << "Response is:\n" << edit.dump(2) << '\n';
	return edit["choices"][0]["text"];
}

std::string gipOpenAI::getTextEmbed(std::string input, int modelType) {
	Json j = {
			{"model", modeltype[modelType]},
			{"input", input}
	};
	auto res = openai::embedding().create(j);
	std::cout << "Response is:\n" << res.dump(2) << '\n';
	return res.dump(2);
}

std::vector<std::string> gipOpenAI::getImage(std::string prompt, int num, std::string size) {
	Json j = {
			{"prompt", prompt},
			{"n", num},
			{"size", size},
			{"response_format", "b64_json"}
	};
    auto image = openai::image().create(j); // Using initializer lists
    std::vector<std::string> data;
    for(int i = 0; i < num; i++) {
    	gFile ifile;
    	ifile.load(gImage::generateDownloadedImagePath(), gFile::FILEMODE_WRITEONLY, true);
    	ifile.write(gDecodeBase64(image["data"][i]["b64_json"]));
    	data.push_back(ifile.getPath().string());
    }
    return data;
}

std::vector<std::string> gipOpenAI::getImageVariation(std::string imageFullPath, int num, std::string size) {
	Json j = {
			{"image", imageFullPath},
			{"n", num},
			{"size", size},
			{"response_format", "b64_json"}
	};

    auto image = openai::image().variation(j);
//    std::cout << "Image response:\n" << image.dump(2) << '\n';

    std::vector<std::string> data;
    for(int i = 0; i < num; i++) {
    	gFile ifile;
    	ifile.load(gImage::generateDownloadedImagePath(), gFile::FILEMODE_WRITEONLY, true);
    	ifile.write(gDecodeBase64(image["data"][i]["b64_json"]));
    	data.push_back(ifile.getPath().string());
    	ifile.close();
    }
    return data;
}

std::string gipOpenAI::getCode(std::string prompt, int maxTokens, int modelType) {
	Json j = {
			{"model", modeltype[modelType]},
			{"max_tokens", maxTokens},
			{"prompt", prompt}
	};
	auto completion = openai::completion().create(j);
//	std::cout << "Response is:\n" << completion.dump(2) << '\n';
	return completion["choices"][0]["text"];
}

std::string gipOpenAI::fineTuneModel(std::string file) {
	auto fineTune = openai::fineTune().create( {
		{ "training_file", file },
		{"model", "davinci"},
		{"n_epochs", 15},
		{"batch_size", 3},
		{"learning_rate_multiplier", 0.3},
	});
	return fineTune["id"].get<std::string>();
}

gipOpenAI::Json gipOpenAI::retrieveFineTunedModelContent(std::string fineTuneId) {
	return openai::fineTune().retrieve(fineTuneId);
}

std::string gipOpenAI::getCompletion(std::string prompt, int maxTokens, std::string modelType, int temperature) {
    Json j = {
            {"model", modelType},
            {"prompt", prompt},
            {"max_tokens", maxTokens},
            {"temperature", temperature}
    };
    auto completion = openai::completion().create(j);
    std::cout << "Response is:\n" << completion.dump(2) << '\n';
    return completion["choices"][0]["text"];
}

std::string gipOpenAI::getFineTunedCompletion(gipOpenAI::Json& retrieve, std::string prompt, int maxTokens, int temperature) {
    Json j = {
            {"model", retrieve["fine_tuned_model"]},
            {"prompt", prompt},
            {"max_tokens", maxTokens},
            {"temperature", temperature}
    };
    auto completion = openai::completion().create(j);
    std::cout << "Response is:\n" << completion.dump(2) << '\n';
    return completion["choices"][0]["text"];
}
