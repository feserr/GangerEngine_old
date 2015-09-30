
#pragma once

#include <Vertex.h>
#include <GLTexture.h>

#include <GL/glew.h>
#include <string>

class Sprite
{
public:
    Sprite();
    ~Sprite();

    void Init(float x, float y, float width, float height, std::string texturePath);
    void Draw();

private:
    void SetSpriteCords(float x, float y, float width, float height, Vertex* vertex);
    void SetColor(Vertex* vertex, int* length);

    template<typename T, int size>
    int GetArrLength(T(&)[size]){ return size; }

    float _x, _y, _width, _height;
    GLuint _vboID;
    GLTexture _texture;
};
