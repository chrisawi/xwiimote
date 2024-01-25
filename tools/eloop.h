/*
 * XWiimote
 *
 * Copyright (c) 2011,2012 David Herrmann <dh.herrmann@googlemail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * Event Loop
 * This provides a basic event loop similar to those provided by glib etc.
 * It uses linux specific features like signalfd so it may not be easy to port
 * it to other platforms.
 */

#ifndef EV_ELOOP_H
#define EV_ELOOP_H

#include <inttypes.h>
#include <stdlib.h>
#include <time.h>

struct ev_eloop;
struct ev_idle;
struct ev_fd;
struct ev_signal;
struct ev_timer;

typedef void (*ev_idle_cb) (struct ev_idle *idle, void *data);
typedef void (*ev_fd_cb) (struct ev_fd *fd, int mask, void *data);
typedef void (*ev_signal_cb) (struct ev_signal *sig,
				int signum,
				void *data);
typedef void (*ev_timer_cb) (struct ev_timer *timer,
				uint64_t num,
				void *data);

enum ev_eloop_flags {
	EV_READABLE = 0x01,
	EV_WRITEABLE = 0x02,
	EV_HUP = 0x04,
	EV_ERR = 0x08,
};

int ev_eloop_new(struct ev_eloop **out);
void ev_eloop_ref(struct ev_eloop *loop);
void ev_eloop_unref(struct ev_eloop *loop);

int ev_eloop_get_fd(struct ev_eloop *loop);
int ev_eloop_dispatch(struct ev_eloop *loop, int timeout);

/* idle sources */

int ev_idle_new(struct ev_idle **out);
void ev_idle_ref(struct ev_idle *idle);
void ev_idle_unref(struct ev_idle *idle);

int ev_eloop_new_idle(struct ev_eloop *loop,
			struct ev_idle **out,
			ev_idle_cb cb,
			void *data);
int ev_eloop_add_idle(struct ev_eloop *loop,
			struct ev_idle *idle,
			ev_idle_cb cb,
			void *data);
void ev_eloop_rm_idle(struct ev_idle *idle);

/* fd sources */

int ev_fd_new(struct ev_fd **out);
void ev_fd_ref(struct ev_fd *fd);
void ev_fd_unref(struct ev_fd *fd);

int ev_eloop_new_fd(struct ev_eloop *loop,
			struct ev_fd **out,
			int rfd,
			int mask,
			ev_fd_cb cb,
			void *data);
int ev_eloop_add_fd(struct ev_eloop *loop,
			struct ev_fd *fd,
			int rfd,
			int mask,
			ev_fd_cb cb,
			void *data);
void ev_eloop_rm_fd(struct ev_fd *fd);
int ev_eloop_update_fd(struct ev_fd *fd, int mask);

/* signal sources */

int ev_signal_new(struct ev_signal **out);
void ev_signal_ref(struct ev_signal *sig);
void ev_signal_unref(struct ev_signal *sig);

int ev_eloop_new_signal(struct ev_eloop *loop,
			struct ev_signal **out,
			int signum,
			ev_signal_cb cb,
			void *data);
int ev_eloop_add_signal(struct ev_eloop *loop,
			struct ev_signal *sig,
			int signum,
			ev_signal_cb cb,
			void *data);
void ev_eloop_rm_signal(struct ev_signal *sig);

/* timer sources */

int ev_timer_new(struct ev_timer **out);
void ev_timer_ref(struct ev_timer *timer);
void ev_timer_unref(struct ev_timer *timer);

int ev_eloop_new_timer(struct ev_eloop *loop,
			struct ev_timer **out,
			const struct itimerspec *spec,
			ev_timer_cb cb,
			void *data);
int ev_eloop_add_timer(struct ev_eloop *loop,
			struct ev_timer *timer,
			const struct itimerspec *spec,
			ev_timer_cb cb,
			void *data);
void ev_eloop_rm_timer(struct ev_timer *timer);
int ev_eloop_update_timer(struct ev_timer *timer,
			const struct itimerspec *spec);

#endif /* EV_ELOOP_H */
