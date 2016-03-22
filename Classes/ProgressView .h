#ifndef __PROGRESS_VIEW_H__
#define __PROGRESS_VIEW_H__

#include "cocos2d.h"  

class ProgressView : public cocos2d::Node
{
public:

	CREATE_FUNC(ProgressView);

public:
	//设置血条背景  
	void setBackgroundTexture(const char *pName);
	//设置血条前景  
	void setForegroundTexture(const char *pName);
	//设置总血量  
	void setTotalProgress(float total);
	//设置当前血量  
	void setCurrentProgress(float progress);
	//得到当前血量  
	float getCurrentProgress() const;
	//得到总血量  
	float getTotalProgress() const;

	void setForegroundColor(const cocos2d::Color3B &col);

private:
	ProgressView();
	//设置前景血条显示的长度  
	void setForegroundTextureRect(const cocos2d::Rect &rect);

private:
	cocos2d::Sprite *m_progressBackground;//背景血条  
	cocos2d::Sprite *m_progressForeground;//前景血条  
	float m_totalProgress;//总血量  
	float m_currentProgress;//当前血量  
	float m_scale;//放大倍数  
};

#endif