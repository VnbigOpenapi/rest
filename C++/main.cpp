#include <QCoreApplication>
#include <iostream>
#include <curl/curl.h>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QMap>


#define BUG qDebug()



typedef struct Datasource
{
    double price; // price;
    double amount; // amount;
    long   timestamp ;// ;
}vnbdepth;

struct Order {
    long long id;
    QString crtime;
    int type;
    double price;
    double count;
    double volume;
    double novolume;
    QString name;
};


QList <vnbdepth> list_asks;

QList <vnbdepth> list_bids;







using namespace std;

QString VNBIG_ADDR = "api.vnbig.com";

size_t call_wirte_func(const char *ptr, size_t size, size_t nmemb, std::string *stream)
{
    size_t len = size * nmemb;
    stream->append(ptr, len);
    return len;
}
size_t header_callback(const char  *ptr, size_t size, size_t nmemb, std::string *stream)
{
    size_t len = size * nmemb;
    stream->append(ptr, len);
    return len;
}

int getbuffer(string pUrl, string &szbuffer)
{
    BUG << pUrl.c_str() ;
  CURL *curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, pUrl.c_str());

#ifdef SKIP_PEER_VERIFICATION

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_DEFAULT);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, call_wirte_func);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &szbuffer);
    curl_easy_setopt( curl, CURLOPT_TIMEOUT, 20 );//接收数据时超时设置，如果10秒内数据未接收完，直接退出
    curl_easy_setopt( curl, CURLOPT_CONNECTTIMEOUT, 20 );//连接超时，这个数值如果设置太短可能导致数据请求不到就断开了
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK){
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
      return 0;
    }
    /* always cleanup */
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();

  return 1;
}


bool postbuffer(string postFields, string url,string &szbuffer) {
    BUG <<"postbuffer : :";
    BUG << "url.c_str()" <<url.c_str() << " " <<  postFields.c_str() ;
    CURL *curl_handle = NULL;
    CURLcode res = CURLE_FAILED_INIT;
    string szheader_buffer;
    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, "contentType: application/x-www-form-urlencoded");
    chunk = curl_slist_append(chunk, "Accept-Encoding: gzip, deflate");
    chunk = curl_slist_append(chunk, "Accept-Language: en-US,en;q=0.8");
    chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/43.0.2357.130 Safari/537.36");

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, 5);   //设置重定向的最大次数
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1);   //设置301、302跳转跟随location
    curl_easy_setopt(curl_handle, CURLOPT_COOKIEFILE, "cookie.dat");
    curl_easy_setopt(curl_handle, CURLOPT_COOKIEJAR, "cookie.dat");
    //curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0);
    //curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0);
    //curl_easy_setopt(curl_handle, CURLOPT_SSLVERSION, CURL_SSLVERSION_DEFAULT);
    //抓取内容后，回调函数
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, call_wirte_func);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &szbuffer);
    //抓取头信息，回调函数
    curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, header_callback);
    curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, &szheader_buffer);
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, postFields.c_str());

    curl_easy_setopt( curl_handle, CURLOPT_TIMEOUT, 20 );//接收数据时超时设置，如果10秒内数据未接收完，直接退出
    curl_easy_setopt( curl_handle, CURLOPT_CONNECTTIMEOUT, 20 );//连接超时，这个数值如果设置太短可能导致数据请求不到就断开了

    res = curl_easy_perform(curl_handle);
    curl_easy_cleanup(curl_handle);
    curl_slist_free_all(chunk);
    chunk = NULL;
    curl_global_cleanup();
    BUG << "postbuffer res "<<res ;
    if (res == CURLE_OK)
    {
        return true;
    }
    else
    {
        BUG <<"res === false";
    }
    return false;
}



QString doOrder(QString _apikey,QString _seckey,QString _symbol,QString _type,QString _price,QString _vol){
    QString url = QString(VNBIG_ADDR)+"/trade";

    QString volume = QString("volume=")+_vol+"&";

    QString apikey = QString("api_key=")+_apikey+"&";

    QString price  = QString("price=")+_price+"&";

    QString symbol = QString("symbol=")+_symbol+"&";

    QString type   = QString("type=")+_type+"&";

    QString seckey = QString("secret_key=")+_seckey;

    QString sign   = QString("sign=")+QCryptographicHash::hash((apikey+price+symbol+type+volume+seckey).toLatin1(),QCryptographicHash::Md5).toHex().toUpper()+"&";

    QString volume1  = QString("volume=")+_vol;
    BUG << apikey+price+symbol+type+volume+seckey;
    seckey         = seckey+"&";
    BUG << sign;

    QString finalurl= apikey+price+sign+symbol+type+volume1;
    BUG << "finalurl === " <<finalurl;
    string result;
    postbuffer(finalurl.toStdString(),url.toStdString(),result);
    BUG << "doOrder postbuffer "<< postbuffer(finalurl.toStdString(),url.toStdString(),result);
    cout << "doOrder RESULT.C_STR() == " << result << endl;
    return result.c_str();
}

QString doBuy(QString _apikey,QString _seckey,QString _symbol,QString _price,QString _vol){
    QString doOrderRST;
    doOrderRST = doOrder(_apikey,_seckey,_symbol,"buy",_price,_vol);
    BUG << "doOrderRST == " << doOrderRST << endl;
    return doOrder(_apikey,_seckey,_symbol,"buy",_price,_vol);
}
QString doSell(QString _apikey,QString _seckey,QString _symbol,QString _price,QString _vol){
    return doOrder(_apikey,_seckey,_symbol,"sell",_price,_vol);
}


string doCancel2(QString _apikey,QString _seckey,QString _symbol,QString _orderid){
    string result;
    QString url = QString(VNBIG_ADDR)+"/cancelorder";
    QString apikey = QString("api_key=")+_apikey+"&";
    QString seckey = QString("secret_key=")+_seckey;
    QString symbol = QString("symbol=")+_symbol+"&";
    QString orderid = QString("order_id=")+_orderid+"&";
    QString sign   = QString("sign=")+QCryptographicHash::hash((apikey+orderid+symbol+seckey).toLatin1(),QCryptographicHash::Md5).toHex().toUpper()+"&";
    QString symbol1= QString("symbol=")+_symbol;

    QString finalurl= apikey+orderid+sign+symbol1;
    postbuffer(finalurl.toStdString(),url.toStdString(),result);
    return result;
}

string getOrderSelf2(QString _symbol,QString _type,QString _apikey,QString _seckey,QMap<double,Order> &listBuy,QMap<double,Order> &listSell){
    string result;
    QString url = QString(VNBIG_ADDR)+"/trades";
    QString apikey = QString("api_key=")+_apikey+"&";
    QString symbol = QString("symbol=")+_symbol+"&";
    QString type   = QString("type=")+_type+"&";
    QString seckey = QString("secret_key=")+_seckey;
    QString sign   = QString("sign=")+QCryptographicHash::hash((apikey+symbol+type+seckey).toLatin1(),QCryptographicHash::Md5).toHex().toUpper()+"&";
    QString type1  = QString("type=")+_type;

    QString finalurl= apikey+sign+symbol+type1;
    postbuffer(finalurl.toStdString(),url.toStdString(),result);

    QJsonDocument jsonDoc = QJsonDocument::fromJson(result.c_str());
    QJsonObject jsonObject = jsonDoc.object();
    QJsonValue jsonCode = jsonObject["result"];
    BUG<<"getOrderSelf2"<<jsonCode.toBool()<<result.c_str();
    if(!jsonCode.toBool()){
        exit(-1);
    }
    QJsonArray jsonData = jsonObject["orders"].toArray();
    for(int i=0;i<jsonData.count();i++){
        Order curOrder = {0};
        QJsonObject jsonObj;
        jsonObj = jsonData.at(i).toObject();
        curOrder.id = jsonObj["order_id"].toVariant().toLongLong();
        curOrder.price = jsonObj["price"].toDouble();
        curOrder.volume = jsonObj["volume"].toDouble();
        curOrder.name = jsonObj["symbol"].toString();
        if(_type=="buy"){
            curOrder.type = 0;
            listBuy.insert(curOrder.price,curOrder);
        }else if(_type=="sell"){
            curOrder.type = 1;
            listSell.insert(curOrder.price,curOrder);
        }else{
            BUG << "unknow type" << jsonObj;
        }
    }

    return result;
}

QStringList getOkPriceList(QString pCoin){
    QString url = "http://api.vnbig.com/depth/";
    url = url+pCoin;//+"btc_etc";
    BUG<< "URL = " << url; //  yxf 2018.5.9
    string buff;
    //QByteArray result = qtGet(url);
    if(!getbuffer(url.toStdString().c_str(),buff)){
        exit(-1);
    }
    QStringList ret;


    QJsonParseError json_error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(buff.c_str(),&json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        BUG << "PARSE Json ok";
    }
    vnbdepth Sbids {0.0,0.0};    //  bids struct
    vnbdepth Sasks {0.0,0.0};    // askss struct

    QList<vnbdepth>::iterator bids_it;
    QList<vnbdepth>::iterator asks_it;

    QJsonObject asksobject = jsonDoc.object();
    QJsonObject bidsobject = jsonDoc.object();
    QJsonObject timeobj = jsonDoc.object();
    QJsonObject bids_obj, ask_obj;

    QJsonValue vnb_bids = bidsobject["bids"];
    QJsonValue vnb_asks = asksobject["asks"];
    QJsonValue times = timeobj["timeStamp"];
    QJsonValue bids_price, asks_price;
    QJsonArray bidsarray = vnb_bids.toArray();
    QJsonArray asksarray = vnb_asks.toArray();

    BUG <<"asksarray size ==== " << asksarray.size();

    for(int i = 0; i < bidsarray.size(); i++)
    {
        bids_obj = bidsarray[i].toObject();

        if(bids_obj.contains("price")) {Sbids.price  = bids_obj.take("price").toDouble(); }

        if(bids_obj.contains("volume")) {Sbids.amount = bids_obj.take("volume").toDouble(); }
        list_bids.insert(i,Sbids);
    }


    for(int n = 0; n < asksarray.size(); n++)
    {
        ask_obj = asksarray[n].toObject();

        if(ask_obj.contains("price")) { Sasks.price = ask_obj.take("price").toDouble(); }

        if(ask_obj.contains("volume")) {Sasks.amount = ask_obj.take("volume").toDouble(); }

        list_asks.insert(n, Sasks);
    }

    for(asks_it = list_asks.begin(); asks_it != list_asks.end(); asks_it++)
    {
        BUG << "asks price ==== "<<(*asks_it).price;
        BUG << "asks volume ==== "<<(*asks_it).amount;
    }

     QString asksvaule(jsonDoc.toJson(QJsonDocument::Compact));

     ret.append(asksvaule);

    return ret;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //QString doBuy(QString _apikey,QString _seckey,QString _symbol,QString _price,QString _vol)
    BUG <<"doBuy :: "<<  doBuy("XXXXX","XXXXX","btc_etc","0.000005","0.1");
    return 0;
}
