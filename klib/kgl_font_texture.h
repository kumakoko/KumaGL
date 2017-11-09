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
        /// <summary>
        /// Initializes a new instance of the <see cref="FontTexture"/> class.
        /// </summary>
        /// <param name="texture_name">The texture_name.</param>
        /// <param name="font_size">The font_size.</param>
        /// <param name="texture_width">The texture_width.</param>
        /// <param name="texture_height">The texture_height.</param>
        FontTexture(const char* texture_name, int32_t font_size, uint32_t texture_width, uint32_t texture_height);

        /// <summary>
        /// Finalizes an instance of the <see cref="FontTexture"/> class.
        /// </summary>
        ~FontTexture();

        /// <summary>
        /// Loads the specified name.
        /// </summary>
        /// <param name="name">The name.</param>
        void Load(const char* name);

        /// <summary>
        /// Unloads this instance.
        /// </summary>
        void Unload();

        /// <summary>
        /// Adds the code point range.
        /// </summary>
        /// <param name="range">The range.</param>
        inline void AddCodePointRange(const CodePointRange& range)
        {
            codepoint_range_list_.push_back(range);
        }

        /// <summary>
        /// Clears the code point ranges.
        /// </summary>
        inline void ClearCodePointRanges()
        {
            codepoint_range_list_.clear();
        }

        /// <summary>
        /// 获取到当前的code point range,用来从truetype字体文件中生成字形
        /// </summary>
        /// <returns>const CodePointRangeList &.</returns>
        inline const CodePointRangeList& codepoint_range_list() const
        {
            return codepoint_range_list_;
        }

        /// <summary>
        /// 获取到纹理的高度
        /// </summary>
        /// <returns>纹理的高度.</returns>
        inline uint32_t GetTextureWidth() const
        {
            return width_;
        }

        /// <summary>
        /// 获取到纹理的宽度
        /// </summary>
        /// <returns>纹理的宽度.</returns>
        inline uint32_t GetTextureHeight() const
        {
            return height_;
        }

        /// <summary>
        /// Determines whether [has blank in texture].
        /// </summary>
        /// <returns>bool.</returns>
        inline bool HasBlankInTexture() const
        {
            return left_blank_num_ > 0;
        }

        /// <summary>
        /// 如果想手动从地从一个纹理中设置到一个字体的属性的话,可以调用本方法
        /// </summary>
        /// <param name="id">要设置的字符的unicode值</param>
        /// <param name="u1_pixel">The u1_pixel.</param>
        /// <param name="v1_pixel">The v1_pixel.</param>
        /// <param name="u2_pixel">The u2_pixel.</param>
        /// <param name="v2_pixel">The v2_pixel.</param>
        /// <param name="texture_aspect">The texture_aspect.</param>
        /// <param name="bearing_x">The bearing_x.</param>
        /// <param name="bearing_y">The bearing_y.</param>
        /// <param name="advance">The advance.</param>
        void SetGlyphTexCoords(uint32_t id, uint32_t u1_pixel, uint32_t v1_pixel, uint32_t u2_pixel, uint32_t v2_pixel, float texture_aspect, int32_t bearing_x, int32_t bearing_y, uint32_t advance);

        /************************************
        Method:    GetGlyphAspectRatio
        FullName:  xgf::Font::GetGlyphAspectRatio
        Access:    public   
        Returns:   float
        Qualifier: const
        Parameter: CodePoint id
        Detail: 获取给定字符的高宽比
        ************************************/
        /// <summary>
        /// 获取给定字符的高宽比
        /// </summary>
        /// <param name="id">要查找的字符的unicode值</param>
        /// <returns>给定字符的高宽比.</returns>
        float GetGlyphAspectRatio(uint32_t id) const;

        /// <summary>
        /// 设置某字符的高宽比
        /// </summary>
        /// <param name="id">The identifier.</param>
        /// <param name="ratio">The ratio.</param>
        void SetGlyphAspectRatio(uint32_t id, float ratio);

        /// <summary>
        /// 获取某个给定字符的字形信息,如果不存在的话会抛异常
        /// </summary>
        /// <param name="id">The identifier.</param>
        /// <returns>const FontGlyphInfo *.</returns>
        const FontGlyphInfo* GetGlyphInfo(uint32_t id) const;

        /// <summary>
        /// 判断某个字符是不是已经在绘制到纹理中
        /// </summary>
        /// <param name="id">The identifier.</param>
        /// <returns>bool.</returns>
        bool IsGlyphInfoExist(uint32_t id) const;

        /// <summary>
        /// 返回给定的字符在纹理中的UV坐标
        /// </summary>
        /// <param name="id">The identifier.</param>
        /// <returns>const FontUVRect &.</returns>
        const FontUVRect& GetGlyphTexCoords(uint32_t id) const;

        /// <summary>
        ///  插入指定的字符
        /// </summary>
        /// <param name="id">The identifier.</param>
        void InsertGlyphInfo(uint32_t id);

        /// <summary>
        /// 把Unicode值为id的那个字符的字形绘制入texture中去
        /// </summary>
        /// <param name="id">等待绘制的那个字符的unicode值</param>
        void RenderGlyphIntoTexture(uint32_t id);

        /// <summary>
        /// 获取到当前使用次数最少的char
        /// </summary>
        /// <returns>uint32_t.</returns>
        uint32_t GetLessUseChar();

        /// <summary>
        /// 移除指定的字符
        /// </summary>
        /// <param name="id">The identifier.</param>
        void RemoveGlyph(uint32_t id);

        /// <summary>
        /// Gets the texture.
        /// </summary>
        /// <returns>SourceTextureSPtr.</returns>
        inline SourceTextureSPtr GetTexture()
        {
            return font_texture_;
        }

        /// <summary>
        /// Gets the name.
        /// </summary>
        /// <returns>const std.string &.</returns>
        inline const std::string& GetName() const
        {
            return texture_name_;
        }

        /// <summary>
        /// Gets the name.
        /// </summary>
        /// <returns>std.string &.</returns>
        inline std::string& GetName()
        {
            return texture_name_;
        }

    protected:
        /// <summary>
        /// truetype字体的大小,单位是点数
        /// </summary>
        float                       ttf_size_;

        /// <summary>
        ///truetype字体的dpi分辨率
        /// </summary>
        uint32_t                    ttf_resolution_;

        /// <summary>
        /// 这种字体中,到baseline的最大距离
        /// </summary>
        int32_t                     ttf_max_bearing_y_;

        /// <summary>
        /// 本选项只对TRUE_TYPE font有效
        /// </summary>
        bool                        antialias_color_;

        /// <summary>
        /// The width_
        /// </summary>
        uint32_t                    width_;

        /// <summary>
        /// The height_
        /// </summary>
        uint32_t                    height_;

        /// <summary>
        /// The font_name_
        /// </summary>
        std::string                 font_name_;

        /// <summary>
        /// 纹理上使用区域还剩的个数
        /// </summary>
        uint32_t                    left_blank_num_;

        /// <summary>
        /// The max_char_num_
        /// </summary>
        uint32_t                    max_char_num_;

        /// <summary>
        /// The image_data_
        /// </summary>
        uint8_t*                    image_data_;

        /// <summary>
        /// The ft_face_
        /// </summary>
        FT_Face                     ft_face_;

        /// <summary>
        /// The pixel_bytes_
        /// </summary>
        uint32_t                    pixel_bytes_;

        /// <summary>
        /// The char_data_width_
        /// </summary>
        uint32_t                    char_data_width_;

        /// <summary>
        /// The max_char_size_
        /// </summary>
        uint32_t                    max_char_size_;

        /// <summary>
        /// The data_size_
        /// </summary>
        uint32_t                    data_size_;

        /// <summary>
        /// The max_height_
        /// </summary>
        int32_t                     max_height_;

        /// <summary>
        /// The max_width_
        /// </summary>
        int32_t                     max_width_;

        /// <summary>
        /// 在字符纹理中，每个字符之间的间隔数
        /// </summary>
        uint32_t                    char_spacer_;

        /// <summary>
        /// The img_char_top_
        /// </summary>

        uint32_t img_char_top_;
        /// <summary>
        /// The img_char_left_
        /// </summary>

        uint32_t img_char_left_;
        /// <summary>
        /// The texture_aspect_
        /// </summary>
        float texture_aspect_;

        /// <summary>
        /// The codepoint_map_
        /// </summary>
        CodePointMap codepoint_map_;

        /// <summary>
        /// Range of code points to generate glyphs for (truetype only)
        /// </summary>
        CodePointRangeList codepoint_range_list_;

        /// <summary>
        /// The font_texture_
        /// </summary>
        SourceTextureSPtr font_texture_;

        /// <summary>
        /// The texture_name_
        /// </summary>
        std::string texture_name_;
    };
}
#endif // kgl_font_texture_h__