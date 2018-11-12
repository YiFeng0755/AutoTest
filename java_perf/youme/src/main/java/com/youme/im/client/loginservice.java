package com.youme.im.client;
import java.util.Arrays;

import com.google.protobuf.InvalidProtocolBufferException;

import YOUMEServiceProtocol.YoumeLogin;
import YOUMEServiceProtocol.YoumeLogin.*;
import YOUMEServiceProtocol.YoumeLogin.LoginReq.Builder;
import YoumeUtil.NumberUtil;



public class loginservice {
	
	private String host = "106.75.35.102";
	private int port = 8082;
	
	public void login_request(String username) throws InvalidProtocolBufferException {
		byte [] head = new byte[16];
		
		short cmd = 1;
		int service_id = 660;
		
		NumberUtil.longToBytes(0L, head, 0);
		NumberUtil.shortToBytes((short) 0, head, 8);
		NumberUtil.shortToBytes(cmd, head, 10);
		NumberUtil.uintToBytes((int) service_id, head, 12);
		//填充用户名
		//ArrayList<Byte> buffer = new ArrayList<Byte>();
		
		byte [] reqBody = new byte[4096];
		
		byte [] msg2 = NumberUtil.ubyteToBytes(username.length());
	 	byte [] msg3 = CommonUtil.xorCrypt(head, username.getBytes());
	 	
	 	System.arraycopy(msg2, 0, reqBody, 16, 1);
	 	System.arraycopy(msg3, 0, reqBody, 17, msg3.length);

	 	Builder loginReq =  LoginReq.newBuilder();
	 	loginReq.setVersion(1);
	 	loginReq.setDeviceToken("123456789");
	 	loginReq.setPswd("123456");
	 	loginReq.setOsType(2);
	 	loginReq.setUserInfo("{\"tst\":1, \"f\": \"abc def\"}");
	 	
	 	int currentIndex = head.length + 1 + msg3.length;
	 	
	 	short ulenBody = (short) loginReq.build().getSerializedSize();
	 	byte [] bodymsg = CommonUtil.xorCrypt(head, loginReq.build().toByteArray());

	 	short dataSize = (short) (16 + 1 + msg3.length + ulenBody);
	 	
	 	System.arraycopy(bodymsg, 0, reqBody, currentIndex, ulenBody);
	 	NumberUtil.shortToBytes(dataSize, head, 8);
	 	System.arraycopy(head, 0, reqBody, 0, 16);
	 	
	 	byte [] readyData = Arrays.copyOfRange(reqBody, 0, dataSize);
	 	byte [] rspdata = CommonUtil.youme_request(host, port, readyData);
	 	//去头
	 	byte [] loginRspBytes = Arrays.copyOfRange(rspdata, 16, rspdata.length);
	 	YoumeLogin.LoginRsp loginRsp = YoumeLogin.LoginRsp.parseFrom(loginRspBytes);
	 	
	 	System.out.println(loginRsp.toString());
	}
	
	public static void main(String[] args) {
		loginservice client = new loginservice();
		try {
			client.login_request("123");
		} catch (InvalidProtocolBufferException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
