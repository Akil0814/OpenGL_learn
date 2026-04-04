#include"texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include"../thirdparty/stb_image.h"
#include "../wrapper/check_error.h"

#include<iostream>

std::map<std::string, Texture*> Texture::_texture_cache{};

Texture* Texture::create_texture(const std::string& path, unsigned int unit)
{
    auto iter = _texture_cache.find(path);
    if (iter != _texture_cache.end())
        return iter->second;

    auto texture = new Texture(path, unit);
    _texture_cache[path] = texture;

    return texture;
}

Texture* Texture::create_texture_from_memory(
    const std::string& path, unsigned int unit,
    unsigned char* data_in,
    uint32_t width_in,
    uint32_t height_in
)
{
    auto iter = _texture_cache.find(path);
    if (iter != _texture_cache.end())
        return iter->second;

    auto texture = new Texture(data_in,width_in,height_in,unit);
    _texture_cache[path] = texture;

    return texture;
}

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
        std::cerr << "failed to load texture\n";
        return;
    }

    //2 生成纹理并激活单元绑定
    GL_CALL(glGenTextures(1, &_texture));
    //激活纹理单元
    GL_CALL(glActiveTexture(GL_TEXTURE0+_unit));
    //绑定纹理对象
    GL_CALL(glBindTexture(GL_TEXTURE_2D, _texture));

    //3 传输纹理数据,开辟显存
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    /*
    //遍历每个mipmap的层级 为每个级别的mipmap填充图片数据
    int width = _width;
    int height = _height;
    for (int level = 0; true; ++level)
    {
        //1 将当前级别的mipmap对应的数据发往gpu
        glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        //2 判断是否要退出

        if (width == 1 && height == 1)
            break;

        //3 计算下次循环的宽高
        width = width > 1 ? width / 2 : 1;
        height = height > 1 ? height / 2 : 1;  
    }
    */

    //释放数据
    stbi_image_free(data);

    //设置纹理的过滤方式
    //当需要的像素 > 图片的像素 Linear
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    //当需要的像素 < 图片像素 Nearest
    //GL_NEAREST        MIPMAP_LINEAR
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));

    //设置纹理包裹方式
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));//u方向
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));//v方向
}

Texture::Texture(
    unsigned char* data_in,
    uint32_t width_in,
    uint32_t height_in,
    unsigned int unit
)
{
    _unit = unit;

    //1 用stbImage读图片
    int channels = { 0 };

    //翻转y轴
    stbi_set_flip_vertically_on_load(true);

    //计算整张图片的大小
    //Assimp规定：如果内嵌纹理是png或者jpg压缩格式的话 height=0， width就代表了图片的大小
    uint32_t data_in_size = 0;
    if (!height_in)
        data_in_size = width_in;
    else
        //偷懒：默认RGBA格式
        data_in_size = width_in * height_in * 4;//

    unsigned char* data = stbi_load_from_memory(data_in, data_in_size, &_width, &_height, &channels, STBI_rgb_alpha);

    if (!data)
    {
        std::cerr << "failed to load texture\n";
        return;
    }

    //2 生成纹理并激活单元绑定
    GL_CALL(glGenTextures(1, &_texture));
    //激活纹理单元
    GL_CALL(glActiveTexture(GL_TEXTURE0 + _unit));
    //绑定纹理对象
    GL_CALL(glBindTexture(GL_TEXTURE_2D, _texture));

    //3 传输纹理数据,开辟显存
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    //释放数据
    stbi_image_free(data);

    //设置纹理的过滤方式
    //当需要的像素 > 图片的像素 Linear
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    //当需要的像素 < 图片像素 Nearest
    //GL_NEAREST        MIPMAP_LINEAR
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));

    //设置纹理包裹方式
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));//u方向
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));//v方向
}

Texture::~Texture()
{
    if (_texture != 0)
    {
        GL_CALL(glDeleteTextures(1, &_texture));
    }
}

void Texture::bind()
{
    //先切换纹理单元，然后绑定texture对象
    GL_CALL(glActiveTexture(GL_TEXTURE0 + _unit));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, _texture));
}
