#!/usr/bin/env python
# encoding: utf-8
'''
@author: Micheal Chen
@email: cdhmuer333@126.com; chendh@youme.im; 531313715@qq.com
@file: login_client.py
@time: 2018/4/20 18:20
@desc: 
'''

appKey = "YOUMEF405C62A2744F22231463EAFB2AFB62749C7F93D"
appSecret = "vxMKLHmmzuoCtUZavCIRMv/e4a9uQ3aoitTVGF9hAtzTVpii1kYXPlXCjcJ7gUrr7rbgt7LKvpEbhQcSrr0jpo9sFt18uNdw5tFiYaD2+Bzt0Vukvktfqoi9HjJuzMVl+gaUFO0jGsr6i14+0RAQZl2Oa3q1xTzpPvphzdpgMzkBAAE="

from protocol import youme_login_pb2
from xor_cryptutil import XorCryptUtil
from encrypt_util import CryptUtil
from rsa import transform, PublicKey
import struct
import socket
import base64
import binascii

"""
{
    uint64_t m_ullReqSeq;
    uint16_t m_usLen;
    uint16_t m_usCmd;
    uint32_t m_uiServiceId;
}
"""
host = "106.75.35.102"
port = 8082


def login_client(username):
    """
    login cmd = 1
    service_id = 660
    :return:
    """
    cmd = 1
    service_id = 660
    head = struct.pack("!q", 0)
    head += struct.pack('!h', 0)
    head += struct.pack('!h', cmd)
    head += struct.pack('!i', service_id)

    b = head
    b += struct.pack('!b', len(username))
    b += XorCryptUtil.xor(username, head)
    login_req = youme_login_pb2.LoginReq()
    login_req.version = 1
    login_req.device_token = "123456789"
    login_req.pswd = "123456"
    login_req.os_type = 2
    login_req.user_info = '{"tst":1, "f": "abc def"}'
    reqmsg = login_req.SerializeToString()
    b += XorCryptUtil.xor(reqmsg, head)

    datasize = len(b)
    pmsg = struct.pack('!h', datasize)

    send_data = b[0:8]
    send_data += pmsg
    send_data += b[10:]

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, port))
    cb = "0000000000000000004600010000029403313233080112093132333435703739391a04a53233343536200232193d2275737420ae312c202266223a2022276262206467f2227d"
    sock.send(send_data)
    #print binascii.b2a_hex(send_data)
    chunk = sock.recv(4096)
    if len(chunk) <= 16:
        print("recv error")
        return None
    buf = chunk[16:]
    xor_head = chunk[:16]
    msg = XorCryptUtil.xor(buf, xor_head)

    login_rsp = youme_login_pb2.LoginRsp()
    login_rsp.ParseFromString(msg)
    secret = login_rsp.secret
    binascii.b2a_hex(secret)
    out = base64.decodestring(appSecret)
    e = transform.bytes2int(out[-3:])
    n = transform.bytes2int(out[128:])
    rsakey = PublicKey(n, e)
    util = CryptUtil()
    text = util.rsa_public_decrypt(secret, rsakey)
    # rsakey.d = 1L
    print binascii.b2a_hex(text)
    return login_rsp


if __name__ == '__main__':
    print login_client("123")