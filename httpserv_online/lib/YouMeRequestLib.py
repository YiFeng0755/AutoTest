#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
@author :chendonghua
'''

import time
import hashlib
import requests
import json

appkey = "YOUMEAA5EE5689436B39DC46E6195FF45F81CD46A1804"
current_time = int(time.time())
app_secret = "3hzCOsAPitudP7DiQN7ANrkbnTpEVdm0KJ1fFNmXzwL6BZTfEGwfBU4W2efhnAxkx11idqN60lIJ26KkRhBrBQcgui8Sahalzx" \
             "tVv+hKHvDhg/KTMhmal8tuknnAcxlWkq7102ZG3EM6loBPCMp6t96078W7XCWNoszlzPxT6w0BAAE="


class YouMeRequestLib:
    """
    for auto test key words lib
    """
    def __init__(self):
        self.appkey = "YOUMEAA5EE5689436B39DC46E6195FF45F81CD46A1804"
        self.secret = "3hzCOsAPitudP7DiQN7ANrkbnTpEVdm0KJ1fFNmXzwL6BZTfEGwfBU4W2efhnAxkx11idqN60lIJ26KkRhBrBQ" \
                      "cgui8Sahalzx" \
                      "tVv+hKHvDhg/KTMhmal8tuknnAcxlWkq7102ZG3EM6loBPCMp6t96078W7XCWNoszlzPxT6w0BAAE="

    def setAppKeyAndSecret(self, appkey, secret):
        self.appkey = appkey
        self.secret = secret

    def today(self, format):
        return time.strftime(format)

    def getNowDate(self):
        d = time.strftime("%Y%m%d")
        return int(d)

    def _create_crypt_string(self, version='v1'):
        current_time = str(int(time.time()))
        md5_salt = "YOUME8140"
        check_sum = ""
        if version == 'v1':
            crypt_text = self.secret + current_time
            sha1 = hashlib.sha1()
            str_tmp = crypt_text
            sha1.update(str_tmp.encode('utf8'))
            check_sum = sha1.hexdigest()
        elif version == "v2":
            #先进行MD5加密
            md5_text = self.appkey + md5_salt
            md5 = hashlib.md5()
            md5.update(md5_text.encode('utf8'))
            md5_checksum = md5.hexdigest()
            #再进行sha1加密
            sha1 = hashlib.sha1()
            crypt_text = self.secret + md5_checksum + current_time
            sha1.update(crypt_text.encode('utf8'))
            check_sum = sha1.hexdigest()
        return {
            "appkey": self.appkey,
            "identifier": "admin",
            "curtime": str(current_time),
            "checksum": check_sum
        }

    def post_data(self, url, uri, data=None):
        if not isinstance(data, dict):
            return None
        headers = {
            'content-type': "application/json",
            'cache-control': "no-cache",
            'usage': "ADTT Test"
        }
        postdata = json.dumps(data)
        params = self._create_crypt_string()
        url = url + uri

        response = requests.request('POST', url , headers=headers, data=postdata, params=params)
        return json.loads(response.text)

    def post_data_v2(self, url, uri, data=None):
        if not isinstance(data, dict):
            return None
        headers = {
            'content-type': "application/json",
            'cache-control': "no-cache",
            'usage': "ADTT Test"
        }
        url = url + uri
        postdata = json.dumps(data)
        params = self._create_crypt_string("v2")
        response = requests.request('POST', url, headers=headers, data=postdata, params=params)
        return json.loads(response.text)


if __name__ == '__main__':
    """
    测试代码
    """
    lib = YouMeRequestLib()
    print lib.today("%Y%m%d")
    # md5_salt = "YOUME8140"
    # appkey = "YOUME3AA1C9CABAD4F182B2BE26D5BFCF8B79593EFAE3"
    # app_secret = "s0f8zCk6WhDAhPrrAfRijzswxBvJotX+7Q1wE7DDxjOTANF56i2YFVKOw5qwBYahKGscVUj/RaiOAr0N5x7dqXTKsSrU/JgMwg8Ix9ZJCfcOpn3hbnPvbJl2gDlQrguHmBzxIFHrZBfir6LBFczFJS4E6l9arM237feU0W1FolMBAAE="
    # md5_text = appkey + md5_salt
    # md5 = hashlib.md5()
    # md5.update(md5_text.encode('utf8'))
    # md5_checksum = md5.hexdigest()
    # # 再进行sha1加密
    # sha1 = hashlib.sha1()
    # crypt_text = app_secret + md5_checksum + "1516886385"
    # sha1.update(crypt_text.encode('utf8'))
    # check_sum = sha1.hexdigest()
    # print("2:", check_sum)
    url = "https://test3api.youme.im/"
    uri = "v2/im/query_anchor_score_rank"
    mydata = {"StartDate":"20170701", "EndDate":"20170724", "SvrArea":"", "ChannelID":"", "OrderBy":1, "Limit": 2}
    lib.setAppKeyAndSecret(appkey, app_secret)
    req = lib.post_data_v2(url, uri, mydata)
    print json.dumps(req)



