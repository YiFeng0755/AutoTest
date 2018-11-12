package com.youme.im.client;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

public class CommonUtil {
	private static int MAX_SIZE = 4096;
	
	public static void main(String[] args) {
		String source = "aababababab";
		String key = "xxyyzz";
		System.out.println(CommonUtil.xorCrypt(key.getBytes(), 
				source.getBytes()));
	}
	
	/**
	 * 异或加密
	 * @param key 加密的key值 类型 byte[]
	 * @param data 待加密的源数据 类型 byte[]
	 * @return 返回结果
	 */
	public static byte [] xorCrypt(byte [] key, byte [] data) {
		byte [] out = new byte[data.length];
		for (int i = 0; i < data.length; ++i) {
			out[i] = (byte) (data[i] ^ key[i % key.length]);
		}
		return out;
	}
	
	 /**
	  * 字节数组转成16进制表示格式的字符串
	  *
	  * @param byteArray
	  *            需要转换的字节数组
	  * @return 16进制表示格式的字符串
	  **/
	 public static String toHexString(byte[] byteArray) {
	  if (byteArray == null || byteArray.length < 1)
	   throw new IllegalArgumentException("this byteArray must not be null or empty");
	 
	  final StringBuilder hexString = new StringBuilder();
	  for (int i = 0; i < byteArray.length; i++) {
	   if ((byteArray[i] & 0xff) < 0x10)//0~F前面不零
	    hexString.append("0");
	   hexString.append(Integer.toHexString(0xFF & byteArray[i]));
	  }
	  return hexString.toString().toLowerCase();
	 }
	
	public static byte [] youme_request(String host, int port, byte [] data) {
		Socket socket;
		byte [] rspData = null;
		try {
			socket = new Socket(host, port);
			DataInputStream inputStream = new DataInputStream(socket.getInputStream());
			DataOutputStream outputStream = new DataOutputStream(socket.getOutputStream());
			outputStream.write(data);
			
			rspData = new byte[MAX_SIZE];
			
		    int size = inputStream.read(rspData);
		    
		    byte [] result = new byte[size];
		    
		    System.arraycopy(rspData, 0, result, 0, size);
		    return result;
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return rspData;
	}
	public static int requestTcpServer() {
		return 0;
	}
}
