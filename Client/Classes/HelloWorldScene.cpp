#include "HelloWorldScene.h"

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

USING_NS_CC;

// UI関係
const static int PLAYER_TEXT_X = 900;
const static int OTHER_TEXT_X = 50;
const static int TEXT_H = 60;

Scene* HelloWorld::createScene()
{
	auto scene = Scene::create();
	auto layer = HelloWorld::create();

	scene->addChild(layer);

	return scene;
}

bool HelloWorld::init()
{
	if (!Layer::init()){
		return false;
	}

	createContent();

	log("-------> Stast connect");
	_client = SocketIO::connect("ws://127.0.0.1:8080/", *this);
	_client->on("hello", CC_CALLBACK_2(HelloWorld::onReceiveEvent, this));

	return true;
}

void HelloWorld::onReceiveEvent(SIOClient* client, const std::string& data){
	log("Co vao day khong vay");
	log("Data : %s", data.c_str());

	rapidjson::Document document;
	document.Parse<0>(data.c_str());

	log("Data Value : %s", document["value"].GetString());

	if (document.HasParseError()){
		log("//=============Parse Error!!!");
		return;
	}
	
	// Lay data
	if (document.IsObject()){
		if (document.HasMember("value")){
			
			std::string value = document["value"].GetString();
			addTalkOther(value);
		}
	}

};

//=============================================================
// Phan bat buoc phai co
//============================================================
void HelloWorld::onConnect(SIOClient* client){
	// SocketIO::connect success
}

void HelloWorld::onMessage(SIOClient* client, const std::string& data){
	// SocketIO::send receive
}
void HelloWorld::onClose(SIOClient* client){
	// SocketIO::disconnect success
}
void HelloWorld::onError(SIOClient* client, const std::string& data){
	// SocketIO::failed
}

//============================================================
// Xu ly truyen du lieu len server
//============================================================

void HelloWorld::createContent()
{
	index = 0;

	Size visibleSize = Director::getInstance()->getVisibleSize();

	editBox = TextField::create("please input text", "fonts/arial.ttf", 40);
	editBox->setContentSize(Size(visibleSize.width, 50));
	editBox->setPosition(Point(visibleSize.width / 2, 30));
	editBox->addEventListener(CC_CALLBACK_2(HelloWorld::textFieldEvent, this));
	this->addChild(editBox);
}

void HelloWorld::textFieldEvent(Ref *pSender, TextField::EventType type)
{
	TextField* text;
	std::string sendText;
	switch (type)
	{
	// Sau khi dong IME lai
	case TextField::EventType::DETACH_WITH_IME:
		text = (TextField*)pSender;

		//text = dynamic_cast<TextField*>(pSender);
		
		// Gui du lieu tu client len server
		sendText = "[{\"value\":\"" + text->getStringValue() + "\"}]";
		_client->emit("hello", sendText);

		addTalkPlayer(text->getStringValue());

		break;
	default:
		break;
	}
}

void HelloWorld::addTalkPlayer(const std::string& str){
	Size size = Director::getInstance()->getVisibleSize();

	DrawNode* draw = DrawNode::create();

	int originalX = PLAYER_TEXT_X;
	int originalY = size.height - (TEXT_H * (index + 1));

	int x = originalX - 290;
	int y = originalY - 60;
	int w = 300;
	int h = 60;

	Vec2 points[] = {
		Vec2(x, y),
		Vec2(x + w, y),
		Vec2(x + w, y + h),
		Vec2(x, y + h),
	};

	this->addChild(draw);
	draw->drawPolygon(points, 4, Color4F(0, 0.5, 0, 1), 1, Color4F(0, 0, 1, 1));

	auto text = Text::create(str, "fronts/arial.ttf", 40);
	text->setTextHorizontalAlignment(TextHAlignment::RIGHT);
	text->setAnchorPoint(Point(1.0, 1.0));
	text->setPosition(Point(originalX, originalY));

	this->addChild(text);
	index++;
}

/**
* その他UI
*/
void HelloWorld::addTalkOther(const std::string& str){
	Size size = Director::getInstance()->getVisibleSize();

	DrawNode* draw = DrawNode::create();

	int originalX = OTHER_TEXT_X;
	int originalY = size.height - (TEXT_H * (index + 1));

	int x = originalX - 10;
	int y = originalY - 60;
	int w = 300;
	int h = 60;

	Vec2 points[] = {
		Vec2(x, y),
		Vec2(x + w, y),
		Vec2(x + w, y + h),
		Vec2(x, y + h),
	};

	this->addChild(draw);
	draw->drawPolygon(points, 4, Color4F(0.5, 0, 0, 1), 1, Color4F(1, 0, 0, 1));

	auto text = Text::create(str, "fronts/arial.ttf", 40);
	text->setTextHorizontalAlignment(TextHAlignment::LEFT);
	text->setAnchorPoint(Point(0.0, 1.0));
	text->setPosition(Point(originalX, originalY));
	text->setColor(Color3B(255, 255, 0));
	this->addChild(text);
	index++;
}