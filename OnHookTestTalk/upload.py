#!/usr/bin/python
#-*- encoding:utf-8 -*-

# @Author : liuyang
# @Time : 2018/11/9

from poster.encode import multipart_encode
from poster.streaminghttp import register_openers
import urllib2
import sys

def upload_file(url,upload_file_path,filename):
	register_openers()
	datagen, headers = multipart_encode({"file": open(upload_file_path, "rb"),"type":"uploadFile","filename":filename})
	request = urllib2.Request(url, datagen, headers)
	return urllib2.urlopen(request).read()

if __name__ =="__main__":
    url = sys.argv[1]
    filePath = sys.argv[2]
    fileName = sys.argv[3]
    print url
    print filePath
    print fileName
    upload_file(url,filePath,fileName)