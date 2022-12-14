#include "text_component.h"
#include "renderer_locator.h"

using namespace nim;

TextComponent::TextComponent() : text{"no_text"} {
    name = "TextComponent";
    transform = nullptr;
    fontColor = Black;
    auto *rend = dynamic_cast<SDLRenderer *>(RendererLocator::GetRenderer());
    renderer = rend->Renderer();
}

TextComponent::TextComponent(const std::string &fontName) : text{"no_text"} {
    font = AssetManager::Instance().Get<Font>(fontName);
    name = "TextComponent";
    transform = nullptr;
    fontColor = Black;
    auto *rend = dynamic_cast<SDLRenderer *>(RendererLocator::GetRenderer());
    renderer = rend->Renderer();
}

TextComponent::TextComponent(const std::string &fontName, int size) : text{"no_text"} {
    auto baseFontName = AssetManager::Instance().GetFilename(fontName);
    auto assetId = fontName + "_" + std::to_string(size);
    font = AssetManager::Instance().LoadAsset<Font>(baseFontName, assetId);
    font.SetSize(size);
    name = "TextComponent";
    transform = nullptr;
    fontColor = Black;
    auto *rend = dynamic_cast<SDLRenderer *>(RendererLocator::GetRenderer());
    renderer = rend->Renderer();
}

TextComponent::TextComponent(const TextComponent &other) {
    name = other.name;
    transform = other.transform;
    fontColor = other.fontColor;
    srcCanvas = other.srcCanvas;
    destCanvas = other.destCanvas;
    texture = other.texture;
    renderer = other.renderer;

    font = other.font;
    text = other.text;
}

TextComponent::TextComponent(TextComponent &&other) noexcept {
    name = std::move(other.name);
    transform = other.transform;
    fontColor = other.fontColor;
    srcCanvas = other.srcCanvas;
    destCanvas = other.destCanvas;
    texture = std::move(other.texture);
    renderer = other.renderer;

    font = std::move(other.font);
    text = std::move(other.text);

    other.transform = nullptr;
}

TextComponent::~TextComponent() {
}

TextComponent &TextComponent::operator=(const TextComponent &other) {
    if (&other != this) {
        name = other.name;
        transform = other.transform;
        fontColor = other.fontColor;
        srcCanvas = other.srcCanvas;
        destCanvas = other.destCanvas;
        texture = other.texture;
        renderer = other.renderer;
        font = other.font;
        text = other.text;
    }
    return *this;
}
TextComponent &TextComponent::operator=(TextComponent &&other) noexcept {
    if (&other != this) {
        transform = nullptr;

        name = std::move(other.name);
        transform = other.transform;
        fontColor = other.fontColor;
        srcCanvas = other.srcCanvas;
        destCanvas = other.destCanvas;
        texture = std::move(other.texture);
        renderer = other.renderer;

        font = std::move(other.font);
        text = std::move(other.text);

        other.transform = nullptr;
    }
    return *this;
}

void TextComponent::Init() {
    CreateTexture();
}

void TextComponent::Update() {
    font.Draw(texture.get(), srcCanvas, destCanvas);
}

void TextComponent::Quit() {}

void TextComponent::SetTransform(Transform *transform) {
    Component::SetTransform(transform);
    if (!font.filename.empty()) {
        auto rect = this->transform->GetRect();
        srcCanvas.x = 0;
        srcCanvas.y = 0;
        srcCanvas.w = rect->w;
        srcCanvas.h = rect->h;
        destCanvas.x = rect->x;
        destCanvas.y = rect->y;
        destCanvas.w = rect->w;
        destCanvas.h = rect->h;
    }
}

void TextComponent::Text(std::string text) {
    this->text = text;
    CreateTexture();
}

void TextComponent::CreateTexture() {
    SDL_Surface *surface = TTF_RenderText_Blended(font.Get(), text.c_str(), fontColor.ToSDLColor());

    srcCanvas.w = surface->w;
    srcCanvas.h = surface->h;
    destCanvas.w = surface->w;
    destCanvas.h = surface->h;

    texture.reset(SDL_CreateTextureFromSurface(renderer, surface), [](SDL_Texture *tex) { SDL_DestroyTexture(tex); });
    SDL_FreeSurface(surface);
}
void TextComponent::SetColor(const Color &newColor) {
    this->fontColor = newColor;
    CreateTexture();
}
