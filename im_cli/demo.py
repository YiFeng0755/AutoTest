#!/usr/bin/env python
# encoding: utf-8
'''
@author: Micheal Chen
@email: cdhmuer333@126.com; chendh@youme.im; 531313715@qq.com
@file: demo.py
@time: 2018/4/18 19:44
@desc: 
'''

from Crypto.Cipher import PKCS1_v1_5
from Crypto.PublicKey import RSA
from Crypto.Random import random
import base64
from protocol.comm import youme_comm_pb2
from protocol import youme_getmsg_pb2

message = str(random.randint(1, 65536 * 65536))
print message

key = "3hzCOsAPitudP7DiQN7ANrkbnTpEVdm0KJ1fFNmXzwL6BZTfEGwfBU4W2efhnAxkx11idqN60lIJ26KkRhBrBQcgui8SahalzxtVv+hKHvDhg/KTMhmal8tuknnAcxlWkq7102ZG3EM6loBPCMp6t96078W7XCWNoszlzPxT6w0BAAE="

#
# rsa_key = RSA.importKey(key)
# cipher = PKCS1_v1_5.new(rsa_key)
#
# msg = cipher.encrypt(message)
#
# print base64.encodestring(msg)


class Demo(object):
    keydata = ['a', 'b']

    def __getattr__(self, item):
        if item in self.keydata:
            # For backward compatibility, allow the user to get (not set) the

            return getattr(self.key, item)
        else:
            raise AttributeError("%s object has no %r attribute" % (self.__class__.__name__, item,))


data = "CAASAzY2NhoHMjAxNjY2NiAGKAIyIeWkmuWcuuaZr+aooeW8j++8jOeBtea0u+mAgumFje+8gTi8gsivsixAhOCQ7NvgjfYCSMONrNcFUh0KGwoIU2VuZFRpbWUSDTE1MjUzNTIxMzE1NzIYBQ=="
s = base64.decodestring(data)
rsp = youme_getmsg_pb2.YoumeMsg()
rsp.ParseFromString(s)
print rsp


