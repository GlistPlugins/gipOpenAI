/*
 * gipOpenAI.cpp
 *
 *  Created on: 14 Feb 2023
 *      Author: Noyan
 */

#include "gipOpenAI.h"


gipOpenAI::gipOpenAI() {
	apikey = "";
	modeltype[0] = "text-davinci-003";
	modeltype[1] = "text-curie-001";
	modeltype[2] = "text-babbage-001";
	modeltype[3] = "text-ada-001";
	modeltype[4] = "code-davinci-002";
	modeltype[5] = "code-cushman-001";
}

gipOpenAI::~gipOpenAI() {
}

void gipOpenAI::initialize(std::string apiKey) {
	apikey = apiKey;
	openai::start(apikey);
}

std::string gipOpenAI::getCompletion(std::string prompt, int maxTokens, int modelType, int temperature) {
	Json j = {
			{"model", modeltype[modelType]},
			{"prompt", prompt},
			{"max_tokens", maxTokens},
			{"temperature", temperature}
	};
	auto completion = openai::completion().create(j);

	std::cout << "Response is:\n" << completion.dump(2) << '\n';
	return completion.dump(2);
}

std::vector<std::string> gipOpenAI::getImage(std::string prompt, int num, std::string size) {
	Json j = {
			{"prompt", prompt},
			{"n", num},
			{"size", size}
	};
    auto image = openai::image().create(j); // Using initializer lists
    std::cout << "Image URL is: " << image.dump(2) << '\n';
    std::vector<std::string> urls;
    for(int i = 0; i < num; i++) urls.push_back(image["data"][i]["url"]);
    return urls;
}

#include "gImage.h"
#include "gFile.h"

std::vector<std::string> gipOpenAI::getImageVariation(std::string fullPath, int num, std::string size) {
	gImage imagefile;
	imagefile.load(fullPath);
	char* fd = (char*)imagefile.getImageData();
//	std::string imgdata(reinterpret_cast<char*>(imagefile.getImageData()));
	std::string imgdata = std::string(fd);
	std::wstring wimgdata = (std::wstring)imgdata;

//	gFile ifile;
//	ifile.load(fullPath, gFile::FILEMODE_READONLY, true);
	Json j = {
			{"image", wimgdata},
			{"n", num},
			{"size", size}
	};

    auto image = openai::image().variation(j);
    std::cout << "Image URL is: " << image.dump(2) << '\n';
    std::vector<std::string> urls;
    for(int i = 0; i < num; i++) urls.push_back(image["data"][i]["url"]);
    return urls;
}
