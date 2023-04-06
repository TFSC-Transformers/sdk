#ifndef __DEBUG__L
#define __DEBUG__L

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#define RED_t "\033[31m"
#define YELLOW_t "\033[33m"
#define GREEN_t "\033[32m"
#define WRITE "\033[0m"

#define errorL(msg) \
	std::cout << RED_t <<"Error:["<< __FILE__  << ":"<< __LINE__ << "]:"<< msg << WRITE <<std::endl;
#define debugL(msg) \
	std::cout << YELLOW_t <<"debug:["<< __FILE__ << ":"<< __LINE__ << "]:"<< msg << WRITE << std::endl;
#define infoL(msg) \
	std::cout << GREEN_t <<"infor:["<< __FILE__ << ":" << __LINE__ << "]:"<< msg << WRITE << std::endl;

#define _S(n) std::to_string(n)


class FileLog {
public:
	FileLog(const std::string& name) {
        file.open(name, std::ios::app);
        if (!file.is_open()) {
            errorL(name + "open fail!");
        }
	}
    ~FileLog() {
        file.close();
    }
    inline  void log(const std::string& msg) {
        file << msg << std::endl;
    }
    void ferror(const std::string& msg) {
        log(RED_t + msg);
    }

    void fdebug(const std::string& msg) {
        log(YELLOW_t + msg);
    }

    void finfo(const std::string& msg) {
        log(GREEN_t + msg);
    }
private:
    std::ofstream file;
};



#define LOG_NAME(name)\
    FileLog name(#name##".log");


#endif // !__DEBUG__L


