using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace Vnbig
{
    public class RequestVnbigAPI
    {
        private static string url = "http://api.vnbig.com";
        private static string apiKey = "";
        private static string apiKeySecret = "";

        public static string userinfo(string symbol, string apiKey, string secretkey)
        {
            return VnbigApiSend(new Dictionary<string, string>(), "/userinfo", symbol, apiKey, secretkey);
        }

        public static string trade(string symbol, string type, string price, string amount, string apiKey, string secretkey)
        {
            Dictionary<string, string> mParam = new Dictionary<string, string>();
            mParam.Add("symbol", symbol);
            mParam.Add("type", type);
            mParam.Add("price", price);
            mParam.Add("volume", amount);
            return VnbigApiSend(mParam, "/trade", symbol, apiKey, secretkey);
        }

        public static string order_info(string symbol, string order_id, string apiKey, string secretkey)
        {
            Dictionary<string, string> mParam = new Dictionary<string, string>();
            mParam.Add("symbol", symbol);
            mParam.Add("order_id", order_id);
            return VnbigApiSend(mParam, "/orderinfo", symbol, apiKey, secretkey);
        }

        public static string cancel_order(string symbol, string order_id, string apiKey, string secretkey)
        {
            Dictionary<string, string> mParam = new Dictionary<string, string>();
            mParam.Add("symbol", symbol);
            mParam.Add("order_id", order_id);
            return VnbigApiSend(mParam, "/cancelorder", symbol, apiKey, secretkey);
        }

        public static string VnbigApiSend(Dictionary<string, string> mParam, string mAction, string symbol, string apiKey, string secretkey)
        {
            mParam.Add("api_key", apiKey);
            string mPre = createLinkString(mParam);
            mPre += "&secret_key=" + apiKeySecret;
            string mMd5 = getMD5String(mPre);
            mPre += "&sign=" + mMd5;
            return requestHttpPost(url, mAction, mPre);
        }
        public static string requestHttpPost(string url_prex, string url, string paras)
        {
            string responseContent = "";
            HttpWebResponse httpWebResponse = null;
            StreamReader streamReader = null;
            try
            {
                //组装访问路径
                url = url_prex + url;
                //根据url创建HttpWebRequest对象
                HttpWebRequest httpWebRequest = (HttpWebRequest)WebRequest.Create(url);
                //设置请求方式和头信息
                httpWebRequest.Method = "POST";
                httpWebRequest.ContentType = "application/x-www-form-urlencoded";

                byte[] btBodys = Encoding.UTF8.GetBytes(paras);
                httpWebRequest.ContentLength = btBodys.Length;
                //将请求内容封装在请求体中
                httpWebRequest.GetRequestStream().Write(btBodys, 0, btBodys.Length);

                //获取响应
                httpWebResponse = (HttpWebResponse)httpWebRequest.GetResponse();
                //得到响应流
                streamReader = new StreamReader(httpWebResponse.GetResponseStream());
                //读取响应内容
                responseContent = streamReader.ReadToEnd();
                //关闭资源
                httpWebResponse.Close();
                streamReader.Close();
                //返回结果
                if (responseContent == null || "".Equals(responseContent))
                {
                    return "";
                }
            }
            catch (WebException ex)
            {
                HttpWebResponse response = (HttpWebResponse)ex.Response;
                using (Stream data = response.GetResponseStream())
                {
                    using (StreamReader reader = new StreamReader(data))
                    {
                        string text = reader.ReadToEnd();
                        return text;
                    }
                }
            }
            finally
            {
                if (httpWebResponse != null)
                {
                    httpWebResponse.Close();

                }
                if (streamReader != null)
                {
                    streamReader.Close();
                }
            }
            return responseContent;
        }

        /// <summary>
        /// 把数组所有元素排序，并按照“参数=参数值”的模式用"&amp;"字符拼接成字符串;
        /// </summary>
        /// <param name="paras"> 需要排序并参与字符拼接的参数组</param>
        /// <returns>拼接后字符串</returns>
        /// 

        public static String createLinkString(Dictionary<String, String> paras)
        {
            List<string> keys = new List<string>(paras.Keys);

            var paraSort = from objDic in paras orderby objDic.Key ascending select objDic;
            String prestr = "";
            int i = 0;
            foreach (KeyValuePair<String, String> kvp in paraSort)
            {
                if (i == keys.Count() - 1)
                {
                    // 拼接时，不包括最后一个&字符
                    prestr = prestr + kvp.Key + "=" + kvp.Value;
                }
                else
                {
                    prestr = prestr + kvp.Key + "=" + kvp.Value + "&";
                }
                i++;
                if (i == keys.Count())
                {
                    break;
                }
            }
            return prestr;
        }

        private static char[] HEX_DIGITS = new char[]{'0', '1', '2', '3', '4', '5',
            '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
        /// <summary>
        /// 生成32位大写MD5值
        /// </summary>
        public static String getMD5String(String str)
        {

            if (str == null || str.Trim().Length == 0)
            {
                return "";
            }
            byte[] bytes = System.Text.Encoding.Default.GetBytes(str);
            MD5CryptoServiceProvider md = new MD5CryptoServiceProvider();
            bytes = md.ComputeHash(bytes);
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < bytes.Length; i++)
            {
                sb.Append(HEX_DIGITS[(bytes[i] & 0xf0) >> 4] + ""
                        + HEX_DIGITS[bytes[i] & 0xf]);
            }
            return sb.ToString();
        }
    }
}
