# 1.Configuration Info
 
# 项目路径 需修改
projectDir="你的项目路径"
 
# 打包生成路径 需修改
ipaPath="/User/Micheal/Desktop/autotest.ipa"
 
# 图标路径 需修改
#iconPath="~/Desktop/icon"
 
# Provisioning Profile 需修改 查看本地配置文件
PROVISIONING_PROFILE="fa970500-ade4-4a64-b0bb-0db370e9b7b3"
 
############# 重签名需要文件
# 以下文件需放在 ipaPath 路径下
Entitlements=$ipaPath/entitlements.plist
#############
 
# 版本号
bundleVersion="1.2.0"
 
# 选择打包序号 多选则以空格隔开 如("1" "2" "3")
appPackNum=("1")
 
# 蒲公英分发参数 不分发可忽略 默认不分发 下面的两个KEY是默认测试的网址对应KEY
ISUPLOAD=0
USERKEY="xxx"
APIKEY="xxx"
 
# ---------------------------可选 如果需要替换 app 的 icon --------------------------------- #
 
# 配置App信息数组 格式:"AppName(和工程中appInfo.Plist对应)" "icon"
#Schemes:
#        1.app1 app1Icon
#        2.app2 app2Icon
#        3.app3 app3Icon
 
# --------------------------------------------------------------------------------------- #
 
# 打包个数
appPackNumLength=${#appPackNum[*]}
 
appInfos=(
          "app1" "app1Icon" "xxxx"
          "app2" "app2Icon" "xxxx"
          "app3" "app3Icon" "xxxx"
         )
appInfosLength=${#appInfos[*]}
					
# Scheme Name
schemeName="xx"
# Code Sign ID
CODE_SIGN_IDENTITY="xx co., LTD"					 
# 生成 APP 路径
buildDir="build/Release-iphoneos"					 
# 开始时间
beginTime=`date +%s`
# 创建打包目录
mkdir ${ipaPath}/AllPack
# 本地存放全部 IPA 的路径
allIPAPackPath="${ipaPath}/allPack"
# 清除缓存
rm -rf $projectDir/$buildDir
					 
# Build 生成 APP
xcodebuild -workspace ${projectDir}/xx.xcworkspace -scheme ${schemeName} -configuration Release clean -sdk iphoneos build CODE_SIGN_IDENTITY="${CODE_SIGN_IDENTITY}" PROVISIONING_PROFILE="${PROVISIONING_PROFILE}" SYMROOT="${projectDir}/build"
 if [[ $? = 0 ]]; then
   echo "\033[31m 编译成功\n \033[0m"
 else
     echo "\033[31m 编译失败\n \033[0m"
 fi

 # 先创建 payload 文件夹
 mkdir ~/Desktop/Payload
 # 移动编译生成的 app 到桌面的 Payload 文件夹下
 cp -Rf ${projectDir}/${buildDir}/${schemeName}.app $ipaPath/Payload

 # 以下二选一
 # 1.----全部打包----
 #for (( i=0; i $plist_path
 itemsassetskindsoftware-packageurlhttps://xxxxxxxxxxxx/$appDownloadName.ipakinddisplay-imageurlhttps://xxxxxxxxxxxx/${appIconName}.pngkindfull-size-imageurlhttps://xxxxxxxxxxxx/${appIconName}.pngmetadatabundle-identifier你的bundidbundle-version$bundleVersionkindsoftwaretitle$appDownloadName
 EOF
 # 移动
 mv ${ipaPath}/$appDownloadName.ipa ${allIPAPackPath}/$appName

 # 6.上传蒲公英分发平台

 if [[ $ISUPLOAD = 1 ]]; then
	 echo "正在上传蒲公英..."
         curl -F "file=@$allIPAPackPath/$appName/$appDownloadName.ipa" -F "uKey=$USERKEY" -F "_api_key=$APIKEY" http://www.pgyer.com/apiv1/app/upload		
fi
done

# 清除无关文件
rm -rf $ipaPath/Payload
# 结束时间
endTime=`date +%s`
echo -e "打包时间$[ endTime - beginTime ]秒"
