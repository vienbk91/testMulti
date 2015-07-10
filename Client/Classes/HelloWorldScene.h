#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "network/SocketIO.h"
#include "ui/cocosGUI.h"

using namespace cocos2d::network;
using namespace cocos2d::ui;

class HelloWorld : public cocos2d::Layer, public SocketIO::SIODelegate
{
private:
	SIOClient* _client;

public:

	int index;
	TextField* editBox;
	
	static cocos2d::Scene* createScene();

	// socket.io event listener
	void onReceiveEvent(SIOClient* client, const std::string& data);

	// SIODelegate
	virtual void onConnect(SIOClient* client);
	virtual void onMessage(SIOClient* client, const std::string& data);
	virtual void onClose(SIOClient* client);
	virtual void onError(SIOClient* client, const std::string& data);

	virtual bool init();

	CREATE_FUNC(HelloWorld);

	// UIŽü‚è
	void createContent();
	
	void textFieldEvent(Ref *pSender, TextField::EventType type);
	void addTalkPlayer(const std::string& str);
	void addTalkOther(const std::string& str);
};

#endif // __HELLOWORLD_SCENE_H__