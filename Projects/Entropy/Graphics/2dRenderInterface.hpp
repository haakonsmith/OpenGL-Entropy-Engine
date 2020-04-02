
namespace Entropy {
    class m_2dRenderInterface {
      public:
        virtual GLuint loadTexture(std::string path) = 0;

        virtual void transform(Renderable *obj) = 0;

        virtual void renderQuad(vec3 position, float width, float height, bool hollow = false, vec3 color = vec3(1, 0, 0)) = 0;

        virtual void buffer(Renderable *_renderable) = 0;

        virtual void render(Renderable *obj) = 0;
    };

}  // namespace Entropy
