#!/usr/bin/env python
# encoding: utf-8
'''
@author: Micheal Chen
@email: cdhmuer333@126.com; chendh@youme.im; 531313715@qq.com
@file: get_room_history_msg_cli.py
@time: 2018/4/27 14:07
@desc: 
'''

from protocol import youme_room_pb2
from protocol.comm import youme_comm_pb2

from xor_cryptutil import XorCryptUtil
from encrypt_util import CryptUtil
from rsa import transform, PublicKey
import struct
import socket
import base64
import binascii

host = "106.75.35.102"
port = 8082


def get_room_history_client():
    cmd = 16
    service_id = 345
    head = struct.pack('!q', 0L)
    head += struct.pack('h', 0)
    head += struct.pack('!h', cmd)
    head += struct.pack('!', service_id)



