/*
 * \file blog.h
 * \brief
 * simplest log wrapper
 *
 * \note
 * author Ben Wei 2014-05
 * copyright MIT License
 **/
#ifndef _BLOG_H
#define _BLOG_H

extern int verbose;

#define BLOG_INIT_ENV() int verbose = 0;

#define BLOG(...)   { if (verbose > 0) printf(__VA_ARGS__); }

#endif /* _BLOG_H */
