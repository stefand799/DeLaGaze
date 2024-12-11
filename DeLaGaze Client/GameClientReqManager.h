#pragma once
#include <qobject.h>

class GameClientReqManager:public QObject
{
	Q_OBJECT
public:
	/*TODO: replace the signals emitted by each screen, mainly those for navigation, with the proper server
	 * induced ones, ALSO including signals inside of them!
	 */
	signals:

};

