#include "MainViewScene.h"

#include "CustomerCenterScene.h"
#include "HotelCenterScene.h"

#include "cocos-ext.h"
#include "../extensions/network/HttpClient.h"
#include "../extensions/network/HttpRequest.h"
USING_NS_CC_EXT;
USING_NS_CC;

using namespace CSJson;
using namespace std;

// on "init" you need to initialize your instance
bool MainView::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCScene::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    m_pUILayer = UILayer::create();
    
    //********************加载场景资源******************//
    
    Layout* equipe_root = dynamic_cast<Layout*>(CCUIHELPER->createWidgetFromJsonFile("cocosgui/mainView/mainView.json"));
    
    m_pUILayer->addWidget(equipe_root);
    
    // hotel center btn
	UIButton* hotel_btn = dynamic_cast<UIButton*>(m_pUILayer->getWidgetByName("hotel_btn"));
	hotel_btn->addReleaseEvent(this, coco_releaseselector(MainView::hotelCenterBtnCallBack));
    
    // customer center btn
	UIButton* customer_btn = dynamic_cast<UIButton*>(m_pUILayer->getWidgetByName("customer_btn"));
	customer_btn->addReleaseEvent(this, coco_releaseselector(MainView::customerCenterBtnCallBack));
    
    
    this->addChild(m_pUILayer);
    
    getData();
    
    return true;
}


void MainView::hotelCenterBtnCallBack(CCObject* pSender)
{
    HotelCenter* pScene = new HotelCenter();
	pScene->init();
    CCDirector::sharedDirector()->replaceScene(pScene);
}

void MainView::customerCenterBtnCallBack(CCObject* pSender)
{
    CustomerCenter* pScene = new CustomerCenter();
	pScene->init();
    CCDirector::sharedDirector()->replaceScene(pScene);
}


void MainView::getData()
{
    CCHttpClient* httpClient = CCHttpClient::getInstance();
    
    CCHttpRequest* httpReq =new CCHttpRequest();
    
    httpReq->setRequestType(CCHttpRequest::kHttpGet);
    httpReq->setUrl("http://www.lenonle.com/app/api.aspx?reqcode=11004&memberid=156&pageSize=10&pageIndex=2&sign=");
    httpReq->setResponseCallback(this,callfuncND_selector(MainView::httpReqFinished));
    httpReq->setTag("FirstNet");
    httpClient->setTimeoutForConnect(30);
    httpClient->send(httpReq);
    
    //httpReq->autorelease();
    
}

void MainView::httpReqFinished( CCNode* node,CCObject* obj )
{
    CCHttpResponse* response = (CCHttpResponse*)obj;
    if (!response->isSucceed())
    {
        CCLog("Receive Error! %s\n",response->getErrorBuffer());
        return ;
    }
    
    const char* tag = response->getHttpRequest()->getTag();
    if ( 0 == strcmp("FirstNet",tag))
    {
        vector<char> *data = response->getResponseData();
        int data_length =  data->size();
        string res;
        for (int i = 0;i<data_length;++i)
        {
            res+=(*data)[i];
        }
        res+='\0';
        
        CCLog("%s",res.c_str());
        parserJson(res);
    }
}

void MainView::parserJson(string detail)
{
    //parser jason
    Reader reader;
    
    Value root;
    Value array;

    if (reader.parse(detail, root))
    {
        if (!root["mylist"].isNull())
        {
            array = root.get("mylist", array);
            
            string str = array[0]["dcity"].asString();
            
            CCLOG("%s",str.c_str());
        }
    }
    //parser jason//
    //// create json////
    Value map;
    map["name"] = "this is test";
    FastWriter write;
    string jsonData = write.write(map);
    CCLOG("%s",jsonData.c_str());
    
    Value CreateArray;
    CreateArray["arrKey1"]="arrValue1";
    CreateArray["arrKey2"]="arrValue2";
    CreateArray["arrKey3"]="arrValue3";
    root["arrayKey"] =CreateArray;
    string hJsonData = write.write(root);
    CCLOG("%s",hJsonData.c_str());
    //// create json////
}