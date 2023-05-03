#pragma once
#include <vector>
#include "color.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

class SaveImage
{
public:
	static void Save(const char* path, std::vector<Color> buffer, int w, int h)
	{
        std::vector<unsigned char> data;

        for (const Color& color : buffer)
        {
            data.push_back(static_cast<char>(255 * color.r));
            data.push_back(static_cast<char>(255 * color.g));
            data.push_back(static_cast<char>(255 * color.b));
        }

        stbi_flip_vertically_on_write(1);
        stbi_write_png(path, w, h, 3, data.data(), w * 3 * sizeof(char));
	}
};