#include"texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include"../thirdparty/stb_image.h"

#include<iostream>

Texture::Texture(const std::string& path, unsigned int unit)
{
    _unit = unit;

    //1 用stbImage读图片
    int channels = { 0 };

    //翻转y轴
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path.c_str(), &_width, &_height, &channels, STBI_rgb_alpha);

    if (!data)
    {
        std::cout << "failed to load texture\n";
        return;
    }

    //2 生成纹理并激活单元绑定
    glGenTextures(1, &_texture);
    //激活纹理单元
    glActiveTexture(GL_TEXTURE0+_unit);
    //绑定纹理对象
    glBindTexture(GL_TEXTURE_2D, _texture);

    //3 传输纹理数据,开辟显存
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    int width = _width;
    int height = _height;
    //遍历每个mipmap的层级 为每个级别的mipmap填充图片数据
    for (int level = 0; true; ++level)
    {
        //1 将当前级别的mipmap对应的数据发往gpu
        glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        //2 判断是否要退出

        if (width == 0 && height == 0)
            break;

        //3 计算下次循环的宽高
        width = width > 1 ? width / 2 : 1;
        height = height > 1 ? height / 2 : 1;  
    }

    //释放数据
    stbi_image_free(data);

    //设置纹理的过滤方式
    //当需要的像素 > 图片的像素 Linear
    //当需要的像素 < 图片像素 Nearest
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //设置纹理包裹方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u方向
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v方向
}
Texture::~Texture()
{
    if (_texture != 0)
    {
        glDeleteTextures(1, &_texture);
    }
}

void Texture::bind()
{
    //先切换纹理单元，然后绑定texture对象
    glActiveTexture(GL_TEXTURE0 + _unit);
    glBindTexture(GL_TEXTURE_2D, _texture);
}
