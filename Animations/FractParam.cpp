//
// Created by lance on 3/16/17.
//

#include <string>
#include "FractParam.h"

FractParam::FractParam()
{
    size = 0;
}

FractParam::FractParam(std::vector <std::string> _rawParameters)
{
    rawParameters = _rawParameters;
    size = (int) rawParameters.size();
    splitRawParameters();
}

void FractParam::splitRawParameters()
{
    std::string tag;
    std::string value;
    for (int i = 0; i < size; i++)
    {
        tag = rawParameters[i].substr(0, rawParameters[i].find(" "));
        tags.push_back(tag);
        value = rawParameters[i].substr(tag.size() + 1, rawParameters[i].size() - tag.size() - 2);
        values.push_back(value);
    }
}

int FractParam::getSize()
{
    return size;
}

std::string FractParam::getTag(int parameterNumber)
{
    return tags[parameterNumber];
}

std::string FractParam::getValue(int parameterNumber)
{
    return values[parameterNumber];
}

std::string FractParam::getValue(std::string tag)
{
    std::string value = "";
    for (int i = 0; i < size; i++)
    {
        if (tags[i] == tag)
        {
            value = values[i];
            break;
        }
    }
    return value;
}

void FractParam::setValue(std::string tag, std::string value)
{
    for (int i = 0; i < size; i++)
    {
        if (tags[i] == tag)
        {
            values[i] = value;
            break;
        }
    }
}

void FractParam::setValue(int tagNumber, std::string value)
{
    if (tagNumber < size - 1) values[tagNumber] = value;
    else std::cout << "Tag number out of bounds to set value \n";
}

void FractParam::print()
{
    for (int i = 0; i < size; i++)
    {
        std::cout << tags[i] << " " << values[i] << ";\n";
    }
}