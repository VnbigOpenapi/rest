package com.vnbig.rest.stock;
import java.io.IOException;
import org.apache.http.HttpException;

/**
 * 行情，交易 REST API
 * @author yangsl
 *
 */
public interface IStockRestApi {

  /**
   * 行情
   * @param symbol btc_eth
   * @return
   * @throws IOException 
   * @throws HttpException 
  */
   public String ticker(String symbol) throws HttpException, IOException;
   
   /**
    * 交易信息
    * @param symbol
    * @return
    * @throws HttpException
    * @throws IOException
    */
   public String trades(String symbol) throws HttpException, IOException;

   /**
    * 获取用户信息
    * @return
    * @throws HttpException
    * @throws IOException
    */
   public String userinfo() throws HttpException, IOException;
}
