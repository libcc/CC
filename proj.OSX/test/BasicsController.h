//
//  BasicsController.h
//  httpd
//
//  Created by CC on 2021/10/22.
//

#import <Cocoa/Cocoa.h>
#include <libcc.h>

@interface BasicsController : NSViewController
+ (bool) isRunning;
+ (void) setRunning:(bool)status;
- (BOOL)isEmptyString:(NSString *)str;

- (id)readLocalJSONFileWithName:(NSString *)name;

- (NSString *)getDeviceIpAddresses:(int) family;

- (void) alert:(NSString*)message Title:(NSString*)title;
@end
