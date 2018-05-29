#!/usr/bin/env python
# encoding=utf8

from locust import HttpLocust, TaskSet, task
import json, random, hashlib
from urllib import parse
import operator
# api_key 和 secret_key
api = {'XXXXX':'XXXXX'}

def signStr(data):
    key = random.choice(list(api.keys()))
    data['api_key'] = key
    sort_1 = sorted(data.items(),key=operator.itemgetter(0))
    sort_2 =  dict(sort_1)
    sort_2['secret_key'] = api[key]
    return parse.urlencode(sort_2)

def md5(str):
    m = hashlib.md5()
    str_t = str.encode('utf-8')
    m.update(str_t)
    return m.hexdigest().upper()

def reqParam(data):
    sign_1 = signStr(data)
    data['sign'] = md5(sign_1)
    return data

class WebsiteTasks(TaskSet):
    def on_start(self):
        self.headers = { 'Host': 'test1.api.vnbig.com', 'Content-origin': 'pc', 'Cookie': '__zlcmid=mXhMJt5WRRzrau; lang=zh'
            , 'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.89 Safari/537.36'
            , 'Connection':'close', 'Content-Type': 'application/x-www-form-urlencoded;charset=UTF-8', 'Accept-Language': 'zh-CN,zh;q=0.8'
            , 'Accept': 'application/json, text/plain, */*', 'Origin': 'http://test1.api.vnbig.com', 'Content-Language': 'zh-cn' }
    '''
    @task(1)
    def list(self):
        with self.client.get('/list', params=None, headers=self.headers, catch_response=True) as response:
            self.dataCheck(response)

    @task(1)
    def ticker(self):
        with self.client.get('/ticker/btc_vnb', params=None, headers=self.headers, catch_response=True) as response:
            self.dataCheck(response)

    @task(1)
    def depth(self):
        with self.client.get('/depth/eth_btc', params=None, headers=self.headers, catch_response=True) as response:
            self.dataCheck(response)

    @task(1)
    def tradesinfo(self):
        with self.client.get('/trades/btc_usdt', params=None, headers=self.headers, catch_response=True) as response:
            self.dataCheck(response)

    @task(1)
    def kline(self):
        temp = {'symbol':'btc_vnb"', 'type':'30min'}
        data = parse.urlencode(temp)
        with self.client.post('/kline', data=data, headers=self.headers, catch_response=True) as response:
            self.dataCheck(response)
    '''
    @task(1)
    def entrustTrade(self):

        temp = {'symbol':random.choice(['btc_eos', 'btc_usdt', 'btc_eth', 'btc_etc', 'btc_bch'
                , 'eth_eos', 'eth_ltc', 'eth_etc','eth_bch'])
            , 'type':random.choice(['buy', 'sell', 'buy_market', 'sell_market'])
                , 'price':1.00000005, 'volume':6, 'amount':1.00000005}
        '''
        #temp   = {'symbol':random.choice(['btc_eos', 'btc_etc', 'btc_eth', 'btc_ltc'])
        temp   = {'symbol':'btc_etc'
            #, 'type':random.choice(['buy', 'sell', 'buy_market', 'sell_market'])
            , 'type':'buy'
            , 'price':1.00000005, 'volume':6, 'amount':1.00000005}
        '''
        data = parse.urlencode(reqParam(temp))
        with self.client.post('/trade', data=data, headers=self.headers, catch_response=True) as response:
            self.dataCheck(response)
    '''
    @task(1)
    def orderinfo(self):
        temp = {'symbol':random.choice(['btc_vnb', 'btc_usdt', 'eth_btc']), 'order_id':-1}
        data = parse.urlencode(reqParam(temp))
        with self.client.post('/orderinfo', data=data, headers=self.headers, catch_response=True) as response:
            self.dataCheck(response)

    @task(1)
    def trades(self):
        temp = {'symbol':random.choice(['btc_vnb', 'btc_usdt', 'eth_btc']), 'type':random.choice(['buy', 'sell', 'buy_market', 'sell_market'])}
        data = parse.urlencode(reqParam(temp))
        with self.client.post('/trades', data=data, headers=self.headers, catch_response=True) as response:
            self.dataCheck(response)

    @task(1)
    def userinfo(self):
        temp = {}
        data = parse.urlencode(reqParam(temp))
        with self.client.post('/userinfo', data=data, headers=self.headers, catch_response=True) as response:
            self.dataCheck(response)

    @task(1)
    def records(self):
        temp = {'symbol': 'btc_vnb'}
        data = parse.urlencode(reqParam(temp))
        with self.client.post('/records', data=data, headers=self.headers, catch_response=True) as response:
            self.dataCheck(response)
    '''
    def dataCheck(self, response):
        if 200 != response.status_code:          #page code check
            response.failure(response.content)
        else:
            if None != response.content:
                res = None
                try:
                    res = response.content.decode()      # 解码成string
                    res = json.loads(res)
                    if not isinstance(res, dict):
                        response.failure(res)
                    else:
                        if not res['result']:
                            response.failure(res)
                except Exception as e:
                    print('%s' % e)
            else:
                response.failure(response.content)

class WebsiteUser(HttpLocust):
    task_set = WebsiteTasks
    host = 'http://api.vnbig.com'
    #host = 'http://10.10.11.96:8091/openapi'
    min_wait = 2000
    max_wait = 5000