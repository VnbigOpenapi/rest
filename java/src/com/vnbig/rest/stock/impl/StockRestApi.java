package com.vnbig.rest.stock.impl;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import org.apache.http.HttpException;
import com.vnbig.rest.HttpUtilManager;
import com.vnbig.rest.MD5Util;
import com.vnbig.rest.StringUtil;
import com.vnbig.rest.stock.IStockRestApi;

public class StockRestApi implements IStockRestApi{    
	private String secret_key;
	
	private String api_key;
	
	private String url_prex;
	
	public StockRestApi(String url_prex,String api_key,String secret_key){
		this.api_key = api_key;
		this.secret_key = secret_key;
		this.url_prex = url_prex;
	}
	
	public StockRestApi(String url_prex){
		this.url_prex = url_prex;
	}
	
	/**
	 * 市场行情URL
	 */
	private final String TICKER_URL = "/ticker";
	
	private final String TRADES_URL = "/trades";
	
	/**
	 * 获取用户信息URL
	 */
	private final String USERINFO_URL = "/userinfo";
	
	@Override
	public String ticker(String symbol) throws HttpException, IOException {
		HttpUtilManager httpUtil = HttpUtilManager.getInstance();
		String param = "";
		if(!StringUtil.isEmpty(symbol )) {
			param+=symbol;
		}
		String result = httpUtil.requestHttpGet(url_prex, TICKER_URL, param);
	    return result;
	}
    
	@Override
	public String trades(String symbol) throws HttpException, IOException {
		HttpUtilManager httpUtil = HttpUtilManager.getInstance();
		String param = "";
		if(!StringUtil.isEmpty(symbol )) {
			param+=symbol;
		}
		String result = httpUtil.requestHttpGet(url_prex, TRADES_URL, param);
	    return result;
	}
	
	@Override
	public String userinfo() throws HttpException, IOException {
		// 构造参数签名
		Map<String, String> params = new HashMap<String, String>();
		params.put("api_key", api_key);
		String sign = MD5Util.buildMysignV1(params, this.secret_key);
		params.put("sign", sign);
		// 发送post请求
		HttpUtilManager httpUtil = HttpUtilManager.getInstance();
		String result = httpUtil.requestHttpPost(url_prex,this.USERINFO_URL,
				params);
		return result;
	}

	public String getSecret_key() {
		return secret_key;
	}

	public void setSecret_key(String secret_key) {
		this.secret_key = secret_key;
	}

	public String getApi_key() {
		return api_key;
	}

	public void setApi_key(String api_key) {
		this.api_key = api_key;
	}

	public String getUrl_prex() {
		return url_prex;
	}

	public void setUrl_prex(String url_prex) {
		this.url_prex = url_prex;
	}
}
