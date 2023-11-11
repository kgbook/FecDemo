#ifndef ELAPSED_TIME_H
#define ELAPSED_TIME_H

#include <string>

class ElapsedTime {
public:
    explicit ElapsedTime(std::string tag = "");

    ~ElapsedTime();

    uint64_t duration() const;

private:
    std::string tag_;
    uint64_t start_;
};

#endif //ELAPSED_TIME_H
