package com.xmeter.functions;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Collection;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;

import org.apache.jmeter.engine.util.CompoundVariable;
import org.apache.jmeter.functions.AbstractFunction;
import org.apache.jmeter.functions.InvalidVariableException;
import org.apache.jmeter.samplers.SampleResult;
import org.apache.jmeter.samplers.Sampler;

public class Sha1Functions extends AbstractFunction{
    //自定义function的描述
    private static final List<String> desc = new LinkedList<String>();
    static {
        desc.add("Sha1 a string with a current timestamp");
        desc.add("Add current time stamp to the tail.");
    }

    //function名称
    private static final String KEY = "__Sha1Func";

    private static final int MAX_PARA_COUNT = 1;
    private static final int MIN_PARA_COUNT = 1;

    //传入参数的值
    private Object[] values;
    
    private static String getSha1(String string) {
		MessageDigest digest;
		StringBuffer hexString = new StringBuffer(); 
		try {
			digest = MessageDigest.getInstance("SHA-1");
			digest.update(string.getBytes());
			byte msgdigest[] = digest.digest();
			 
	        // 字节数组转换为 十六进制 数  
	        for (int i = 0; i < msgdigest.length; i++) {  
	            String shaHex = Integer.toHexString(msgdigest[i] & 0xFF);  
	            if (shaHex.length() < 2) {  
	                hexString.append(0);  
	            }  
	            hexString.append(shaHex);  
	        } 
		} catch (NoSuchAlgorithmException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return hexString.toString();	
	}
    public static void main(String[] args) {
    	
	}
    public List<String> getArgumentDesc() {
        return desc;
    }

    @Override
    public String execute(SampleResult previousResult, Sampler currentSampler) throws InvalidVariableException {
        try {
            String str1 = ((CompoundVariable)this.values[0]).execute().trim();
            return Sha1Functions.getSha1(str1);
        } catch(Exception ex) {
            throw new InvalidVariableException(ex);
        }
    }

    @Override
    public String getReferenceKey() {
        return KEY;
    }

    @Override
    public void setParameters(Collection<CompoundVariable> parameters) throws InvalidVariableException {
         checkParameterCount(parameters, MIN_PARA_COUNT, MAX_PARA_COUNT); //检查参数的个数是否正确
         values = parameters.toArray(); //将值存入类变量中
    }
  
}