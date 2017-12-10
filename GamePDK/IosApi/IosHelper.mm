//
//  IosHelper.cpp
//  kkddz
//
//  Created by macbook110 on 13-6-4.
//
//

#include "IosHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import <UIKit/UIDevice.h>
#include <sys/socket.h> // Per msqr
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#import "WXApi.h"
#import "WXApiObject.h"
#include "Game/Script/utility.h"
#include "cocos2d.h"
#endif//

UIImage* thumbnailWithImage(std::string fileName,float scale)
{
    
    UIImage *newimage = [UIImage imageWithContentsOfFile: [NSString stringWithUTF8String:fileName.c_str()]];
    
    if (nil == newimage) {
        return nil;
    }
    
    else{
        CGSize size = [newimage size];
        size = CGSizeMake(size.width * scale, size.height * scale);
        UIGraphicsBeginImageContext(size);
        
        [newimage drawInRect:CGRectMake(0, 0, size.width, size.height)];
        
        newimage = UIGraphicsGetImageFromCurrentImageContext();
        
        UIGraphicsEndImageContext();
        
    }
    
    return newimage;
    
}

void IosHelper::sendAuthRequest()
{ 
    //构造SendAuthReq结构体 
    SendAuthReq* req =[[[SendAuthReq alloc ] init ] autorelease ];
    req.scope = @"snsapi_userinfo" ;
    req.state = @"123" ;
    //第三方向微信终端发送一个SendAuthReq消息结构
    [WXApi sendReq:req]; 
}

void IosHelper::shareWithWeixinCircleTxt(const char * pTxt,const char * pDes,const char * pUrl){
    NSString *txt = [NSString stringWithCString:pTxt encoding:NSUTF8StringEncoding];
    WXMediaMessage *message = [WXMediaMessage message];
    message.title = txt;
    message.description = [NSString stringWithCString:pDes encoding:NSUTF8StringEncoding];
    message.messageExt = txt;
    message.messageAction = txt;
    
    [message setThumbImage:[UIImage imageNamed:@"weixin_share_icon.png"]];
    
    WXWebpageObject *ext = [WXWebpageObject object];
    ext.webpageUrl = [NSString stringWithCString:pUrl encoding:NSUTF8StringEncoding];
    
    message.mediaObject = ext;
    
    
    SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
    req.bText = NO;
    req.message = message; 
    req.scene = WXSceneTimeline;
    
    [WXApi sendReq:req];
}
void IosHelper::shareWithWeixinFriendTxt(const char * pTxt,const char * pDes,const char * pUrl){
    
    NSString *txt = [NSString stringWithCString:pTxt encoding:NSUTF8StringEncoding];
    WXMediaMessage *message = [WXMediaMessage message];
    message.title = txt;
    message.description = [NSString stringWithCString:pDes encoding:NSUTF8StringEncoding];
    message.messageExt = txt;
    message.messageAction = txt;
    
    WXWebpageObject *ext = [WXWebpageObject object];
    ext.webpageUrl = [NSString stringWithCString:pUrl encoding:NSUTF8StringEncoding];
    
    message.mediaObject = ext;
    
    
    SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
    req.bText = NO;
    req.message = message; 
    req.scene = WXSceneSession;
    
    [WXApi sendReq:req];

}

void IosHelper::shareWithWeixinFriendImg(const char * pTxt,const char *FileName)
{
    NSString *txt = [NSString stringWithCString:pTxt encoding:NSUTF8StringEncoding];
    NSString *filePath = [NSString stringWithCString:FileName encoding:NSUTF8StringEncoding];
    WXMediaMessage *message = [WXMediaMessage message];
    
    message.title = txt;
    message.description = txt;
    [message setThumbImage:thumbnailWithImage(FileName,0.3)];
    
    WXImageObject *ext = [WXImageObject object];
    ext.imageData = [NSData dataWithContentsOfFile:filePath];
    
    
    
    message.mediaObject = ext;
    
    SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
    req.bText = NO;
    req.message = message;
    req.scene = WXSceneSession;
    
    [WXApi sendReq:req];
}
void IosHelper::shareWithWeixinCircleImg(const char * pTxt,const char *FileName)
{
    NSString *txt = [NSString stringWithCString:pTxt encoding:NSUTF8StringEncoding];
    NSString *filePath = [NSString stringWithCString:FileName encoding:NSUTF8StringEncoding];
    WXMediaMessage *message = [WXMediaMessage message];
    
    message.title = txt;
    message.description = txt;
    [message setThumbImage:thumbnailWithImage(FileName,0.3)];

    WXImageObject *ext = [WXImageObject object];
    ext.imageData = [NSData dataWithContentsOfFile:filePath];
    
    message.mediaObject = ext;
    
    SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
    req.bText = NO;
    req.message = message;
    req.scene = WXSceneTimeline;
    
    [WXApi sendReq:req];
}
void IosHelper::startBrowserJni( const char * url)
{
    NSString *nsFineName = [NSString stringWithCString:url encoding:NSUTF8StringEncoding];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:nsFineName]];
}
 AVAudioRecorder *recorder = NULL;
void IosHelper::beginRecord(const char *_fileName)
{
    if (IosHelper::canRecord() == FALSE)
    {
        NSDictionary *infoDictionary = [[NSBundle mainBundle] infoDictionary];
        NSString * appName = [infoDictionary objectForKey:@"CFBundleDisplayName"];
        NSString * tmpTitle = [NSString stringWithUTF8String:utility::getScriptString("MicrophoneTitle").c_str()];
        NSString * tmpMsg = [NSString stringWithFormat:[NSString stringWithUTF8String:utility::getScriptString("MicrophoneText").c_str()],appName];
        UIAlertView * messageBox = [[UIAlertView alloc] initWithTitle: tmpTitle
                                                              message: tmpMsg
                                                             delegate: nil
                                                    cancelButtonTitle: @"OK"
                                                    otherButtonTitles: nil];
        [messageBox autorelease];
        [messageBox show];
        return;
    }
    if (recorder == nil)
    {
        //设置文件名和录音路径
        NSString *recordFilePath = [NSString stringWithCString:_fileName encoding:NSUTF8StringEncoding];
        
        NSDictionary *recordSetting = [[NSDictionary alloc] initWithObjectsAndKeys:
                                       [NSNumber numberWithFloat: 8000.0],AVSampleRateKey, //采样率
                                       [NSNumber numberWithInt: kAudioFormatLinearPCM],AVFormatIDKey,
                                       [NSNumber numberWithInt:16],AVLinearPCMBitDepthKey,//采样位数 默认 16
                                       [NSNumber numberWithInt: 1], AVNumberOfChannelsKey,//通道的数目
                                       nil];
        //初始化录音
        NSError *error = nil;
        recorder = [[ AVAudioRecorder alloc] initWithURL:[NSURL URLWithString:recordFilePath] settings:recordSetting error:&error];
    }
    recorder.meteringEnabled = YES;
    [recorder prepareToRecord];
    //开始录音
    UInt32 sessionCategory = kAudioSessionCategory_PlayAndRecord;
    AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(sessionCategory), &sessionCategory);
    
    // 扬声器播放
    UInt32 audioRouteOverride = kAudioSessionOverrideAudioRoute_Speaker;
    AudioSessionSetProperty (kAudioSessionProperty_OverrideAudioRoute, sizeof(audioRouteOverride), &audioRouteOverride);
    [[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategoryPlayAndRecord error:nil];
    [[AVAudioSession sharedInstance] setActive:YES error:nil];
    [recorder record];
}
const char * IosHelper::endRecord()
{
    if (recorder == nil)
        return "";
    if (recorder.isRecording)
    {
        [recorder stop];
        [recorder release];
        recorder = nil;
    }
	return "";
}

bool IosHelper::canRecord()
{
    __block BOOL bCanRecord = YES;
    if ([[[UIDevice currentDevice]systemVersion]floatValue] >= 7.0) {
        
        AVAudioSession *audioSession = [AVAudioSession sharedInstance];
        if ([audioSession respondsToSelector:@selector(requestRecordPermission:)]) {
            [audioSession performSelector:@selector(requestRecordPermission:) withObject:^(BOOL granted) {
                if (granted) {
                    bCanRecord = YES;
                } else {
                    bCanRecord = NO;
                }
            }];
        }
    }
    
    NSLog(@"bCanRecord1 : %d",bCanRecord);
    return (bCanRecord == YES);
}
