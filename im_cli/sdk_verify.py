#!/usr/bin/env python
# encoding: utf-8
'''
@author: Micheal Chen
@email: cdhmuer333@126.com; chendh@youme.im; 531313715@qq.com
@file: sdk_verify.py
@time: 2018/4/18 14:57
@desc: 
'''
import hashlib
import random
import base64
import binascii

from common import Youme_Request
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_v1_5
from Crypto.Random import random
from protocol import youme_comm_conf_pb2
from protocol.comm import youme_comm_pb2
import time
import struct

import socket

appkey = "YOUMEAA5EE5689436B39DC46E6195FF45F81CD46A1804"
appsecret = "3hzCOsAPitudP7DiQN7ANrkbnTpEVdm0KJ1fFNmXzwL6BZTfEGwfBU4W2efhnAxkx11idqN60lIJ26KkRhBrBQcgui8SahalzxtVv+hKHvDhg/KTMhmal8tuknnAcxlWkq7102ZG3EM6loBPCMp6t96078W7XCWNoszlzPxT6w0BAAE="


host = "106.75.35.102"
port = 8081

Youme_Request(host=host, port=port)


def encrypt(appkesecret, message):
    out = base64.decodestring(appkesecret)
    key_hex_str = binascii.b2a_hex(out[-3:])
    module_hex_str = binascii.b2a_hex(out[:128])
    _key = int(key_hex_str, 16)
    _module = int(module_hex_str, 16)
    #appsecret 会解码成一个131长度的二进制字节
    #公钥取二进制的末尾三位
    rsa_obj = RSA.construct((_module, long(_key)))
    return rsa_obj.encrypt(message, random.randint(1, 65536 * 65536))

def validate_sdk(appkey, appsecret, packname):
    random_int = random.randint(1, 65536 * 65536)
    #print random_int

    conn_req = youme_comm_conf_pb2.CommConfReq()

    conn_req.version = 1
    conn_req.appkey = appkey
    tuple_str = encrypt(appsecret, random_int)
    conn_req.verify = bin(tuple_str[0])
    conn_req.platform =  youme_comm_pb2.Platform_Unknow
    conn_req.brand = "python"
    conn_req.sys_version = "3.0.0"
    conn_req.service_type = youme_comm_conf_pb2.SERVICE_IM
    conn_req.cpu_arch = "x86"
    conn_req.cpu_chip = "x86"
    conn_req.sdk_version = 1
    conn_req.pkg_name = "com.youme.pythoncli"
    conn_req.device_token = ""
    conn_req.model = "python"
    conn_req.sdk_name = "im2.1.3"
    message = conn_req.SerializeToString()
    print len(message)
    sk = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sk.connect((host, port))
    sk.send(message)

    resmsg = sk.recv(4)
    print len(resmsg)
    rsp = youme_comm_conf_pb2.CommConfRsp()
    rsp.ParseFromString(resmsg)
    print str(rsp)

if __name__ == '__main__':
    #global appkey
    #global appsecret
    #validate_sdk(appkey, appsecret, "python_cli")
    #encrypt(appsecret, 12321313L)
    validate_sdk(appkey, appsecret, "")






