#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

using namespace std;

void showDir(const char * dir);

int main(int argc, char ** argv) {
    const char* dir;
    
    if (argc >= 2) {
        dir = argv[1];
    } else {
        dir = ".";
    }
    
    showDir(dir);
    
    return 0;
}

struct Dirent {
    string name;
    bool isDir;
    bool isLink;
};

bool  isDirectory(const char * dir) {
    struct stat st;
    return stat(dir, &st) == 0 && (st.st_mode & S_IFDIR) != 0;
}

bool sortDirents(Dirent* a, Dirent* b) {
    if (a->isDir != b->isDir) {
        return b->isDir;
    } 
    if (a->isLink != b->isLink) {
        return b->isLink;
    }
    return a->name < b->name;
}

vector<struct Dirent*> getContent(string dir) {
    vector<struct Dirent*> result;
    
	DIR* dirp = opendir(dir.c_str());
	while (dirp != NULL) {
		dirent* dp = readdir(dirp);
		if (dp == NULL) {
            break;
        }
        
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            struct Dirent* curDirent = new struct Dirent;
            curDirent->name = (string)dp->d_name;
            curDirent->isDir = isDirectory((dir + "/" + dp->d_name).c_str());
            curDirent->isLink = dp->d_type == DT_LNK;
            result.push_back(curDirent);
        }
	}
	closedir(dirp);
    
    sort(result.begin(), result.end(), sortDirents);
    return result;
}

void showName(string name, vector<bool>& isLastSection, bool isDirectory) {
    for (int i = 0; i < (int)isLastSection.size() - 1; i++) {
        std :: cout << (isLastSection[i] ? "    " : "│   ");
    }
    if (isLastSection.size() > 0) {
        std :: cout << (isLastSection.back() ? "└── " : "├── ");
    }
    
    if (isDirectory) { // blue color text
        std :: cout << "\033[1;34m";
    }
    std :: cout << name;
    if (isDirectory) { // end of blue color text
        std :: cout << "\033[0m";
    }
    cout << endl;
}

void showName(Dirent* dirent, vector<bool>& isLastSection) {
    showName(
        dirent->isLink ? dirent->name + " (link)" : dirent->name, 
        isLastSection,
        dirent->isDir
    );
}

void freeDirents(vector<Dirent*>& dirents) {
    for (int i = 0; i < (int)dirents.size(); i++) {
        if (dirents[i] != NULL) {
            delete dirents[i];
            dirents[i] = NULL;
        }
    }
}

void __showDir(string dir, vector<bool>& isLastSection) {
    vector<Dirent*> dirents = getContent(dir);
    
    isLastSection.push_back(false);
    
    for (int i = 0; i < (int)dirents.size(); i++) {
        isLastSection.back() = (i + 1) == (int)dirents.size();
        showName(dirents[i], isLastSection);
        if (dirents[i]->isDir) {
            __showDir(dir + "/" + dirents[i]->name, isLastSection);
        }
    }
    
    isLastSection.pop_back();
    freeDirents(dirents);
}

void showDir(const char * dir) {
    if (!isDirectory(dir)) {
        cout << "'" << dir << "' is not a directory\n";
        return;
    }
    
    vector<bool> isLastSection;
    showName(dir, isLastSection, true);
    __showDir((string)dir, isLastSection);
}
