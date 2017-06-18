// 封装绘制模式
namespace kgl
{
    enum DrawMode
    {
        DM_POINT = GL_POINT,
        DM_LINE = GL_LINE,
        DM_FILL = GL_FILL
    };

    class RenderStateDrawMode
    {
    private:
        DrawMode current_draw_mode_;
    public:
        void SetCurrentDrawMode(DrawMode current_mode);

        inline DrawMode GetCurrentDrawMode()
        {
            return current_draw_mode_;
        }
    };
}