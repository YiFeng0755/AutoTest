#!/usr/bin/env python
# encoding: utf-8
'''
@author: Micheal Chen
@email: cdhmuer333@126.com; chendh@youme.im; 531313715@qq.com
@file: xor_cryptutil.py
@time: 2018/4/20 19:45
@desc: 
'''

import socket
import binascii
from struct import pack, unpack
from Crypto.Util import strxor


class XorCryptUtil():
    """
    这个客户端请求的封装是针对im服务器
    服务器的请求头有四个字段
    {
        uint32_t req; //请求序列号 8字节
        uint16_t uslen ; //字节的包长 2字节
        uint16_t ucmd ; // 命令字， 其服务器定义放在数据库的配置表中，客户端可以定义对应的枚举值，以应对变化 2字节
        uint32_t appId; // 4字节 服务器应用的id
    }
    在接入头结构的后边，还有一字节用来表示用户名的长度，以及n个字节用户名数据，解包和压包都不能忽略它
    """

    @classmethod
    def xor(cls, data, key):
        key_len = len(key)
        j, xor_str = 0, ""
        try:
            for c in data:
                x = ord(c)
                x ^= ord(key[j % key_len])
                xor_str += chr(x)
                j += 1
        except (ValueError, TypeError):
            return ""
        return xor_str

    #
    # def request(self, username, reqmsg, cmd, key=None):
    #     """
    #     :param username: username 用户名，需要传入
    #     :param reqmsg: reqmsg 请求包，用户自己来序列化
    #     :param cmd: cmd 命令字，一个整数
    #     :return: 二进制字符串，用户需要使用protbuf的协议回应格式来解包
    #     """
    #
    #     head = pack("!q", 15)
    #
    #     head += pack("!h", len(username) + len(reqmsg) + 1 + 16)
    #     head += pack('!h', cmd)
    #     head += pack('!i', self.__svrid)
    #
    #     b = head
    #     b += pack('!b', len(username))
    #
    #     b += Youme_Request.xor(username, head)
    #     if key is not None:
    #         b += Youme_Request.xor(reqmsg, key)
    #     else:
    #         b += Youme_Request.xor(reqmsg, head)
    #     sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    #     sock.connect((self.__host, self.__port))
    #     sock.send(b)
    #     #print(repr(b))
    #     chunk = sock.recv(4096)
    #     if len(chunk) <= 16:
    #         print("recv error")
    #         return None
    #     buf = chunk[16:]
    #     xor_head = chunk[:16]
    #     if key:
    #         msg = Youme_Request.xor(buf, key)
    #     else:
    #         msg = Youme_Request.xor(buf, xor_head)
    #     #print (repr(msg))
    #     return msg

if __name__ == '__main__':
   pass
