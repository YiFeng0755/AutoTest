#!/usr/bin/env python
# encoding: utf-8
'''
@author: Micheal Chen
@email: cdhmuer333@126.com; chendh@youme.im; 531313715@qq.com
@file: demo2learn.py
@time: 2018/6/12 17:43
@desc: 
'''


def func():
    print "> why doesn't this line print"
    exit(0)
    yield "==================="

#
# x = func()
# print (next(x))
# def one_to_ten():
#     val = 1
#     while val <= 10:
#         #print val
#         yield val
#         val += 1
#
# for ele in one_to_ten():
#     print ele


def is_prime(number):
    if number == 2:
        return True

    i = 2
    while i * i <= number:
        if number % i == 0:
            return False
        i += 1
    else:
        return True


def get_prime(number):
    while True:
        if is_prime(number):
            yield number
        number += 1

for element in get_prime(2):
    print(element)