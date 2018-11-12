#!/usr/bin/env python
# encoding: utf-8
'''
@author: Micheal Chen
@email: cdhmuer333@126.com; chendh@youme.im; 531313715@qq.com
@file: encrypt_util.py
@time: 2018/4/20 13:09
@desc: 
'''

from rsa import PublicKey, common, transform, core
from rsa import *
import binascii
import base64


class CryptUtil(object):
    """
    RSA加密适配类型
    """
    # @classmethod
    # def decrypt(self, cipher, public_key):
    #     encrypted = transform.bytes2int(cipher)
    #     decrypted = core.decrypt_int(encrypted, public_key.e
    #                                  , public_key.n)
    #     text = transform.int2bytes(decrypted)
    #     if len(text) > 0 and text[0] == '\x01':
    #         pos = text.find('\x00')
    #         if pos > 0:
    #             return text[pos + 1]
    #         else:
    #             return None

    def __init__(self):
        self.pub_key = None
        self.pri_key = None

    def rsa_set_private_key(self, publicPem, privatePem, modules):
        pub_pem = base64.decodestring(publicPem)
        pri_pem = base64.decodestring(privatePem)
        m = base64.decodestring(modules)
        rsa_key = PrivateKey(n=int(binascii.b2a_hex(pub_pem), 16),
                             d=int(binascii.b2a_hex(pri_pem), 16),
                             e=int(binascii.b2a_hex(m), 16),
                             p=None, q=None)
        return rsa_key

    def rsa_set_public_key(self, str_pub_key, modules):
        """
        :param str_pub_key: 二进制str e
        :param modules:  二进制str n
        :return: 一个设置好的公钥key
        """
        e = int(binascii.b2a_hex(str_pub_key), 16)
        n = int(binascii.b2a_hex(modules), 16)
        rsa_key = PublicKey(e=e, n=n)
        return rsa_key

    def rsa_public_encrypt(self, message, pub_key):
        return encrypt(message, pub_key)

    def rsa_public_decrypt(self, cipher_text, pub_key):
        encrypt_int = transform.bytes2int(cipher_text)
        decrypted = core.decrypt_int(encrypt_int, pub_key.e
                                     , pub_key.n)
        text = transform.int2bytes(decrypted)
        return text

if __name__ == '__main__':
    u = CryptUtil()
    appsecret = "3hzCOsAPitudP7DiQN7ANrkbnTpEVdm0KJ1fFNmXzwL6BZTfEGwfBU4W2efhnAxkx11idqN60lIJ26KkRhBrBQcgui8SahalzxtVv+hKHvDhg/KTMhmal8tuknnAcxlWkq7102ZG3EM6loBPCMp6t96078W7XCWNoszlzPxT6w0BAAE="
    out = base64.decodestring(appsecret)
    e_hex_str = binascii.b2a_hex(out[-3:])
    n_hex_str = binascii.b2a_hex(out[:128])
    e = transform.bytes2int(out[-3:])
    n = transform.bytes2int(out[:128])

    pub_key = PublicKey(n, e)
    print type(pub_key)
    cipher_text = u.rsa_public_encrypt("hello world", pub_key)
