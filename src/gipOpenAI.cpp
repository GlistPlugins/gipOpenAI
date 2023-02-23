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
	openai::start();
	setActiveInstance();
}

//void gipOpenAI::createNewInstance(openai::OpenAI& openai) {
//    // You can try catch API method if an error occur
//    try {
//        auto res = openai.completion.create({
//            {"model", "text-davinci-003"},
//            {"prompt", "Say bar() function called"}
//        });
//        std::cout << res << "\n\n";
//    }
//    catch(std::exception const& e) {
//        std::cerr << "Exception:" <<  e.what() << "\n\n";
//    }
//}
//

std::string gipOpenAI::createInstance(openai::OpenAI& openai, std::string prompt, int maxTokens, int modelType, int temperature) {
//	obje.getTextCompletion(prompt, 500);

	Json j = {
				{"model", modeltype[modelType]},
				{"prompt", prompt},
				{"max_tokens", maxTokens},
				{"temperature", temperature}
		};
		auto completion = openai.completion.create(j);
	//	std::cout << "Response is:\n" << completion.dump(2) << '\n';
//		std::cout << completion["choices"][0]["text"] << "\n";
		return completion["choices"][0]["text"];
}

std::string gipOpenAI::createNewInstance(openai::OpenAI& openai) {

    // You can try catch API method if an error occur
	Json j = {{"model", "text-davinci-003"},{"prompt", "say AAAAAAAA!!!!!! then give 5 random girl names "},{"max_tokens", 500}};
    try {
        auto completion = openai.completion.create(j);
        std::cout << completion["choices"][0]["text"] << "\n";
        return completion["choices"][0]["text"];
    }
    catch(std::exception const& e) {
        std::cerr << "Exception:" <<  e.what() << "\n\n";
    }
}

void gipOpenAI::setActiveInstance() {
	// Create openai instance
//	openai_instance = openai::OpenAI{"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"};
	openai::OpenAI openai_instance{"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"};
//	openai::OpenAI openai_instance{};
	createNewInstance(openai_instance);

	// You can create other openai instances with different tokens and parameters
	{
		openai::OpenAI another_openai_instance{"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"};
		// another_openai_instance.setThrowException(false); // You can set throw exception to false if you want

		try {
			auto completion = another_openai_instance.completion.create({
				{"model", "text-davinci-003"},
				{"prompt", "Say this should throw since token is invalid here"}
			});
//			std::cout << completion["choices"][0]["text"] << "\n";
		}
		catch(std::exception const& e) {
			std::cerr << "02-basic failed purposely because of " << e.what() << "\n\n";
		}

	}

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

std::string gipOpenAI::getCodeDeneme(openai::OpenAI& openai, std::string prompt, int maxTokens, int modelType) {
	Json j = {
			{"model", modeltype[modelType]},
			{"max_tokens", maxTokens},
			{"prompt", prompt}
	};
	auto completion = openai.completion.create(j);
//	std::cout << "Response is:\n" << completion.dump(2) << '\n';
	return completion["choices"][0]["text"];
}

