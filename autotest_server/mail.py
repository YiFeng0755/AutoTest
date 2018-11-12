#!/usr/bin/python
#-*- encoding:utf-8 -*-

import email
from email.mime.text import MIMEText
from email.header import Header
import smtplib

sender = 'auto@youmme.im'
receiver = 'cdhmuer333@126.com' #for test
subject = '测试报告'
smtpserver = 'smtp.exmail.qq.com'
username = 'auto@youme.im'
password = 'Auto418'


import smtplib
from email.mime.text import MIMEText
mailto_list=['qa@youme.im','chendh@youme.im', 'xiaoxiao@youme.im']           #收件人(列表) for test
mail_host="smtp.exmail.qq.com"            #使用的邮箱的smtp服务器地址，这里是163的smtp地址
mail_user = username                           #用户名
mail_pass = password                             #密码
mail_postfix = "qq.com"                     #邮箱的后缀

def send_mail(to_list, sub, content):
    #me="hello"+"<"+mail_user+"@"+mail_postfix+">"
    me = mail_user
    msg = MIMEText(content, _subtype='html', _charset="utf-8")
    msg['Subject'] = sub
    msg['From'] = me
    msg['To'] = ';'.join(to_list)                #将收件人列表以‘；’分隔
    try:
        server = smtplib.SMTP()
        server.connect(mail_host)                            #连接服务器
        server.login(mail_user, mail_pass)               #登录操作
        server.sendmail(me, to_list, msg.as_string())
        server.close()
        return True
    except Exception, e:
        print str(e)
        return False

#test code
if __name__ == '__main__':
    for i in range(1):  # 发送1封，上面的列表是几个人，这个就填几
        if send_mail(mailto_list, "测试测试", "Hello,我的是机器人，大家好。"):  # 邮件主题和邮件内容
            # 这是最好写点中文，如果随便写，可能会被网易当做垃圾邮件退信
            print "done!"
        else:
            print "failed!"

