//
//  kgl_message_box.m
//  klib
//
//  Created by xiongxinke on 2017/10/16.
//  Copyright © 2017年 xiongxinke. All rights reserved.
//

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
