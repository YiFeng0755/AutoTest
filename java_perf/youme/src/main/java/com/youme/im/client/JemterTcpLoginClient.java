package com.youme.im.client;
import com.google.protobuf.InvalidProtocolBufferException;
import YOUMEServiceProtocol.YoumeLogin;
import YOUMEServiceProtocol.YoumeLogin.LoginReq;
import YOUMEServiceProtocol.YoumeLogin.LoginReq.Builder;
import YoumeUtil.NumberUtil;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Arrays;
import org.apache.jmeter.protocol.tcp.sampler.*;

public class JemterTcpLoginClient extends AbstractTCPClient {
	private static final org.slf4j.Logger log = org.slf4j.LoggerFactory.getLogger(JmeterTcpClient.class);
	
	public String read(InputStream iStream) throws ReadException {
		ByteArrayOutputStream byteO = new ByteArrayOutputStream();
	 	//去头
	 	byte[] buffer = new byte[4096];
	 	int x = 0;
		try {
			x = iStream.read(buffer);
		} catch (IOException e1) {
			log.debug("read buffer failed!!!");
			e1.printStackTrace();
		}
        byte [] data = Arrays.copyOfRange(buffer, 16, x); 
        log.debug("buffer is " + data); 
        byteO.write(data, 0, data.length);
        //}
        YoumeLogin.LoginRsp loginRsp = null;
		try {
			loginRsp = YoumeLogin.LoginRsp.parseFrom(data);
		} catch (InvalidProtocolBufferException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return null;
		}   	
        return loginRsp.toString();
	}

	public void write(OutputStream outputStream, InputStream inputStream) throws IOException {
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
		
		byte [] is = new byte[1024];
		int x = inputStream.read(is);
		byte [] username = Arrays.copyOf(is, x);
		
		log.debug("username is " + username);
		
		byte [] msg2 = NumberUtil.ubyteToBytes(username.length);
	 	byte [] msg3 = CommonUtil.xorCrypt(head, username);
	 	
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
	 	
	 	log.debug("body message " + bodymsg);
	 	short dataSize = (short) (16 + 1 + msg3.length + ulenBody);
	 	
	 	System.arraycopy(bodymsg, 0, reqBody, currentIndex, ulenBody);
	 	NumberUtil.shortToBytes(dataSize, head, 8);
	 	System.arraycopy(head, 0, reqBody, 0, 16);
	 	
	 	byte [] readyData = Arrays.copyOfRange(reqBody, 0, dataSize);
	 	
	 	outputStream.write(readyData);
	 	outputStream.flush();
	 	log.debug("Output Stream write bytes " + readyData);
	}

	public void write(OutputStream os, String s) throws IOException {
		byte [] head = new byte[16];
		
		short cmd = 1;
		//String s 传进来username和service_id
		
		String[] params = s.split(",");
		if (s.isEmpty() || params.length != 3) {
			log.debug("Input string is empty Or param's number is incorrect!!!");
			return;
		}

		int service_id = Integer.parseInt(params[0]);
		String username = params[1];
		String userinfo = params[2];
		
		NumberUtil.longToBytes(0L, head, 0);
		NumberUtil.shortToBytes((short) 0, head, 8);
		NumberUtil.shortToBytes(cmd, head, 10);
		NumberUtil.uintToBytes((int) service_id, head, 12);
		//填充用户名
		//ArrayList<Byte> buffer = new ArrayList<Byte>();
		
		byte [] reqBody = new byte[4096];
		byte [] is = new byte[1024];
		
		byte [] msg2 = NumberUtil.ubyteToBytes(username.length());
	 	byte [] msg3 = CommonUtil.xorCrypt(head, username.getBytes());
	 	
	 	System.arraycopy(msg2, 0, reqBody, 16, 1);
	 	System.arraycopy(msg3, 0, reqBody, 17, msg3.length);

	 	Builder loginReq =  LoginReq.newBuilder();
	 	loginReq.setVersion(1);
	 	loginReq.setDeviceToken("123456789");
	 	loginReq.setPswd("123456");
	 	loginReq.setOsType(2);
	 	loginReq.setUserInfo(userinfo);
	 	
	 	int currentIndex = head.length + 1 + msg3.length;
	 	
	 	short ulenBody = (short) loginReq.build().getSerializedSize();
	 	byte [] bodymsg = CommonUtil.xorCrypt(head, loginReq.build().toByteArray());

	 	short dataSize = (short) (16 + 1 + msg3.length + ulenBody);
	 	
	 	System.arraycopy(bodymsg, 0, reqBody, currentIndex, ulenBody);
	 	NumberUtil.shortToBytes(dataSize, head, 8);
	 	System.arraycopy(head, 0, reqBody, 0, 16);
	 	
	 	byte [] readyData = Arrays.copyOfRange(reqBody, 0, dataSize);
	 	
	 	os.write(readyData);
	 	os.flush();
	 	log.debug("Output Stream write bytes " + readyData);
	}

	public void setupTest() {
		// TODO Auto-generated method stub
		
	}

	public void teardownTest() {
		// TODO Auto-generated method stub
		
	}

	public byte getEolByte() {
		// TODO Auto-generated method stub
		return 0;
	}

	public String getCharset() {
		// TODO Auto-generated method stub
		return null;
	}

	public void setEolByte(int eolInt) {
		// TODO Auto-generated method stub
		
	}
	
}
