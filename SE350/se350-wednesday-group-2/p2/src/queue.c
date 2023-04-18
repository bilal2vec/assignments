#include "queue.h"
#include "common.h"

int empty(queue* q) {
	return q->head == NULL && q->tail == NULL;
}

int single_element_or_empty(queue* q) {
	return q->head == q->tail;
}

int enqueue(queue *q, node *n) {
	if (q == NULL) {
		debug(("enqueue: FATAL ERROR: queue is NULL\r\n"));

		return RTX_ERR;
	}

	// n will become the tail so clear its next pointer value
	n->next = NULL;

	// Queue is empty
	if (empty(q)) {
		q->head = n;
		q->tail = n;

		return RTX_OK;
	}

	// Add n to the back of the queue
	q->tail->next = n;
	q->tail = n;
	return RTX_OK;
}

// Remove first element from queue and place into result
// Result is NULL is no element found
int dequeue(node **result, queue *q) {
	if (q == NULL) {
		debug(("dequeue: FATAL ERROR: queue is NULL\r\n"));
		
		if (result != NULL){
			*result = NULL;
		}
		return RTX_ERR;
	}
	
	if (result != NULL){
		*result = q->head;
	}
	// Queue is empty
	if (empty(q)) {
		return RTX_OK;
	}
	// Queue has exactly 1 element
	if (single_element_or_empty(q)) {
		q->head = NULL;
		q->tail = NULL;
	}
	// General case
	else if (q->head != NULL && q->head->next != NULL) {
		q->head = q->head->next;
	}
	// Invalid state
	else {
		debug(("dequeue: FATA ERROR: length > 1 queue has invalid state\r\n"));
		return RTX_ERR;
	}

	return RTX_OK;
}

// Get a pointer to the front of the queue (head)
// Result is NULL is no element found
int try_dequeue(node **result, queue *q) {
	if (q == NULL) {
		debug(("dequeue: FATAL ERROR: queue is NULL\r\n"));

		*result = NULL;
		return RTX_ERR;
	}

	*result = q->head;

	return RTX_OK;
}

/*
	pre: q is non-NULL and valid
	post: result contains a non-NULL pointer to the first element
	in the queue satisfying pred, if no such element exists result is NULL
	return is a code OK or ERR
*/
int get_elt_if(node **result, queue *q, BOOL (*pred)(node *, void *), void * pred_param) {
	
	if (result != NULL){
		*result = NULL;
	}

	if (q == NULL) {
		debug(("dequeue: FATAL ERROR: queue is NULL\r\n"));
		return RTX_ERR;
	}

	node *cur = q->head;

	while (cur != NULL) {
		if (pred(cur, pred_param)) {
			if (result != NULL){
				*result = (void *) cur;
			}
			return RTX_OK;
		}
		cur = cur->next;
	}

	return RTX_OK;
}

/*
	pre: q is non-NULL and valid
	post: q contains target node after the first instance where pred is true
	or at the very end if pred never satisfied
*/
int insert_elt_if(queue *q, node *target, BOOL (*pred)(node *, void *), void *pred_param) {
	
	if (target == NULL){
		debug(("insert_elt_if: FATAL ERROR: target node is NULL\r\n"));
		return RTX_ERR;
	}
	
	if (q == NULL) {
		debug(("insert_elt_if: FATAL ERROR: queue is NULL\r\n"));
		return RTX_ERR;
	}	
		
	node *cur = q->head;

	while (cur != NULL) {
		if (pred(cur, pred_param)) {
			if (cur == q->tail) {
				enqueue(q, target);
				return RTX_OK;
			}
			
			node *temp = cur->next;
			cur->next = target;
			target->next = temp;
			
			return RTX_OK;
		}
		cur = cur->next;
	}
	
	// No predicate satisfied
	enqueue(q, target);

	return RTX_OK;
	
}

/*
	pre: q is non-NULL and valid
	post: result contains a non-NULL pointer to the first element
	in the queue satisfying pred, which is now deleted from q
	if no such element exists result is NULL
	return is a code OK or ERR
*/
int delete_elt_if(node **deleted, queue *q, BOOL (*pred)(node *, void *), void * pred_param) {
	
	if (deleted != NULL){
		*deleted = NULL;
	}
	
	if (q == NULL) {
		debug(("dequeue: FATAL ERROR: queue is NULL\r\n"));
		return RTX_ERR;
	}

	if (empty(q)) {
		// Nothing in q means nothing to delete
		return RTX_OK;
	}

	// Check if the head satisfies the predicate
	// If so, delete it
	if (pred(q->head, pred_param)) {
		if (deleted != NULL) {
			*deleted = (void *) q->head;
		}
		if (single_element_or_empty(q)) {
			q->head = NULL;
			q->tail = NULL;
			return RTX_OK;
		}
		q->head = q->head->next;
		return RTX_OK;
	}

	node *cur = q->head;

	// Otherwise, check the rest of the queue
	// If we find an element satisfying the predicate, delete it
	while (cur->next != NULL) {
		if (pred(cur->next, pred_param)) {
			if (deleted != NULL){
				*deleted = cur->next;
				if (*deleted == q->tail) {
					q->tail = cur;
				}
			}
			cur->next = cur->next->next;
			return RTX_OK;
		}
		cur = cur->next;
	}

	// No element found to delete
	return RTX_OK;
}

// Move take the first element in queue 'from' satisfying pred
// nd move it out of the 'from' queue and enqueue it to the 'to' queue
int move_elt_if(node** moved, queue* from, queue* to, BOOL (*pred)(node *, void *), void * pred_param) {
	
	if (moved != NULL) {
		*moved = NULL;
	}
	
	if (delete_elt_if(moved, from, pred, pred_param) != RTX_OK) {
		debug(("move_elt_if: FATAL ERROR failed to delete from queue \r\n"));
		return RTX_ERR;
	}

	if (*moved == NULL) {
		debug(("move_elt_if: nothing deleted\r\n"));
		return RTX_OK;
	}

	if (enqueue(to, (node *) *moved) != RTX_OK) {
		debug(("move_elt_if: FATAL ERROR: failed to enqueue\r\n"));
		return RTX_ERR;
	}

	return RTX_OK;
}

