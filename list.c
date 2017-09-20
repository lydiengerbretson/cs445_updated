/*
 * list.c - Source code for doubly linked circular list.
 *
 * Copyright (C) 2014 Andrew Schwartzmeyer
 *
 * This file released under the AGPLv3.
 *
 */

#include <stdlib.h>
#include <stdio.h>

#include "list.h"

struct list_node *list_node_init()
{
	struct list_node *n = malloc(sizeof(*n));
	if (n == NULL) {
		perror("list_node_init()");
		return NULL;
	}

	n->sentinel = false;
	n->next = NULL;
	n->prev = NULL;
	n->data = NULL;

	return n;
}

struct list *list_init()
{
	struct list *self = malloc(sizeof(*self));
	if (self == NULL) {
		perror("list_init()");
		return NULL;
	}

	struct list_node *sentinel = list_node_init();
	if (sentinel == NULL) {
		free(self);
		return NULL;
	}

	self->sentinel = sentinel;
	self->size = 0;

	sentinel->sentinel = true;
	sentinel->next = sentinel;
	sentinel->prev = sentinel;

	return self;
}

/*
 * Use destroy function to free data of each node, free said node,
 * finally free sentinel and list.
 */
void list_destroy(struct list *self, void (*destroy)(void *data))
{
	if (self == NULL) {
		fprintf(stderr, "list_destroy(): self was null\n");
		return;
	}

	while (!list_empty(self)) {
		void *d = list_pop(self);
		if (destroy != NULL)
			destroy(d);
	}
	free(self->sentinel);
	free(self);
}

size_t list_size(const struct list *self)
{
	if (self == NULL) {
		fprintf(stderr, "list_size(): self was null\n");
		return 0;
	}

	return self->size;
}

bool list_empty(const struct list *self)
{
	if (self == NULL) {
		fprintf(stderr, "list_empty(): self was null\n");
		return false;
	}

	return (self->size == 0);
}

bool list_end(const struct list_node *n)
{
	if (n == NULL) {
		fprintf(stderr, "list_end(): n was null\n");
		return false;
	}

	return n->sentinel;
}

struct list_node *list_head(const struct list *self)
{
	if (self == NULL) {
		fprintf(stderr, "list_tail(): self was null\n");
		return NULL;
	}

	return self->sentinel->next;
}

struct list_node *list_tail(const struct list *self)
{
	if (self == NULL) {
		fprintf(stderr, "list_tail(): self was null\n");
		return NULL;
	}

	return self->sentinel->prev;
}

/*
 * Use compare function to return found node, else returns sentinel.
 */
struct list_node *list_find(const struct list *self, void *data,
                            bool (*compare)(void *a, void *b)) {
	if (self == NULL) {
		fprintf(stderr, "list_find(): self was null\n");
		return NULL;
	}

	struct list_node *iter = list_head(self);
	while (!list_end(iter)) {
		if (compare(data, iter->data))
			return iter;
		iter = iter->next;
	}
	return iter;
}

struct list *list_push(struct list *self, void *data)
{
	if (self == NULL) {
		fprintf(stderr, "list_push(): self was null\n");
		return NULL;
	}

	struct list_node *n = list_node_init();
	if (n == NULL)
		return NULL;

	n->data = data;

	n->prev = self->sentinel->prev;
	n->prev->next = n;

	n->next = self->sentinel;
	self->sentinel->prev = n;

	++self->size;

	return self;
}

struct list *list_push_front(struct list *self, void *data)
{
	if (self == NULL) {
		fprintf(stderr, "list_push_front(): self was null\n");
		return NULL;
	}

	struct list_node *n = list_node_init();
	if (n == NULL)
		return NULL;

	n->data = data;

	n->next = self->sentinel->next;
	n->next->prev = n;

	n->prev = self->sentinel;
	self->sentinel->next = n;

	++self->size;

	return self;
}

void *list_pop(struct list *self)
{
	if (self == NULL) {
		fprintf(stderr, "list_pop(): self was null\n");
		return NULL;
	}

	struct list_node *n = self->sentinel->prev;
	if (list_end(n)) {
		return NULL;
	}

	void *d = n->data;

	self->sentinel->prev = n->prev;
	n->prev->next = self->sentinel;

	free(n);

	--self->size;

	return d;
}

void *list_pop_front(struct list *self)
{
	if (self == NULL) {
		fprintf(stderr, "list_pop_front(): self was null\n");
		return NULL;
	}

	struct list_node *n = self->sentinel->next;
	if (list_end(n))
		return NULL;

	void *d = n->data;

	self->sentinel->next = n->next;
	n->next->prev = self->sentinel;

	free(n);

	--self->size;

	return d;
}

void *list_peek(const struct list *self)
{
	if (self == NULL) {
		fprintf(stderr, "list_peek(): self was null\n");
		return NULL;
	}

	if (list_empty(self))
		return NULL;

	return self->sentinel->prev->data;
}

void *list_peek_front(const struct list *self)
{
	if (self == NULL) {
		fprintf(stderr, "list_peek_front(): self was null\n");
		return NULL;
	}

	if (list_empty(self))
		return NULL;

	return self->sentinel->next->data;
}
