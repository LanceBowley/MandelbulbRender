//
// Created by lance on 3/16/17.
//

#ifndef RENDER_KEYFRAME_H
#define RENDER_KEYFRAME_H

#include <vector>
#include <iostream>

class FractParam {
public:
    FractParam();
    FractParam(std::vector<std::string> _rawParameters);

    int getSize();
    std::string getTag(int parameterNumber);
    std::string getValue(int parameterNumber);
    std::string getValue(std::string tag);
    void setValue(std::string tag, std::string value);
    void setValue(int tagNumber, std::string value);

    void print();

private:
    std::vector<std::string> rawParameters;
    std::vector<std::string> tags;
    std::vector<std::string> values;
    std::string defaultKeyframePath;

    int size;

    void splitRawParameters();
};



#endif //RENDER_KEYFRAME_H
