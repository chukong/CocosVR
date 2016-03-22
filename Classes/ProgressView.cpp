#include "ProgressView .h"
USING_NS_CC;

ProgressView::ProgressView()
	: m_progressBackground(NULL)
	, m_progressForeground(NULL)
	, m_totalProgress(0.0f)
	, m_currentProgress(0.0f)
	, m_scale(1.0f)
{

}
void ProgressView::setBackgroundTexture(const char *pName)
{
	m_progressBackground = Sprite::create(pName);
	this->addChild(m_progressBackground);
}

void ProgressView::setForegroundTexture(const char *pName)
{
	m_progressForeground = Sprite::create(pName);
	m_progressForeground->setAnchorPoint(Vec2(0.0f, 0.5f));//ÉèÖÃÃªµã  
	m_progressForeground->setPosition(Vec2(-m_progressForeground->getContentSize().width * 0.5f, 0));
	this->addChild(m_progressForeground);
}

void ProgressView::setForegroundColor(const Color3B &col)
{
	m_progressForeground->setColor(col);
}

void ProgressView::setTotalProgress(float total)
{
	if (m_progressForeground == NULL) { return; }
	m_scale = m_progressForeground->getContentSize().width / total;
	m_totalProgress = total;
	m_currentProgress = total;
}

void ProgressView::setCurrentProgress(float progress)
{
	if (m_progressForeground == NULL) { return; }
	if (progress < 0.0f) { progress = 0.0f; }
	if (progress > m_totalProgress) { progress = m_totalProgress; }
	m_currentProgress = progress;
	float rectWidth = progress * m_scale;
	const Vec2 from = m_progressForeground->getTextureRect().origin;
	const Rect rect = Rect(from.x, from.y, rectWidth, m_progressForeground->getContentSize().height);
	setForegroundTextureRect(rect);
}

void ProgressView::setForegroundTextureRect(const CCRect &rect)
{
	m_progressForeground->setTextureRect(rect);
}



float ProgressView::getCurrentProgress() const
{
	return m_currentProgress;
}

float ProgressView::getTotalProgress() const
{
	return m_totalProgress;
}