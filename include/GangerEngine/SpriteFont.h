/*
    This is a modified version of the SpriteFont class from the
    Seed Of Andromeda source code.
    Use it for any of your projects, commercial or otherwise,
    free of charge, but do not remove this disclaimer.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS
    ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
    EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
    INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
    RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
    ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
    OF THIS SOFTWARE.

    November 28 2014
    Original Author: Cristian Zaloj
    Modified By: Benjamin Arnold and GangerGames
*/

#ifndef _SPRITEFONT_H_
#define _SPRITEFONT_H_

#include <GangerEngine/Vertex.h>

#include <SDL/SDL_ttf.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

namespace GangerEngine {
struct GLTexture;
class SpriteBatch;

struct CharGlyph {
public:
    char character;
    glm::vec4 uvRect;
    glm::vec2 size;
};

#define FIRST_PRINTABLE_CHAR ((char)32)
#define LAST_PRINTABLE_CHAR ((char)126)

/// For text justification
enum class Justification {
    LEFT, MIDDLE, RIGHT
};

class SpriteFont {
 public:
    SpriteFont() {};
    SpriteFont(const char* font, int size, char cs, char ce);
    SpriteFont(const char* font, int size) :
        SpriteFont(font, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR)
    {
        // Empty
    }

    void Init(const char* font, int size);
    void Init(const char* font, int size, char cs, char ce);

    /// Destroys the font resources
    void Dispose();

    int GetFontHeight() const { return m_fontHeight; }

    /// Measures the dimensions of the text
    glm::vec2 Measure(const char* s);

    /// Draws using a spritebatch
    void Draw(SpriteBatch& batch, const char* s, glm::vec2 position,
        glm::vec2 scaling, float depth, ColorRGBA8 tint,
        Justification just = Justification::LEFT);

 private:
    static std::vector<int>* CreateRows(glm::ivec4* rects, int rectsLength,
        int r, int padding, int& w);

    int m_regStart, m_regLength;
    CharGlyph* m_glyphs;
    int m_fontHeight;

    unsigned int m_texID;
};
}  // namespace GangerEngine

#endif  // _SPRITEFONT_H_
