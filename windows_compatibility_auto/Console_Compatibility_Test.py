#encoding: utf-8

from selenium import webdriver
from selenium.common.exceptions import NoAlertPresentException
from FileUtil import createDir
from time import sleep
import os,time
import traceback
import random


drivers = []
fire_driver = webdriver.Firefox(executable_path="geckodriver.exe")
chrome_driver = webdriver.Chrome(executable_path="chromedriver.exe")
ie_driver = webdriver.Ie(executable_path="IEDriverServer.exe")
drivers.append(fire_driver)
drivers.append(chrome_driver)
drivers.append(ie_driver)


#保存异常图片
picDir = createDir()
def takeScreenshot(driver,savePath,picName):
    picPath = os.path.join(savePath,str(picName).decode("utf-8").encode("gbk")+ ".png")
    try:
        driver.get_screenshot_as_file(picPath)
    except Exception,e:
        print traceback.print_exc()




#判断页面元素是否出现
def isElementPresent(by,value):
    #从selenium.common.exceptions模块导入NoSuchElementException异常类
    from selenium.common.exceptions import NoSuchElementException
    try:
        driver.find_element(by = by,value = value)
    except NoSuchElementException,e:
        #打印异常信息
        print e
        #发生异常，说明页面中未找到元素，返回False
        return False
    else:
        return True

def login():
    elem_user = driver.find_element_by_xpath('//input[@id="email"]')
    elem_user.send_keys('test@youme.im')
    elem_pwd = driver.find_element_by_xpath('//input[@type="password"]')
    elem_pwd.send_keys('test0000')
    #sleep(1)
    sumit_button = driver.find_element_by_id("J_submit")
    sleep(2)
    sumit_button.click()
    #res = isElementPresent("id","J_submit")
    #if res is True:
        #driver.find_element_by_id("J_submit").click()
    sleep(3)
    driver.get(url)
    driver.find_element_by_id("345").click()
    #driver.switch_to_window(driver.window_handles[-1])

#主播账号
def click_host_account():
    global url
    driver.find_element_by_id("345").click()
    driver.find_element_by_xpath('//p[.="主播管理"]/parent::a').click()
    driver.implicitly_wait(5)
    #判断是否在主播账号页签
    driver.get(url+"/anchor/345#345")
    print driver.current_url
    #driver.find_element_by_partial_link_text("主播账号")
    driver.implicitly_wait(5)
    #添加主播账号
    driver.find_element_by_id("add_anchor").click()
    driver.implicitly_wait(5)
    #主播游戏ID
    elem_id = driver.find_element_by_xpath('//input[@id="gameid"]')
    random_id = "abc" + str(random.randint(1, 100000))
    elem_id.send_keys(random_id)
    #添加主播昵称
    elem_name = driver.find_element_by_xpath('//input[@id="name"]')
    random_name = "11" + str(random.randint(1, 100000))
    elem_name.send_keys(random_name)
    #添加主播账号
    elem_user2 = driver.find_element_by_xpath('//input[@id="uname"]')
    random_user2 = "egdkrd" + str(random.randint(1, 10000))
    elem_user2.send_keys(random_user2)
    #添加密码
    elem_pwd2 = driver.find_element_by_xpath('//input[@id="password"]')
    elem_pwd2.send_keys('zbc123')
    driver.find_element_by_class_name("btn-submit").click()
    driver.implicitly_wait(5)
    #刷新页面
    driver.get(url+"/anchor/345#345")
    driver.refresh()
    driver.implicitly_wait(10)
    table = driver.find_element_by_id('datatable')
    table_rows = table.find_elements_by_tag_name('tr')
    print "总行数:", len(table_rows)
    table_cols = table_rows[0].find_elements_by_tag_name('th')
    print "总列数:", len(table_cols)
    row1_col2 = table_rows[1].find_elements_by_tag_name('td')[2].text
    print "第一行第二列的text:", row1_col2
    #查找主播账号页签中表格中的“编辑”按钮
    table_rows[1].find_elements_by_tag_name('td')[6].find_element_by_class_name("name-action").click()
    driver.implicitly_wait(5)
    driver.find_element_by_class_name("btn-submit").click()
    driver.implicitly_wait(3)
    driver.find_element_by_class_name("btn-ok").click()
    #查找主播账号页签中表格中的“删除”按钮
    table = driver.find_element_by_id('datatable')
    table_rows = table.find_elements_by_tag_name('tr')
    table_rows[0].find_elements_by_tag_name('th')
    table_rows[1].find_elements_by_tag_name('td')[2].text
    sleep(2)
    table_rows[1].find_elements_by_tag_name('td')[6].find_element_by_class_name("del_anchor").click()
    driver.implicitly_wait(5)
    driver.find_element_by_class_name("btn-ok").click()
    sleep(3)
    # 主播频道
    driver.find_element_by_partial_link_text("主播频道").click()
    driver.implicitly_wait(5)



def click_host_channel():
    # 主播频道
    global url
    driver.find_element_by_partial_link_text("主播频道").click()
    sleep(1)
    url1 = url+"/clinfo/345"
    driver.get(url1)
    driver.refresh()
    #判断页面元素id属性值为“J_tab-detail”的页面元素是否存在
    res = isElementPresent("id","J_add_")
    if res is True:
        print u"所查找的元素存在于页面上！"
    else:
        print u"页面中未找到所需要的页面元素！"
    btn_add = driver.find_element_by_id("J_add_")
    sleep(2)
    btn_add.click()
    elem_channel_name = driver.find_element_by_xpath('//input[@id="cname"]')
    channel_name = "abc" + str(random.randint(1, 100000))
    elem_channel_name.send_keys(channel_name)
    elem_channel_id = driver.find_element_by_xpath('//input[@id="ciidd"]')
    channel_id = random.randint(1, 100000)
    elem_channel_id.send_keys(channel_id)
    driver.find_element_by_class_name("btn-submit").click()
    #刷新页面
    driver.get(url1)
    driver.refresh()
    sleep(3)
    #获取表格
    table = driver.find_element_by_id('datatable')
    table_rows = table.find_elements_by_tag_name('tr')
    print "添加主播频道table总行数:", len(table_rows)
    table_cols = table_rows[0].find_elements_by_tag_name('th')
    print "添加主播频道table总列数:", len(table_cols)
    row1_col2 = table_rows[1].find_elements_by_tag_name('td')[3].text
    print "添加主播频道table第二行第三列的text:", row1_col2
    driver.implicitly_wait(5)
    table_rows[1].find_elements_by_tag_name('td')[4].find_element_by_class_name("white-action").click()
    driver.implicitly_wait(5)
    driver.find_element_by_id("userid").clear()
    driver.find_element_by_id("userid").send_keys("123")
    driver.find_element_by_css_selector("#J_add-userid .footer input").click()
    sleep(5)
    table = driver.find_element_by_id('datatable')
    table_rows = table.find_elements_by_tag_name('tr')
    table_rows[0].find_elements_by_tag_name('th')
    table_rows[1].find_elements_by_tag_name('td')[3].text
    table_rows[1].find_elements_by_tag_name('td')[4].find_element_by_class_name("black-action").click()
    sleep(2)
    driver.find_element_by_id("userid").clear()
    driver.find_element_by_id("userid").send_keys("456")
    driver.find_element_by_css_selector("#J_add-userid .footer input").click()
    sleep(5)
    table = driver.find_element_by_id('datatable')
    table_rows = table.find_elements_by_tag_name('tr')
    table_rows[0].find_elements_by_tag_name('th')
    #添加主播频道table
    row1_col2 = table_rows[1].find_elements_by_tag_name('td')[5].text
    print "主播频道中添加主播频道table中第2行第6列内容：",row1_col2
    action_button = table_rows[1].find_elements_by_tag_name('td')[5].find_element_by_class_name("name-action")
    sleep(2)
    action_button.click()
    sleep(2)
    driver.find_element_by_id("imid").clear()
    driver.find_element_by_id("imid").send_keys("1245")
    driver.find_element_by_css_selector("#J_add-box .footer input").click()
    sleep(3)
    table = driver.find_element_by_id('datatable')
    table_rows = table.find_elements_by_tag_name('tr')
    table_rows[0].find_elements_by_tag_name('th')
    table_rows[1].find_elements_by_tag_name('td')[5].find_element_by_class_name("del_anchor").click()
    driver.find_element_by_class_name("btn-ok").click()

def click_host_Schedul():
    global url
    sleep(2)
    driver.find_element_by_partial_link_text("主播排班").click()
    sleep(2)
    url15 = url+"/schedule/345"
    driver.get(url15)
    print driver.current_url
    sleep(1)
    add_sche = driver.find_element_by_id("J_add_sche")
    sleep(1)
    add_sche.click()
    sleep(2)
    driver.find_element_by_css_selector(".multiDiv div:nth-child(2)").click()
    sleep(2)
    driver.find_element_by_css_selector("#channelid").click()
    sleep(1)
    driver.find_element_by_css_selector("#channelid option:nth-child(2)").click()
    sleep(2)
    #提交
    driver.find_element_by_xpath(".//*[@id='btnSave']").click()
    sleep(1)
    res = isElementPresent("id", "btnSave")
    if res is True:
        print u"所查找的元素存在于页面上！"
        driver.find_element_by_xpath(".//*[@id='cancelBtn']").click()
    else:
        print "添加主播排班成功"

def click_host_monitor():
    global url
    driver.find_element_by_partial_link_text("主播监控").click()
    url1 = url+"/anchorMonitor/345"
    driver.get(url1)
    driver.refresh()
    sleep(2)

def click_forbid_manage():
    global url
    sleep(4)
    driver.find_element_by_id("345").click()
    sleep(3)
    driver.find_element_by_xpath('//p[.="禁言管理"]/parent::a').click()
    url2 = url+"/forbidMgr/345#345"
    driver.get(url2)
    driver.refresh()
    sleep(5)


def click_forbid_user():
    global url
    click_forbid_manage()
    driver.find_element_by_xpath('.//*[@id="datatable"]/table/tbody/tr[1]/td[4]/a').click()
    url3 = url+"/forbidPlayer/345"
    driver.get(url3)
    sleep(2)
    driver.find_element_by_xpath(".//*[@id='add_anchor']").click()
    sleep(2)
    UserID = driver.find_element_by_xpath(".//*[@id='user_id']")
    UserID.send_keys("345")
    forbbinCheckBox = driver.find_element_by_xpath(".//*[@id='reason_checkd']/div[2]")
    forbbinCheckBox.click()
    forbbinChannel = driver.find_element_by_xpath(".//*[@id='forbid-check']")
    forbbinChannel.click()
    driver.find_element_by_id("submitBtn").click()
    sleep(4)
    confirm_button = driver.find_element_by_xpath(".//*[@id='J_confirmbox']/div[2]/div[3]/a[1]")
    sleep(2)
    confirm_button.click()
    url4 = url+"/forbidPlayer/345"
    driver.get(url4)
    driver.refresh()
    #点击禁言玩家的查看按钮
    sleep(2)
    table = driver.find_element_by_id('datatable')
    table_rows = table.find_elements_by_tag_name('tr')
    print "禁言玩家页面table总行数:", len(table_rows)
    #####
    table_cols = table_rows[0].find_elements_by_tag_name('th')
    print "禁言玩家页面table总列数:", len(table_cols)
    row1_col2 = table_rows[1].find_elements_by_tag_name('td')[3].text
    print "禁言玩家页面table第二行第三列的text:", row1_col2
    table_rows[1].find_elements_by_tag_name('td')[9].find_element_by_id("search-btn").click()
    edit_button = driver.find_element_by_id("editBtn")
    sleep(1)
    edit_button.click()
    #判断禁言详情中玩家“UID345”是否存在
    res = isElementPresent("id","user_id")
    if res is True:
        aElement = driver.find_element_by_id("user_id").get_attribute('value')
        print "+"*20
        print aElement
        if aElement == "345":
             driver.find_element_by_id("submitBtn").click()
        else:
             print u"玩家UID内容错误"
    else:
        print u"页面中未出现玩家UID"
    sleep(2)
    driver.find_element_by_xpath(".//*[@id='J_confirmbox']/div[2]/div[3]/a[1]").click()
    sleep(3)
    #点击解禁按钮
    url5 = url+"/forbidPlayer/345"
    driver.get(url5)
    driver.refresh()
    sleep(5)
    table = driver.find_element_by_id('datatable')
    table_rows = table.find_elements_by_tag_name('tr')
    table_rows[0].find_elements_by_tag_name('th')
    table_rows[1].find_elements_by_tag_name('td')[3].text
    table_rows[1].find_elements_by_tag_name('td')[9].find_element_by_id("delete-btn").click()
    sleep(2)
    confirm_button = driver.find_element_by_xpath(".//*[@id='J_confirmbox']/div[2]/div[3]/a[1]")
    sleep(1)
    confirm_button.click()
    sleep(4)
    save_button = driver.find_element_by_xpath(".//*[@id='J_confirmbox']/div[2]/div[3]/a[1]")
    sleep(1)
    save_button.click()

def click_forbbin_channel():
    global url
    click_forbid_manage()
    driver.find_element_by_xpath(".//*[@id='datatable']/table/tbody/tr[2]/td[4]/a").click()
    sleep(2)
    url6 = url+"/forbidRoom/345"
    driver.get(url6)
    driver.refresh()
    sleep(2)
    driver.find_element_by_xpath(".//*[@id='add_anchor']").click()
    channelID = driver.find_element_by_id("chanelid")
    channelID.send_keys("user123")
    reason_box = driver.find_element_by_xpath(".//*[@id = 'reason_checkd']/div[2]")
    sleep(1)
    reason_box.click()
    driver.find_element_by_id("submitBtn").click()

    #判断禁言详情页面是否出现禁言失败字样
    try:
        assert u"禁言失败!" in driver.page_source
        print 'Assertion test pass.'
    except AssertionError, e:
        takeScreenshot(driver, picDir, e)
    except Exception, e:
        print traceback.print_exc()
        takeScreenshot(driver, picDir, e)
    else:
        driver.find_element_by_id("submitBtn").click()
    sleep(3)
    confirm_button = driver.find_element_by_xpath(".//*[@id='J_confirmbox']/div[2]/div[3]/a[1]")
    sleep(1)
    confirm_button.click()
    url7 =url+"/forbidRoom/345"
    driver.get(url7)
    driver.refresh()
    #点击禁言玩家的查看按钮
    sleep(2)
    table = driver.find_element_by_id('datatable')
    table_rows = table.find_elements_by_tag_name('tr')
    print "禁言频道页面table总行数:", len(table_rows)
    #####
    table_cols = table_rows[0].find_elements_by_tag_name('th')
    print "禁言频道页面table总列数:", len(table_cols)
    row1_col2 = table_rows[1].find_elements_by_tag_name('td')[3].text
    print "禁言频道页面table第二行第三列的text:", row1_col2
    table_rows[1].find_elements_by_tag_name('td')[8].find_element_by_id("search-btn").click()
    edit_button = driver.find_element_by_id("editBtn")
    sleep(2)
    edit_button.click()
    #判断禁言详情中玩家“UID345”是否存在
    res = isElementPresent("id","chanelid")
    if res is True:
        aElement = driver.find_element_by_id("chanelid").get_attribute('value')
        if aElement == "user123":
             sumit_button = driver.find_element_by_id("submitBtn")
             sleep(1)
             sumit_button.click()
        else:
             print u"频道ID内容错误"
    else:
        print u"页面中未出现频道ID"
    sleep(2)
    confirm_button = driver.find_element_by_xpath(".//*[@id='J_confirmbox']/div[2]/div[3]/a[1]")
    sleep(2)
    confirm_button.click()
    sleep(3)
    #点击解禁按钮
    url8 = url+"/forbidRoom/345"
    driver.get(url8)
    driver.refresh()
    sleep(5)
    table = driver.find_element_by_id('datatable')
    table_rows = table.find_elements_by_tag_name('tr')
    table_rows[0].find_elements_by_tag_name('th')
    table_rows[1].find_elements_by_tag_name('td')[3].text
    table_rows[1].find_elements_by_tag_name('td')[8].find_element_by_id("delete-btn").click()
    sleep(1)
    confirm_button = driver.find_element_by_xpath(".//*[@id='J_confirmbox']/div[2]/div[3]/a[1]")
    sleep(1)
    confirm_button.click()
    sleep(4)
    save_button = driver.find_element_by_xpath(".//*[@id='J_confirmbox']/div[2]/div[3]/a[1]")
    sleep(1)
    save_button.click()

def click_keyword_manage():
    global url
    driver.find_element_by_id("345").click()
    sleep(3)
    driver.find_element_by_xpath('//p[.="关键字管理"]/parent::a').click()
    url9 = url+"/sensiword/345#345"
    driver.get(url9)
    sleep(4)
    driver.find_element_by_id("add_word").click()
    category_name = driver.find_element_by_id("categary_name")
    sleep(1)
    category_name.send_keys("youmite")
    sleep(1)
    driver.find_element_by_class_name("btn-submit").click()
    sleep(2)
    driver.find_element_by_class_name("btn-ok").click()
    sleep(4)
    url10 = url+"/sensiword/345#345"
    driver.get(url10)
    sleep(5)
    ##添加类别页面的table
    table = driver.find_element_by_id('datatable')
    table_rows = table.find_elements_by_tag_name('tr')
    print "关键字管理页面table总行数:", len(table_rows)
    table_cols = table_rows[0].find_elements_by_tag_name('th')
    print "关键字管理页面table总列数:", len(table_cols)
    row1_col2 = table_rows[2].find_elements_by_tag_name('td')[3].text
    print "关键字管理页面table第二行第三列的text:", row1_col2
    table_rows[2].find_elements_by_tag_name('td')[4].find_element_by_id("search-btn").click()
    sleep(5)
    add_word = driver.find_element_by_id("add_word")
    sleep(2)
    add_word.click()
    sleep(3)
    word_list = driver.find_element_by_id("wordlist")
    word_list.send_keys("bad246")
    sleep(3)
    #添加关键字的提交按钮
    driver.find_element_by_xpath(".//*[@id='J_addall-box']/div[2]/div[2]/form/div[2]/input").click()
    sleep(3)
    #确认按钮
    driver.find_element_by_xpath(".//*[@id='J_confirmbox']/div[2]/div[3]/a[1]").click()
    sleep(5)
    driver.current_url
    driver.get(driver.current_url)
    #添加关键字页面的table
    sleep(5)
    table = driver.find_element_by_id('datatable')
    table_rows = table.find_elements_by_tag_name('tr')
    cell_row = table_rows[0].find_elements_by_tag_name('th')
    print cell_row
    table_rows[1].find_elements_by_tag_name('td')[2].text
    table_rows[1].find_elements_by_tag_name('td')[3].find_element_by_class_name("edit-self").click()
    keyword_name = driver.find_element_by_id("Word")
    keyword_name.send_keys("ioyhui123")
    driver.find_element_by_xpath(".//*[@id='J_add-box']/div[2]/div[2]/form/div[2]/input").click()
    sleep(4)
    driver.find_element_by_xpath(".//*[@id='J_confirmbox']/div[2]/div[3]/a[1]").click()
    sleep(5)
    ##添加关键字页面的table
    table = driver.find_element_by_id('datatable')
    table_rows = table.find_elements_by_tag_name('tr')
    table_rows[0].find_elements_by_tag_name('th')
    table_rows[1].find_elements_by_tag_name('td')[2].text
    table_rows[1].find_elements_by_tag_name('td')[3].find_element_by_class_name("delete-self").click()
    sleep(1)
    driver.find_element_by_xpath(".//*[@id='J_confirmbox']/div[2]/div[3]/a[1]").click()

    driver.find_element_by_xpath("html/body/div[1]/div[1]/div[3]/div/ol/li[3]/a").click()
    url11 = url+"/sensiword/345"
    driver.get(url11)
    sleep(5)
    ##添加类别页面的table
    table = driver.find_element_by_id('datatable')
    table_rows = table.find_elements_by_tag_name('tr')
    print "关键字管理页面table总行数:", len(table_rows)
    #####
    table_cols = table_rows[0].find_elements_by_tag_name('th')
    print "关键字管理页面table总列数:", len(table_cols)
    row1_col2 = table_rows[2].find_elements_by_tag_name('td')[3].text
    print "关键字管理页面table第二行第三列的text:", row1_col2
    table_rows[2].find_elements_by_tag_name('td')[4].find_element_by_class_name("delete-sensiwords").click()
    sleep(3)
    driver.find_element_by_xpath(".//*[@id='J_confirmbox']/div[2]/div[3]/a[1]").click()

    print "***" * 20
    print "finished"

#url = "https://console.youme.im"
#url = "https://testconsole.youme.im"
url = "http://zdd_console2_7.web.nxit.us"

if __name__ == '__main__':
    for driver in drivers:
        driver.maximize_window()
        driver.get(url)
        print driver.current_url
        sleep(2)
        login()
        click_host_account()
        click_host_channel()
        click_host_Schedul()
        click_forbid_user()
        click_forbbin_channel()
        click_keyword_manage()
        driver.quit()








