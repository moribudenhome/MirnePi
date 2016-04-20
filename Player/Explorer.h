#pragma once

#include <string>
#include <map>
#include <memory>
#include <vector>

#include "OmxPlayer.h"

class  ExplorerItem
{
public:
	ExplorerItem(const std::string path, const std::string fileName);
	~ExplorerItem() {};
public:
	void Play(OMXPlayer* player);
public:
	std::string fileName;
	std::string path;
};

class Explorer
{
public:
	Explorer();
	~Explorer();
public:
	void Build(const std::string root);
private:
public:
	std::vector<std::shared_ptr<ExplorerItem>> items;
};

