#include <dirent.h>
#include <stdio.h>

#include "Explorer.h"

ExplorerItem::ExplorerItem(const std::string path, const std::string fileName)
{
	this->fileName = fileName;
	this->path = path;
}

void ExplorerItem::Play(OMXPlayer* player)
{
	if (player == nullptr) { return; }
	player->Play(path + fileName);
}

Explorer::Explorer()
{
}


Explorer::~Explorer()
{
}

void Explorer::Build(const std::string root)
{
	DIR *dir;
	struct dirent *dp;

	if ( (dir = opendir(root.c_str())) == NULL) {
		return;
	}
	for ( dp = readdir(dir); dp != nullptr; dp = readdir(dir) ) {
		//printf("%s\n", dp->d_name);
		std::string name = dp->d_name;
		if(name == "." || name == ".."){ continue; }
		items.push_back(std::shared_ptr<ExplorerItem>(new ExplorerItem(root, name)));
	}
	closedir(dir);
}