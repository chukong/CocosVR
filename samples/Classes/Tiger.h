#ifndef __TIGER_H__
#define __TIGER_H__
#include "Tank.h"

class Tiger : public Tank
{
public:

	CREATE_FUNC(Tiger);

	virtual ~Tiger();

	virtual bool init() override;
	virtual void update(float delta) override;

	virtual std::string getModelFilePath() override;
	virtual std::string getDamagedModelFilePath() override;

protected:

	Tiger();

};

#endif