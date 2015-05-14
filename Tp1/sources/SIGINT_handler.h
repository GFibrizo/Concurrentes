#ifndef SIGINT_HANDLER_H_
#define SIGINT_HANDLER_H_

#include <signal.h>
#include <assert.h>

#include "event_handler.h"

class SIGINT_Handler : public EventHandler {

	private:
		sig_atomic_t graceful_quit;

	public:
		SIGINT_Handler() : graceful_quit(0) {
		}

		~SIGINT_Handler() {
		}

		virtual int handle_signal(int signal_number) {
			assert(signal_number == SIGINT);
			this->graceful_quit = 1;
			return 0;
		}

		sig_atomic_t get_graceful_quit() const {
			return this->graceful_quit;
		}
};

#endif /* SIGINT_HANDLER_H_ */
