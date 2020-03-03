///**************************************************************************************************************************
//Copyright(C) 2014-2020 www.xionggf.com
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
//files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
//modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the 
//Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
//Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
//WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
//COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
//ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//**************************************************************************************************************************/
#ifndef kgl_md2_animation_status_h__
#define kgl_md2_animation_status_h__

namespace kgl
{
	class MD2AnimationStatus
	{
	public:
		static const int ANIM_TYPE_NAME_LEN = 32;

		inline int start_frame() const
		{
			return start_frame_;
		}
		
		inline void set_start_frame(int val)
		{ 
			start_frame_ = val; 
		}

		inline int end_frame() const
		{ 
			return end_frame_;
		}

		inline void set_end_frame(int val)
		{ 
			end_frame_ = val;
		}

		inline int fps() const 
		{
			return fps_; 
		}

		inline void set_fps(int val)
		{
			fps_ = val;
		}

		inline float current_time() const 
		{
			return current_time_;
		}

		inline void set_current_time(float val)
		{ 
			current_time_ = val;
		}
		
		inline float old_time() const 
		{
			return old_time_;
		}

		inline void set_old_time(float val)
		{ 
			old_time_ = val;
		}
		
		inline float interpolation_percent() const
		{
			return interpolation_percent_; 
		}

		inline void set_interpolation_percent(float val)
		{ 
			interpolation_percent_ = val;
		}
		
		inline int current_frame() const
		{ 
			return current_frame_;
		}

		inline void set_current_frame(int val) 
		{
			current_frame_ = val;
		}
		
		inline int next_frame() const
		{ 
			return next_frame_;
		}

		inline void set_next_frame(int val)
		{
			next_frame_ = val;
		}
		
		void set_type(const char* val);

		inline const char* type() const 
		{
			return type_;
		}

		/*********************************************************
		判断本status是不是一个无效status，当start frame end frame和fps任意一个为-1时
		表示本status就不是一个无效的status
		@return         无效返回true，有效返回false    
		*********************************************************/
		bool IsInvalidStatus() const;

		/*********************************************************
		判断本status对象是否要使用下一帧了
		@return         是true否false       
		*********************************************************/
		bool IsShouldUseNextFrame() const;
	
		MD2AnimationStatus();
	private:
		int		start_frame_;				// first frame
		int		end_frame_;				// last frame
		int		fps_;					// frame per second for this animation
		float	current_time_;				// current time
		float	old_time_;				// old time
		float	interpolation_percent_;				// percent of interpolation
		char    type_[ANIM_TYPE_NAME_LEN];					// animation type
		int		current_frame_;				// current frame
		int		next_frame_;				// next frame
	};
}

#endif // kgl_md2_animation_status_h__