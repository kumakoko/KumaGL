// 字体纹理类
#ifndef kgl_font_texture_h__
#define kgl_font_texture_h__

#include "kgl_source_texture.h"
#include "kgl_font_uv_rect.h"
#include "kgl_font_glyph_info.h"

namespace kgl
{
    class FontTexture
    {
    public:
        typedef std::pair<uint32_t, uint32_t> CodePointRange;  /// A range of code points, inclusive on both ends
        typedef std::vector<CodePointRange> CodePointRangeList;
        typedef std::map<uint32_t, FontGlyphInfo> CodePointMap;
    public:
        /************************************
        Method:    Font
        FullName:  kgl::FontTexture::FontTexture
        Access:    public
        Returns:
        Qualifier:
        Parameter: RendererSPtr renderer
        Detail:
        ************************************/
        FontTexture(const char* texture_name, int32_t font_size, uint32_t texture_width, uint32_t texture_height);

        /************************************
        Method:    ~Font
        FullName:  kgl::FontTexture::~FontTexture
        Access:    public
        Returns:
        Qualifier:
        Detail:
        ************************************/

        ~FontTexture();

        /************************************
        Method:    Load
        FullName:  xgf::Font::Load
        Access:    public
        Returns:   void
        Qualifier:
        Parameter: const std::string & name
        Detail:
        ************************************/
        void Load(const char* name);

        /************************************
        Method:    Unload
        FullName:  xgf::Font::Unload
        Access:    public
        Returns:   void
        Qualifier:
        Detail:
        ************************************/
        void Unload();

        /************************************
        Method:    AddCodePointRange
        FullName:  xgf::Font::AddCodePointRange
        Access:    public
        Returns:   void
        Qualifier:
        Parameter: const CodePointRange & range
        Detail:
        ************************************/
        inline void AddCodePointRange(const CodePointRange& range)
        {
            codepoint_range_list_.push_back(range);
        }

        /************************************
        Method:    ClearCodePointRanges
        FullName:  xgf::Font::ClearCodePointRanges
        Access:    public
        Returns:   void
        Qualifier:
        Detail:
        ************************************/
        inline void ClearCodePointRanges()
        {
            codepoint_range_list_.clear();
        }

        /************************************
        Method:    codepoint_range_list
        FullName:  xgf::Font::codepoint_range_list
        Access:    public
        Returns:   const CodePointRangeList&
        Qualifier: const
        Detail:    获取到当前的code point range,用来从truetype字体文件中生成字形
        ************************************/
        inline const CodePointRangeList& codepoint_range_list() const
        {
            return codepoint_range_list_;
        }

        /************************************
        Method:    GetTextureWidth
        FullName:  xgf::Font::GetTextureWidth
        Access:    public
        Returns:   uint32_t
        Qualifier: const
        Detail: 获取到纹理的高度
        ************************************/
        inline uint32_t GetTextureWidth() const
        {
            return width_;
        }

        /************************************
        Method:    GetTextureHeight
        FullName:  xgf::Font::GetTextureHeight
        Access:    public
        Returns:   uint32_t
        Qualifier: const
        Detail: 获取到纹理的宽度
        ************************************/
        inline uint32_t GetTextureHeight() const
        {
            return height_;
        }

        /************************************
        Method:    HasBlankInTexture
        FullName:  xgf::Font::HasBlankInTexture
        Access:    public
        Returns:   bool
        Qualifier: const
        Detail:
        ************************************/
        inline bool HasBlankInTexture() const
        {
            return left_blank_num_ > 0;
        }

        /************************************
        Method:    SetGlyphTexCoords
        FullName:  xgf::Font::SetGlyphTexCoords
        Access:    public
        Returns:   void
        Qualifier:
        Parameter: CodePoint id     要设置的字符的unicode值
        Parameter: uint32_t u1_pixel
        Parameter: uint32_t v1_pixel
        Parameter: uint32_t u2_pixel
        Parameter: uint32_t v2_pixel
        Parameter: float texture_aspect
        Detail: 如果想手动从地从一个纹理中设置到一个字体的属性的话,可以调用本方法
        ************************************/
        void SetGlyphTexCoords(uint32_t id, uint32_t u1_pixel, uint32_t v1_pixel, uint32_t u2_pixel, uint32_t v2_pixel, float texture_aspect, int32_t bearing_x, int32_t bearing_y, uint32_t advance);

        /************************************
        Method:    GetGlyphAspectRatio
        FullName:  xgf::Font::GetGlyphAspectRatio
        Access:    public   要查找的字符的unicode值
        Returns:   float
        Qualifier: const
        Parameter: CodePoint id
        Detail: 获取给定字符的高宽比
        ************************************/
        float GetGlyphAspectRatio(uint32_t id) const;

        /************************************
        Method:    SetGlyphAspectRatio
        FullName:  xgf::Font::SetGlyphAspectRatio
        Access:    public
        Returns:   void
        Qualifier:
        Parameter: CodePoint id
        Parameter: float ratio
        Detail: 设置某字符的高宽比
        ************************************/
        void SetGlyphAspectRatio(uint32_t id, float ratio);

        /************************************
        Method:    GetGlyphInfo
        FullName:  xgf::Font::GetGlyphInfo
        Access:    public
        Returns:   const GlyphInfo*
        Qualifier: const
        Parameter: CodePoint id
        Detail: 获取某个给定字符的字形信息,如果不存在的话会抛异常
        ************************************/
        const FontGlyphInfo* GetGlyphInfo(uint32_t id) const;

        /************************************************************
        Method:    IsGlyphInfoExist
        FullName:  xgf::Font::IsGlyphInfoExist
        Access:    public
        Returns:   bool
        Qualifier: const
        Parameter: CodePoint id
        Details: 判断某个字符是不是已经在绘制到纹理中
        ************************************************************/
        bool IsGlyphInfoExist(uint32_t id) const;

        /************************************
        Method:    GetGlyphTexCoords
        FullName:  xgf::Font::GetGlyphTexCoords
        Access:    public
        Returns:   const UVRect&
        Qualifier: const
        Parameter: CodePoint id
        Detail:  返回给定的字符在纹理中的UV坐标
        ************************************/
        const FontUVRect& GetGlyphTexCoords(uint32_t id) const;

        /************************************
        Method:    InsertGlyphInfo
        FullName:  xgf::Font::InsertGlyphInfo
        Access:    public
        Returns:   void
        Qualifier:
        Parameter: CodePoint id
        Detail:  插入指定的字符
        ************************************/
        void InsertGlyphInfo(uint32_t id);

        /************************************
        Method:    RenderGlyphIntoTexture
        FullName:  xgf::Font::RenderGlyphIntoTexture
        Access:    public
        Returns:   void
        Qualifier:
        Parameter: CodePoint id 等待绘制的那个字符的unicode值
        Detail: 把Unicode值为id的那个字符的字形绘制入texture中去
        ************************************/
        void RenderGlyphIntoTexture(uint32_t id);

        /************************************
        Method:    GetLessUseChar
        FullName:  xgf::Font::GetLessUseChar
        Access:    public
        Returns:   xgf::Font::CodePoint
        Qualifier:
        Detail:  获取到当前使用次数最少的char
        ************************************/
        uint32_t GetLessUseChar();

        /************************************
        Method:    RemoveGlyph
        FullName:  xgf::Font::RemoveGlyph
        Access:    public
        Returns:   void
        Qualifier:
        Parameter: CodePoint id
        Detail:    移除指定的字符
        ************************************/
        void RemoveGlyph(uint32_t id);

        inline SourceTextureSPtr GetTexture()
        {
            return font_texture_;
        }

        inline const std::string& GetName() const
        {
            return texture_name_;
        }

        inline std::string& GetName()
        {
            return texture_name_;
        }

    protected:
        float                       ttf_size_;          // truetype字体的大小,单位是点数
        uint32_t                    ttf_resolution_;    // truetype字体的dpi分辨率
        int32_t                     ttf_max_bearing_y_; // 这种字体中,到baseline的最大距离
        bool                        antialias_color_;   // 本选项只对TRUE_TYPE font有效
        uint32_t                    width_;
        uint32_t                    height_;
        std::string                 font_name_;
        uint32_t                    left_blank_num_;    /// 纹理上使用区域还剩的个数
        uint32_t                    max_char_num_;
        uint8_t*                    image_data_;
        FT_Face                     ft_face_;
        uint32_t                    pixel_bytes_;
        uint32_t                    char_data_width_;
        uint32_t                    max_char_size_;
        uint32_t                    data_size_;
        int32_t                     max_height_;
        int32_t                     max_width_;
        uint32_t                    char_spacer_;       // 在字符纹理中，每个字符之间的间隔数
        uint32_t                    img_char_top_;
        uint32_t                    img_char_left_;
        float                       texture_aspect_;
        CodePointMap                codepoint_map_;
        CodePointRangeList          codepoint_range_list_; /// Range of code points to generate glyphs for (truetype only)
        SourceTextureSPtr           font_texture_;
        std::string                 texture_name_;
    };
}
#endif // kgl_font_texture_h__