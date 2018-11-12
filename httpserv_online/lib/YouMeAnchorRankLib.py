#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
@author :chendonghua
'''
from YouMeRequestLib import YouMeRequestLib
from datetime import datetime
from datetime import timedelta


class YouMeAnchorRankLib:
    """
    主播排行榜查询接口
    """
    def anchor_rank_case(self, number_str):
        """
        :param number: number 表示测试用例的编号 从1开始
        :return:
        """
        lib = YouMeRequestLib()
        url = "https://test3api.youme.im/"
        uri = "v1/im/query_anchor_score_rank"

        case_base = {
            "StartDate": "",
            "EndDate": "",
            "SvrArea":"",
            "ChannelID": "",
            "OrderBy": 1,
            "Limit": 50
        }
        result = {}
        now = datetime.now()
        delta = timedelta(days=5)
        start_date = (now - delta).strftime("%Y%m%d")
        end_date = now.strftime("%Y%m%d")
        number = int(number_str)
        if number == 1:
            case_base["StartDate"] = now.strftime("%Y%m%d")
            delta = timedelta(days=3)
            end_date = now + delta #3天后
            case_base["EndDate"] = end_date.strftime("%Y%m%d")
            return lib.post_data(url, uri, data=case_base)

        if number == 2:
            #35天前
            delta = timedelta(days=35)
            end_date = now - delta
            case_base["StartDate"] = now.strftime("%Y%m%d")
            case_base["EndDate"] = end_date.strftime("%Y%m%d")
            return lib.post_data(url, uri, data=case_base)

        if number == 3:
            delta = timedelta(days=36)
            end_date = now - delta
            case_base["StartDate"] = now.strftime("%Y%m%d")
            case_base["EndDate"] = end_date.strftime("%Y%m%d")
            return lib.post_data(url, uri, data=case_base)

        if number == 4:
            case_base["StartDate"] = start_date
            case_base["EndDate"] = end_date
            case_base["Limit"] = 50
            case_base["OrderBy"] = 1
            return lib.post_data(url, uri, data=case_base)

        if number == 5:
            case_base["StartDate"] = start_date
            case_base["EndDate"] = end_date
            case_base["Limit"] = 50
            case_base["OrderBy"] = 2
            print lib.post_data(url, uri, data=case_base)
            return lib.post_data(url, uri, data=case_base)

        if number == 6:
            case_base["StartDate"] = start_date
            case_base["EndDate"] = end_date
            case_base["Limit"] = -1
            case_base["OrderBy"] = 1
            return lib.post_data(url, uri, data=case_base)

        if number == 7:
            case_base["StartDate"] = start_date
            case_base["EndDate"] = end_date
            case_base["ChannelID"] = "20481234"
            case_base["OrderBy"] = 1
            return lib.post_data(url, uri, data=case_base)

        if number == 7:
            case_base["StartDate"] = start_date
            case_base["EndDate"] = end_date
            case_base["ChannelID"] = "20481234"
            case_base["SvrArea"] = u" "
            case_base["OrderBy"] = 1
            return lib.post_data(url, uri, data=case_base)
        return result

    def user_rank_case(self, number_str):
        number = 0
        try:
            number = int(number_str)
        except ValueError as verr:
            print(verr)
            return None
        uri = "v1/im/query_im_user_score_rank"
        url = "https://test3api.youme.im/"
        lib = YouMeRequestLib()
        body_base = {
            "UserID": "",
            "Date": "",
            "Summary": "",
            "SvrArea": "",
            "Limit": 0,
            "NeedFavoriteAnchor":1
        }
        now = datetime.now()
        delta = timedelta(days=5)
        date = (now - delta).strftime("%Y%m%d")

        if number == 1:
            body_base["UserID"] = "1234"
            body_base["Date"] = date
            body_base["Limit"] = 10
            body_base["NeedFavoriteAnchor"] = 0
            return lib.post_data(url, uri, body_base)
        if number == 2:
            body_base["UserID"] = ""
            body_base["Date"] = date
            body_base["Limit"] = 10
            body_base["NeedFavoriteAnchor"] = 0
            return lib.post_data(url, uri, body_base)
        if number == 3:
            body_base["UserID"] = "1234"
            body_base["Date"] = "19700101"
            body_base["Limit"] = 10
            body_base["NeedFavoriteAnchor"] = 0
            return lib.post_data(url, uri, body_base)
        if number == 4:
            body_base["UserID"] = "1234"
            body_base["Date"] = date
            body_base["Limit"] = -1
            body_base["NeedFavoriteAnchor"] = 0
            return lib.post_data(url, uri, body_base)
        if number == 5:
            body_base["UserID"] = "1234"
            body_base["Date"] = date
            body_base["Limit"] = 300
            body_base["NeedFavoriteAnchor"] = 1
            return lib.post_data(url, uri, body_base)
        if number == 6:
            body_base["UserID"] = "1234"
            body_base["Date"] = date
            body_base["Limit"] = 10
            body_base["NeedFavoriteAnchor"] = 1
            body_base["SvrArea"] = " 测试2015"
            return lib.post_data(url, uri, body_base)
        if number == 7:
            body_base["UserID"] = "1234"
            body_base["Date"] = date
            body_base["Limit"] = 10
            body_base["NeedFavoriteAnchor"] = 1
            body_base["Summary"] = 1
            return lib.post_data(url, uri, body_base)
        return {}

if __name__ == '__main__':
    lib = YouMeAnchorRankLib()
    print lib.anchor_rank_case(7)