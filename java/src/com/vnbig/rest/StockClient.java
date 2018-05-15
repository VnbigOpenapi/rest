package com.vnbig.rest;
import java.io.IOException;
import org.apache.http.HttpException;
import com.vnbig.rest.stock.IStockRestApi;
import com.vnbig.rest.stock.impl.StockRestApi;
/**
 * 现货 REST API 客户端请求
 * @author yangsl
 *
 */
public class StockClient {
	public static void main(String[] args) throws HttpException, IOException{	
	    String api_key = "XXXXX";  //VNBIG申请的api_key
       	String secret_key = "XXXXX";  //VNBIG 申请的secret_key
 	    String url_prex = "http://api.vnbig.com";  //注意：请求URL http://api.vnbig.com
	    /**
	     * 公共接口
	     * 
	    */
	    IStockRestApi stockGet = new StockRestApi(url_prex);
	    //市场行情
	    String ticker = stockGet.ticker("btc_etc");
	    //返回市场行情的数据
	    System.out.println(ticker);
	    
	    //获取交易信息
	    String trades = stockGet.trades("btc_etc");
	    //返回交易信息的数据
	    System.out.println(trades);
		
	    /**
	     * 私有接口
	     * 需要进行身份验证
	     * 根据api_key和secret_key
	     * 发送post请求之前，程序会做自动加密，生成签名。
	     * 
	    */
	    IStockRestApi stockPost = new StockRestApi(url_prex, api_key, secret_key);
	    //获取用户信息
	    String userinfo = stockPost.userinfo();
	    //返回户信息的数据
	    System.out.println(userinfo);
		
	}
}
