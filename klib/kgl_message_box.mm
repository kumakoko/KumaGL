/**************************************************************************************************************************
Copyright(C) 2014-2018 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
#import <Foundation/Foundation.h>
#import <Foundation/NSString.h>
#import <string>
#import "kgl_message_box.h"

namespace kgl
{
    void MessageBox(const std::wstring& title,const std::wstring& content)
    {
        // NSString* msg_str = [NSString stringWithCString:utf8_c_str encoding:NSUTF8StringEncoding];
    
        NSData* title_data = [NSData dataWithBytes:title.data() length:sizeof(wchar_t) * title.size()];
        NSString* title_str = [[NSString alloc] initWithData:title_data encoding:NSUTF32LittleEndianStringEncoding];
         

        NSData* content_data = [NSData dataWithBytes:content.data() length:sizeof(wchar_t) * content.size()];
        NSString* content_str = [[NSString alloc] initWithData:content_data encoding:NSUTF32LittleEndianStringEncoding];
  
        
        CFOptionFlags  result;
        CFUserNotificationDisplayAlert(0,
                                       kCFUserNotificationNoDefaultButtonFlag,
                                       NULL, NULL, NULL,
                                       (CFStringRef)title_str,
                                       (CFStringRef)content_str,
                                       NULL, NULL, NULL, &result);
    }
}
