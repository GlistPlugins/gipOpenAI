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
	modeltype[0] = "text-davinci-003";
	modeltype[1] = "text-curie-001";
	modeltype[2] = "text-babbage-001";
	modeltype[3] = "text-ada-001";
	modeltype[4] = "text-davinci-edit-001";
	modeltype[5] = "text-embedding-ada-002";
	modeltype[6] = "code-davinci-002";
	modeltype[7] = "code-cushman-001";
}

gipOpenAI::~gipOpenAI() {
}

void gipOpenAI::initialize(std::string apiKey) {
	apikey = apiKey;
	openai::start(apikey);
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
	gFile ifileo;
	ifileo.load(imageFullPath, gFile::FILEMODE_READONLY, true);
	std::vector<char> ibytes = ifileo.getBytes();
//	gLogi("OAI") << "image fullpath:" << ifileo.getPath().string();
//	gLogi("OAI") << "bytes:" << gEncodeBase64(ibytes.data(), ibytes.size());
	Json j = {
			{"image", gEncodeBase64(ibytes.data(), ibytes.size())},
			{"n", num},
			{"size", size},
			{"response_format", "b64_json"}
	};
	ifileo.close();

    auto image = openai::image().variation(j);
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
