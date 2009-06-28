#ifndef _VS_TEXT_H_
#define _VS_TEXT_H_

// vsText.h: interface for the vsText class.
//
//////////////////////////////////////////////////////////////////////
#include "MePrecision.h"

#include <osgText/Text>
#include <osg/Group>
#include <string>

class vsText  
{
public:
	vsText();
	vsText(void *userText);
	virtual ~vsText();
	virtual bool _initialize();
	void addLineOfText(char *t);
    std::string getString();
	virtual void setColor(MeVector4 &c);
	virtual void setPosition(MeVector3 &pos);
	virtual void setCharSize(float chSize){mCharacterSize = chSize;};
	virtual void setMargine(float mar){mMargin = mar;};
	virtual MeVector4 *getColor(){return &mColor;};
	virtual float getCharSize(){return mCharacterSize;};
	virtual float getMargine(){return mMargin;};
	virtual void disable(bool bVal);

protected:
	bool mInitialized;
	void *mUserText;
    std::string vsString;
	MeVector4 mColor;
	MeVector3 mPos;
	float mWinHeight;
	float mWinLength;
    float mCharacterSize;    
    float mMargin ;

};

class osgVsText : public vsText
{
public:
	osgVsText();
	osgVsText(osgText::Text *userText);
	virtual ~osgVsText();
	void addLineOfText(char *t);
	void setFont(osgText::Font* f);
	void setColor(MeVector4 &c);
	osg::Group *getTextNode();
	void setPosition(MeVector3 &pos);
	void disable(bool bVal);
protected:
	bool _initialize();
	osg::Group *mTextNode;
	osg::Geode* mTextGeode;
	osg::MatrixTransform* mModelview_abs;
	osg::Projection* mProjection;
	osgText::Font* mFont;
	osgText::Text* mText;
};

#endif // _VS_TEXT_H_