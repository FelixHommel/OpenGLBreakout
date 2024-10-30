#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "glad/include/glad/gl.h"

class Texture2D
{
    public:
        Texture2D();

        void generate(unsigned int width, unsigned int height, unsigned char* data);

        inline void bind() const { glBindTexture(GL_TEXTURE_2D, m_ID); }
        [[nodiscard]] inline unsigned int& ID() { return m_ID; }

        inline void setInternalFormat(unsigned int format) { m_internalFormat = format; }
        inline void setImageFormat(unsigned int format) { m_imageFormat = format; }

    private:
        unsigned int m_ID;

        unsigned int m_width;
        unsigned int m_height;

        unsigned int m_internalFormat;
        unsigned int m_imageFormat;

        unsigned int m_wrapS;
        unsigned int m_wrapT;
        unsigned int m_filterMin;
        unsigned int m_filterMax;
};

#endif //!TEXTURE_HPP
