#ifndef __PANTHER_H__
#define __PANTHER_H__
#include "Tank.h"

class Panther : public Tank
{
public:

	CREATE_FUNC(Panther);

	virtual ~Panther();

	virtual bool init() override;

	virtual std::string getModelFilePath() override;
	virtual std::string getDamagedModelFilePath() override;

protected:

	Panther();
};

#endif