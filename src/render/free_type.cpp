#include <game/gameManager.hpp> 
#include <ft2build.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H
#include FT_ERRORS_H



GLuint createTextTexture(const std::string& text) {
    int width = 0;
    int height = 0;
    int baseline = 0;
    
    // Вычисление размеров текстуры
    for (auto c : text) {
        Character ch = (*GameManager::Characters)[c];

        width += (ch.Advance >> 6);  // Продвигаемся на величину advance
        height = std::max(height, ch.Size.y);
        baseline = std::max(baseline, ch.Bearing.y);
    }

    // Создание пустого изображения
    std::vector<uint8_t> image(width * height, 0);

    // Заполнение изображения символами
    int xOffset = 0;
    for (auto c : text) {
        Character ch = (*GameManager::Characters)[c];

        for (int y = 0; y < ch.Size.y; ++y) {
            for (int x = 0; x < ch.Size.x; ++x) {
                int ix = xOffset + x + (baseline - ch.Bearing.y + y) * width;
                image[ix] = ch.TextureID; // Проверьте, что используете правильный индекс и данные
            }
        }

        xOffset += (ch.Advance >> 6); // Движение на следующий символ
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, image.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}
