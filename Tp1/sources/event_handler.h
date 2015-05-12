#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

class EventHandler {

public:
	virtual int handleSignal(int signal_number) = 0;
	virtual ~EventHandler() {};
};

#endif /* EVENTHANDLER_H_ */
