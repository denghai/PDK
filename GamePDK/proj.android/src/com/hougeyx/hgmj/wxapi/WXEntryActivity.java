package com.hougeyx.hgmj.wxapi;

import java.io.File;

import org.cocos2dx.cpp.Native;
import org.cocos2dx.cpp.Util;

import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.modelmsg.SendAuth;
import com.tencent.mm.sdk.modelmsg.SendMessageToWX;
import com.tencent.mm.sdk.modelmsg.WXAppExtendObject;
import com.tencent.mm.sdk.modelmsg.WXImageObject;
import com.tencent.mm.sdk.modelmsg.WXMediaMessage;
import com.tencent.mm.sdk.modelmsg.WXTextObject;
import com.tencent.mm.sdk.modelmsg.WXWebpageObject;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;

public class WXEntryActivity extends Activity implements IWXAPIEventHandler {
	public static final String APP_ID = "wx38582698b2312a77";
	public static final String AppSecret = "9b955664a4b8bb7ec9966412d0a18266";
	public static String ReqWXPay;
	public static String ReqWxLogin;
	public static String ReqWxShareImg;
	public static String ReqWxShareTxt;
	public static String ReqWxShareUrl;
	private static final int SceneSession = 0;
	private static final int SceneTimeline = 1;
	private static final int THUMB_SIZE = 150;
	private static final int TIMELINE_SUPPORTED_VERSION = 553779201;
	public static String Tag = "WXEntryActivity";
	public static boolean bLogonWX;
	private static WXEntryActivity sContext = null;
	private IWXAPI api;

	static {
		bLogonWX = false;
		ReqWxLogin = "ReqWxLogin";
		ReqWxShareImg = "ReqWxShareImg";
		ReqWxShareTxt = "ReqWxShareTxt";
		ReqWxShareUrl = "ReqWxShareUrl";
		ReqWXPay = "ReqWXPay";
	}

	private String buildTransaction(String paramString) {
		String str;
		if (paramString != null)
			str = paramString + System.currentTimeMillis();
		else
			str = String.valueOf(System.currentTimeMillis());
		return str;
	}

	public void onCreate(Bundle savedInstanceState) {	        
		super.onCreate(savedInstanceState);
	    sContext = this;
	    Log.d(Tag,"onCreate");
	    
		Intent intent = getIntent();
	
		api = WXAPIFactory.createWXAPI(this,APP_ID, true);
	    api.registerApp(APP_ID);
	    api.handleIntent(intent, this);
	    
		if (intent.hasExtra(ReqWxLogin) && !bLogonWX) 
		{
			reqLogin();
		}
		else if(intent.hasExtra(ReqWxShareImg))
		{
			String ImgPath = intent.getStringExtra("ImgPath");
			int nType = intent.getIntExtra("ShareType",0);
			 reqShareImg(ImgPath,nType);
		}
		else if(intent.hasExtra(ReqWxShareTxt))
		{
			String ShareText = intent.getStringExtra("ShareText");
			int nType = intent.getIntExtra("ShareType",0);
			reqShareTxt(ShareText,nType);
		}
		else if(intent.hasExtra(ReqWxShareUrl))
		{
			String ShareUrl = intent.getStringExtra("ShareUrl");
			String ShareTitle = intent.getStringExtra("ShareTitle");
			String ShareDesc = intent.getStringExtra("ShareDesc");
			int nType = intent.getIntExtra("ShareType",0);
			reqShareUrl(ShareUrl,ShareTitle,ShareDesc,nType);
		}
		finish();
	}

	protected void onNewIntent(Intent paramIntent) {
		super.onNewIntent(paramIntent);
		setIntent(paramIntent);
		this.api.handleIntent(paramIntent, this);
	}

	public void onReq(BaseReq paramBaseReq) {
		switch (paramBaseReq.getType()) {
		case 3:
			Log.d(Tag, "onReq:COMMAND_GETMESSAGE_FROM_WX");
			break;
		case 4:
			Log.d(Tag, "onReq:COMMAND_SHOWMESSAGE_FROM_WX");
		}
		Log.d(Tag, "onReq:" + paramBaseReq.getType());
	}

	public void onResp(BaseResp paramBaseResp) {
		Log.d(Tag, "errCode:" + paramBaseResp.errCode);
		switch (paramBaseResp.errCode) {
		case -3:
		case -1:
		default:
			Native.WxLoginGetAccessToken("REE_Unknow");
			break;
		case -4:
			Native.WxLoginGetAccessToken("ERR_AUTH_DENIED");
			break;
		case -2:
			Native.WxLoginGetAccessToken("ERR_USER_CANCEL");
			break;
		case 0:
			String str = ((SendAuth.Resp) paramBaseResp).code;
			str = APP_ID + ":" + AppSecret + ":" + str + ":authorization_code";
			Log.d(Tag, str);
			Native.WxLoginGetAccessToken(str);
		}
	}

	public void reqLogin() {
		final SendAuth.Req localReq = new SendAuth.Req();
		localReq.scope = "snsapi_userinfo";
		localReq.state = "carjob_wx_login";
		bLogonWX = this.api.sendReq(localReq);
		Log.d(Tag, "reqLogin" + bLogonWX);
	}

	public void reqShareImg(String paramString, int paramInt) {
		if (new File(paramString).exists()) {
			Object localObject1 = BitmapFactory.decodeFile(paramString);
			Object localObject2 = new WXImageObject((Bitmap) localObject1);
			WXMediaMessage localWXMediaMessage = new WXMediaMessage();
			localWXMediaMessage.mediaObject = ((WXMediaMessage.IMediaObject) localObject2);
			localObject2 = Bitmap.createScaledBitmap((Bitmap) localObject1, 128, 72, true);
			((Bitmap) localObject1).recycle();
			localWXMediaMessage.thumbData = Util.bmpToByteArray((Bitmap) localObject2, true);
			localObject1 = new SendMessageToWX.Req();
			((SendMessageToWX.Req) localObject1).transaction = buildTransaction("img");
			((SendMessageToWX.Req) localObject1).message = localWXMediaMessage;
			if (paramInt != 1) {
				if (paramInt == 0)
					((SendMessageToWX.Req) localObject1).scene = 0;
			} else
				((SendMessageToWX.Req) localObject1).scene = 1;
			this.api.sendReq((BaseReq) localObject1);
			Log.d(Tag, "reqShare Ok:" + paramString);
		} else {
			Log.d(Tag, "reqShare file not exists:" + paramString);
		}
	}

	public void reqShareTxt(String paramString, int paramInt) {
		Object localObject = new WXTextObject();
		((WXTextObject) localObject).text = paramString;
		WXMediaMessage localWXMediaMessage = new WXMediaMessage();
		localWXMediaMessage.mediaObject = ((WXMediaMessage.IMediaObject) localObject);
		localWXMediaMessage.description = paramString;
		localObject = new SendMessageToWX.Req();
		((SendMessageToWX.Req) localObject).transaction = buildTransaction("text");
		((SendMessageToWX.Req) localObject).message = localWXMediaMessage;
		if (paramInt != 1) {
			if (paramInt == 0)
				((SendMessageToWX.Req) localObject).scene = 0;
		} else
			((SendMessageToWX.Req) localObject).scene = 1;
		this.api.sendReq((BaseReq) localObject);
		Log.d(Tag, "reqShareTxt Ok:" + paramString);
	}

	public void reqShareTxtCB(String paramString, int paramInt) {
		Object localObject = new WXAppExtendObject("lallalallallal", "filePath");
		if (!((WXAppExtendObject) localObject).checkArgs())
			Log.d(Tag, "reqShareTxtCB Error:" + paramString);
		WXMediaMessage localWXMediaMessage = new WXMediaMessage();
		localWXMediaMessage.title = "11";
		localWXMediaMessage.description = "22";
		localWXMediaMessage.mediaObject = ((WXMediaMessage.IMediaObject) localObject);
		localObject = new SendMessageToWX.Req();
		((SendMessageToWX.Req) localObject).transaction = buildTransaction("appdata");
		((SendMessageToWX.Req) localObject).message = localWXMediaMessage;
		if (paramInt != 1) {
			if (paramInt == 0)
				((SendMessageToWX.Req) localObject).scene = 0;
		} else
			((SendMessageToWX.Req) localObject).scene = 1;
		this.api.sendReq((BaseReq) localObject);
		Log.d(Tag, "reqShareTxtCB Ok:" + paramString);
	}

	public void reqShareUrl(String paramString1, String paramString2, String paramString3, int paramInt) {
		Object localObject = new WXWebpageObject();
		((WXWebpageObject) localObject).webpageUrl = paramString1;
		WXMediaMessage localWXMediaMessage = new WXMediaMessage();
		localWXMediaMessage.mediaObject = ((WXMediaMessage.IMediaObject) localObject);
		localWXMediaMessage.title = paramString2;
		localWXMediaMessage.description = paramString3;
		localObject = new SendMessageToWX.Req();
		((SendMessageToWX.Req) localObject).transaction = buildTransaction("webpage");
		((SendMessageToWX.Req) localObject).message = localWXMediaMessage;
		if (paramInt != 1) {
			if (paramInt == 0)
				((SendMessageToWX.Req) localObject).scene = 0;
		} else
			((SendMessageToWX.Req) localObject).scene = 1;
		this.api.sendReq((BaseReq) localObject);
		Log.d(Tag, "reqShareUrl Ok:" + paramString1);
	}
}