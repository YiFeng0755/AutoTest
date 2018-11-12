package com.xmeter.functions;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Collection;
import java.util.LinkedList;
import java.util.List;

import org.apache.jmeter.engine.util.CompoundVariable;
import org.apache.jmeter.functions.AbstractFunction;
import org.apache.jmeter.functions.InvalidVariableException;
import org.apache.jmeter.samplers.SampleResult;
import org.apache.jmeter.samplers.Sampler;

public class CurrentTime extends AbstractFunction {
	 private static final List<String> desc = new LinkedList<String>();
	    static {
	        desc.add("Get current time stamp");
	    }

	    //function名称
	    private static final String KEY = "__CurrentTime";

	    private static final int MAX_PARA_COUNT = 0;
	    private static final int MIN_PARA_COUNT = 0;

	    //传入参数的值
	    private Object[] values;
	    
	    public List<String> getArgumentDesc() {
	        return desc;
	    }

	    @Override
	    public String execute(SampleResult previousResult, Sampler currentSampler) throws InvalidVariableException {
	        try {
	            return Long.toString(System.currentTimeMillis() / 1000);
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
