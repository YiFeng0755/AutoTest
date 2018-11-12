package com.youme.im.client;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Arrays;

import org.apache.jmeter.protocol.tcp.sampler.AbstractTCPClient;
import org.apache.jmeter.protocol.tcp.sampler.ReadException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.InvalidProtocolBufferException;

import YOUMEServiceProtocol.YoumeLogin;
import YOUMEServiceProtocol.YoumeLogin.LoginReq;
import YOUMEServiceProtocol.YoumeLogin.LoginReq.Builder;
import YoumeUtil.NumberUtil;

public class JmeterTcpClient extends AbstractTCPClient{
	Logger log = LoggerFactory.getLogger(JmeterTcpClient.class);

	public void write(OutputStream outputStream, InputStream inputStream) throws IOException {
		byte [] head = new byte[16];
		
		short cmd = 1;
		int service_id = 660;
		
		NumberUtil.longToBytes(0L, head, 0);
		NumberUtil.shortToBytes((short) 0, head, 8);
		NumberUtil.shortToBytes(cmd, head, 10);
		NumberUtil.uintToBytes((int) service_id, head, 12);
		//����û���
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
		//String s ������username��service_id
		
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
		//����û���
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

	public String read(InputStream is) throws ReadException {
		ByteArrayOutputStream byteO = new ByteArrayOutputStream();
	 	//ȥͷ
	 	byte[] buffer = new byte[4096];
	 	int x = 0;
        //while ((x = iStream.read(buffer)) > -1) {
        byte [] data = Arrays.copyOfRange(buffer, 16, x); 
            //ȥ��ͷ
        log.debug("buffer is " + data); 
        byteO.write(data, 0, data.length);
       	
        return byteO.toString();
	}
	
	public static void main(String[] args) {
		JmeterTcpClient client = new JmeterTcpClient();
		System.out.println("client class");
	}
}
