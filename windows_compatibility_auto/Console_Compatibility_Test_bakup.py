#coding=utf-8

from selenium import webdriver
from selenium.webdriver.support.wait import WebDriverWait
from time import sleep
import unittest
import random

class VisitSogouByFirefox(unittest.TestCase):
    def setUp(self):
        #启动Firefox浏览器
        self.driver = webdriver.Firefox(executable_path="geckodriver.exe")
        self.driver = webdriver.Chrome(executable_path="chromedriver.exe")
        self.driver = webdriver.Ie(executable_path="IEDriverServer.exe")
        self.driver.maximize_window()


    def login(self):
        #self.driver = webdriver.Firefox(executable_path="C:\drivers/geckodriver.exe")
        self.driver.get("http://t.console.nxit.us/user/login")
        # 打印当前网页的网址
        print self.driver.current_url
        sleep(3)

        elem_user = self.driver.find_element_by_xpath('//input[@id="email"]')
        elem_user.send_keys('test@youme.im')
        elem_pwd = self.driver.find_element_by_xpath('//input[@type="password"]')
        elem_pwd.send_keys('test0000')
        self.driver.find_element_by_id("J_submit").click()
        self.driver.implicitly_wait(10)
        #self.driver.switch_to_window(self.driver.window_handles[-1])


    def click_host_account(self):
        self.driver.find_element_by_id("345").click()
        self.driver.find_element_by_xpath('//p[.="主播配置"]/parent::a').click()
        self.driver.implicitly_wait(5)
        self.driver.find_element_by_id("add_anchor").click()
        self.driver.implicitly_wait(10)
        elem_id = self.driver.find_element_by_xpath('//input[@id="gameid"]')
        random_id = "abc" + str(random.randint(1, 100000))
        elem_id.send_keys(random_id)
        elem_name = self.driver.find_element_by_xpath('//input[@id="name"]')
        random_name = "11" + str(random.randint(1, 100000))
        elem_name.send_keys(random_name)
        elem_user2 = self.driver.find_element_by_xpath('//input[@id="uname"]')
        random_user2 = "egdkrd" + str(random.randint(1, 10000))
        elem_user2.send_keys(random_user2)
        elem_pwd2 = self.driver.find_element_by_xpath('//input[@id="password"]')
        elem_pwd2.send_keys('zbc123')
        self.driver.find_element_by_class_name("btn-submit").click()
        self.driver.implicitly_wait(5)

        self.driver.get("http://t.console.nxit.us/anchor/345#345")
        self.driver.refresh()
        self.driver.implicitly_wait(10)
        table = self.driver.find_element_by_id('datatable')
        table_rows = table.find_elements_by_tag_name('tr')
        print "总行数:", len(table_rows)
        table_cols = table_rows[0].find_elements_by_tag_name('th')
        print "总列数:", len(table_cols)
        row1_col2 = table_rows[1].find_elements_by_tag_name('td')[2].text
        print "第二行第二列的text:", row1_col2
        table_rows[1].find_elements_by_tag_name('td')[5].find_element_by_class_name("name-action").click()
        self.driver.implicitly_wait(5)
        self.driver.find_element_by_class_name("btn-submit").click()
        self.driver.implicitly_wait(3)
        self.driver.find_element_by_class_name("btn-ok").click()

        table = self.driver.find_element_by_id('datatable')
        table_rows = table.find_elements_by_tag_name('tr')
        #print "总行数:", len(table_rows)
        table_rows[0].find_elements_by_tag_name('th')
        #print "总列数:", len(table_cols)
        table_rows[1].find_elements_by_tag_name('td')[2].text
        self.driver.implicitly_wait(5)
        table_rows[1].find_elements_by_tag_name('td')[5].find_element_by_class_name("del_anchor").click()
        self.driver.implicitly_wait(5)
        self.driver.find_element_by_class_name("btn-ok").click()
        sleep(3)
        #主播频道
        self.driver.find_element_by_partial_link_text("主播频道").click()
        self.driver.implicitly_wait(5)

    def isElementPresent(self,by,value):
        #从selenium.common.exceptions模块导入NoSuchElementException异常类
        from selenium.common.exceptions import NoSuchElementException
        try:
            element = self.driver.find_element(by = by,value = value)
        except NoSuchElementException,e:
            #打印异常信息
            print e
            #发生异常，说明页面中未找到元素，返回False
            return False
        else:
            return True

    def test_isElementPresent(self):
        self.click_host_account()
        # url = "http://t.console.nxit.us/clinfo/345"
        # self.driver.get(url)
        #判断页面元素id属性值为“J_tab-detail”的页面元素是否存在
        res = self.isElementPresent("id","J_add_")
        if res is True:
            print u"所查找的元素存在于页面上！"
        else:
            print u"页面中未找到所需要的页面元素！"
        btn_add = self.driver.find_element_by_id("J_add_")
        sleep(2)
        btn_add.click()
        elem_channel_name = self.driver.find_element_by_xpath('//input[@id="cname"]')
        channel_name = "abc" + str(random.randint(1, 100000))
        elem_channel_name.send_keys(channel_name)
        elem_channel_id = self.driver.find_element_by_xpath('//input[@id="ciidd"]')
        channel_id = random.randint(1, 100000)
        elem_channel_id.send_keys(channel_id)
        self.driver.find_element_by_class_name("btn-submit").click()
        #刷新页面
        self.driver.get("http://t.console.nxit.us/clinfo/345")
        self.driver.refresh()
        sleep(3)
        #获取表格
        table = self.driver.find_element_by_id('datatable')
        table_rows = table.find_elements_by_tag_name('tr')
        print "总行数:", len(table_rows)
        table_cols = table_rows[0].find_elements_by_tag_name('th')
        print "总列数:", len(table_cols)
        row1_col2 = table_rows[1].find_elements_by_tag_name('td')[3].text
        print "第二行第三列的text:", row1_col2
        table_rows[1].find_elements_by_tag_name('td')[4].find_element_by_class_name("white-action").click()
        self.driver.implicitly_wait(5)
        self.driver.find_element_by_id("userid").clear()
        self.driver.find_element_by_id("userid").send_keys("123")
        self.driver.find_element_by_css_selector("#J_add-userid .footer input").click()
        sleep(5)
        table = self.driver.find_element_by_id('datatable')
        table_rows = table.find_elements_by_tag_name('tr')
        table_rows[0].find_elements_by_tag_name('th')
        table_rows[1].find_elements_by_tag_name('td')[3].text
        table_rows[1].find_elements_by_tag_name('td')[4].find_element_by_class_name("black-action").click()
        sleep(2)
        self.driver.find_element_by_id("userid").clear()
        self.driver.find_element_by_id("userid").send_keys("456")
        self.driver.find_element_by_css_selector("#J_add-userid .footer input").click()
        sleep(3)
        table = self.driver.find_element_by_id('datatable')
        table_rows = table.find_elements_by_tag_name('tr')
        table_rows[0].find_elements_by_tag_name('th')
        table_rows[1].find_elements_by_tag_name('td')[5].find_element_by_class_name("name-action").click()
        self.driver.find_element_by_id("imid").clear()
        self.driver.find_element_by_id("imid").send_keys("1245")
        self.driver.find_element_by_css_selector("#J_add-box .footer input").click()
        sleep(3)
        table = self.driver.find_element_by_id('datatable')
        table_rows = table.find_elements_by_tag_name('tr')
        table_rows[0].find_elements_by_tag_name('th')
        table_rows[1].find_elements_by_tag_name('td')[5].find_element_by_class_name("del_anchor").click()
        self.driver.find_element_by_class_name("btn-ok").click()

    #def tearDown(self):
        #退出Firefox浏览器
        #self.driver.quit()


if __name__ == '__main__':
    unittest.main()